#include "TcpIpSocket.h"
#include "TcpSocketBuilder.h"
#include "WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UTcpIpSocket::UTcpIpSocket()
	: ReceiveMode(EReceiveMode::Size)
	, BodySize(0)
{

}

UTcpIpSocket::~UTcpIpSocket()
{

}


void UTcpIpSocket::Close(bool Wait /*=true*/)
{
	if (!ConnectedSocket) return;

	CurrentThread->Kill(Wait);
	ConnectedSocket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectedSocket);

	ConnectedSocket = nullptr;
}

void UTcpIpSocket::Send(const TArray<uint8>& DataBuffer)
{
	if (!ConnectedSocket) return;

	int32 BytesSent;
	ConnectedSocket->Send(DataBuffer.GetData(), DataBuffer.Num(), BytesSent);
}

void UTcpIpSocket::OnConnected(FSocket* ConnectionSocket)
{
	ConnectedSocket = ConnectionSocket;
	StartPollilng();
}

void UTcpIpSocket::StartPollilng()
{
	ReceiveMode = EReceiveMode::Size;
	ReceiveBuffer.Reset(1024);

	auto PollingThread = new FWorkerThread([this] { ReceivedData(); });
	CurrentThread = FRunnableThread::Create(PollingThread, TEXT("CommNet TcpIpSocket PollingThread"));

}

void UTcpIpSocket::ReceivedData()
{
	uint32 Size;
	while (ConnectedSocket->HasPendingData(Size))
	{
		if (Size < WantSize()) return;

		int32 Read = 0;
		if (!ConnectedSocket->Recv(ReceiveBuffer.GetData(), WantSize(), Read, ESocketReceiveFlags::WaitAll))
		{
			Disconnected.Broadcast();
			Close(false);
			break;
		}

		if (ReceiveMode == EReceiveMode::Size)
		{
			OnReceivedSize();
		}
		else
		{
			OnReceivedBody();
		}
	}

}

uint32 UTcpIpSocket::WantSize()
{
	if (ReceiveMode == EReceiveMode::Size)
	{
		return 4;
	}

	return BodySize;
}

void UTcpIpSocket::OnReceivedSize()
{
	BodySize = ReceiveBuffer[0] << 24 | ReceiveBuffer[1] << 16 | ReceiveBuffer[2] << 8 | ReceiveBuffer[3];

	if (BodySize > (uint32)ReceiveBuffer.Num())
	{
		ReceiveBuffer.Reset(BodySize);
	}

	ReceiveMode = EReceiveMode::Body;
}

void UTcpIpSocket::OnReceivedBody()
{
	ReceiveData.Broadcast(ReceiveBuffer, (int32)BodySize);

	ReceiveMode = EReceiveMode::Size;
}