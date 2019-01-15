#include "CNTcpIpSocket.h"
#include "TcpSocketBuilder.h"
#include "WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "CNPacketRule.h"

UCNTcpIpSocket::UCNTcpIpSocket()
{

}

UCNTcpIpSocket::~UCNTcpIpSocket()
{
}


void UCNTcpIpSocket::Close_Implementation()
{
	CloseSocket(true);
}


void UCNTcpIpSocket::CloseSocket(bool Wait)
{
	if (!InnerSocket) return;

	CurrentThread->Kill(Wait);
	delete CurrentThread;
	CurrentThread = nullptr;

	delete CurrentInnerThread;
	CurrentInnerThread = nullptr;

	CloseInnerSocket();
}

void UCNTcpIpSocket::Send_Implementation(const TArray<uint8>& DataBuffer)
{
	if (!InnerSocket) return;

	PacketRule->MakeSendPacket(DataBuffer);
}

void UCNTcpIpSocket::OnConnected(FSocket* ConnectionSocket)
{
	InnerSocket = ConnectionSocket;
	StartPollilng();
}

void UCNTcpIpSocket::StartPollilng()
{
	ReceiveBuffer.SetNum(1024);
	CurrentInnerThread = new FWorkerThread([this] { ReceivedData(); });
	CurrentThread = FRunnableThread::Create(CurrentInnerThread, TEXT("CommNet TcpIpSocket PollingThread"));
}

void UCNTcpIpSocket::ReceivedData()
{
	TArray<uint8> BodyBuffer;

	uint32 Size = 0;
	while (InnerSocket->HasPendingData(Size))
	{
		uint32 wantSize = PacketRule->GetWantSize();

		if (wantSize > 0)
		{
			if (Size < wantSize) return;
		}

		auto receiveSize = wantSize == 0 ? Size : wantSize;

		ReceiveBuffer.SetNum(receiveSize, false);

		int32 Read = 0;
		if (!InnerSocket->Recv(ReceiveBuffer.GetData(), ReceiveBuffer.Num(), Read, ESocketReceiveFlags::WaitAll))
		{
			DispatchDisconnected(this);
			CloseSocket(false);
			break;
		}

		PacketRule->NotifyReceiveData(ReceiveBuffer);
	}

	if (InnerSocket->GetConnectionState() != ESocketConnectionState::SCS_Connected)
	{
		DispatchDisconnected(this);
		CloseSocket(false);
	}
}

void UCNTcpIpSocket::RequestSend(const TArray<uint8>& DataBuffer)
{
	SendToConnected(DataBuffer);
}