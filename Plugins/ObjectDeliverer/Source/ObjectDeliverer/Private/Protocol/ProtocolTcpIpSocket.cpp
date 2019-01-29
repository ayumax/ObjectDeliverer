#include "ProtocolTcpIpSocket.h"
#include "TcpSocketBuilder.h"
#include "Utils/WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "PacketRule.h"

UProtocolTcpIpSocket::UProtocolTcpIpSocket()
{

}

UProtocolTcpIpSocket::~UProtocolTcpIpSocket()
{
}


void UProtocolTcpIpSocket::Close()
{
	CloseSocket(true);
}


void UProtocolTcpIpSocket::CloseSocket(bool Wait)
{
	if (!InnerSocket) return;

	if (!InnerSocket) return;

	CloseInnerSocket();

	if (!CurrentThread) return;
	CurrentThread->Kill(Wait);

	delete CurrentThread;
	CurrentThread = nullptr;

	if (!CurrentInnerThread) return;
	delete CurrentInnerThread;
	CurrentInnerThread = nullptr;

}

void UProtocolTcpIpSocket::Send(const TArray<uint8>& DataBuffer)
{
	if (!InnerSocket) return;

	PacketRule->MakeSendPacket(DataBuffer);
}

void UProtocolTcpIpSocket::OnConnected(FSocket* ConnectionSocket)
{
	InnerSocket = ConnectionSocket;
	StartPollilng();
}

void UProtocolTcpIpSocket::StartPollilng()
{
	ReceiveBuffer.SetNum(1024);
	CurrentInnerThread = new FWorkerThread([this] { return ReceivedData(); });
	CurrentThread = FRunnableThread::Create(CurrentInnerThread, TEXT("ObjectDeliverer TcpIpSocket PollingThread"));
}

bool UProtocolTcpIpSocket::ReceivedData()
{
	if (!InnerSocket)
	{
		DispatchDisconnected(this);
		return false;
	}

	// check if the socket has closed
	{
		int32 BytesRead;
		uint8 Dummy;
		if (!InnerSocket->Recv(&Dummy, 1, BytesRead, ESocketReceiveFlags::Peek))
		{
			CloseInnerSocket();
			DispatchDisconnected(this);
			return false;
		}
	}

	// Block waiting for some data
	if (!InnerSocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(0.1)))
	{
		if (InnerSocket->GetConnectionState() == SCS_ConnectionError)
		{
			CloseInnerSocket();
			DispatchDisconnected(this);
			return false;
		}
		return true;
	}

	uint32 Size = 0;
	while (InnerSocket->HasPendingData(Size))
	{
		uint32 wantSize = PacketRule->GetWantSize();

		if (wantSize > 0)
		{
			if (Size < wantSize) return true;
		}

		auto receiveSize = wantSize == 0 ? Size : wantSize;

		ReceiveBuffer.SetNum(receiveSize, false);

		int32 Read = 0;
		if (!InnerSocket->Recv(ReceiveBuffer.GetData(), ReceiveBuffer.Num(), Read, ESocketReceiveFlags::WaitAll))
		{
			CloseInnerSocket();
			DispatchDisconnected(this);
			return false;
		}

		PacketRule->NotifyReceiveData(ReceiveBuffer);
	}

	return true;
}

void UProtocolTcpIpSocket::RequestSend(const TArray<uint8>& DataBuffer)
{
	SendToConnected(DataBuffer);
}