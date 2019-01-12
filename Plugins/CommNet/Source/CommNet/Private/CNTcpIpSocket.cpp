#include "CNTcpIpSocket.h"
#include "TcpSocketBuilder.h"
#include "WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UCNTcpIpSocket::UCNTcpIpSocket()
	: ReceiveMode(EReceiveMode::Size)
	, BodySize(0)
{

}

UCNTcpIpSocket::~UCNTcpIpSocket()
{

}


void UCNTcpIpSocket::Close()
{
	CloseSocket(true);
}

void UCNTcpIpSocket::BeginDestroy()
{
	Super::BeginDestroy();

	CloseSocket(true);
}

void UCNTcpIpSocket::CloseSocket(bool Wait)
{
	if (!InnerSocket) return;

	CurrentThread->Kill(Wait);

	CloseInnerSocket();
}

void UCNTcpIpSocket::Send(const TArray<uint8>& DataBuffer)
{
	if (!InnerSocket) return;

	TArray<uint8> buffer;
	buffer.Reset(4 + DataBuffer.Num());
	buffer.SetNum(4);

	buffer[0] = (uint8)((DataBuffer.Num() >> 24) & 0xFF);
	buffer[1] = (uint8)((DataBuffer.Num() >> 16) & 0xFF);
	buffer[2] = (uint8)((DataBuffer.Num() >> 8) & 0xFF);
	buffer[3] = (uint8)(DataBuffer.Num() & 0xFF);

	buffer.Append(DataBuffer);

	SendToConnected(buffer);
}

void UCNTcpIpSocket::OnConnected(FSocket* ConnectionSocket)
{
	InnerSocket = ConnectionSocket;
	StartPollilng();
}

void UCNTcpIpSocket::StartPollilng()
{
	ReceiveMode = EReceiveMode::Size;
	ReceiveBuffer.SetNum(1024);

	auto PollingThread = new FWorkerThread([this] { ReceivedData(); });
	CurrentThread = FRunnableThread::Create(PollingThread, TEXT("CommNet TcpIpSocket PollingThread"));
}

void UCNTcpIpSocket::ReceivedData()
{
	uint32 Size = 0;
	while (InnerSocket->HasPendingData(Size))
	{
		if (Size < WantSize()) return;

		int32 Read = 0;
		if (!InnerSocket->Recv(ReceiveBuffer.GetData(), WantSize(), Read, ESocketReceiveFlags::WaitAll))
		{
			Disconnected.Broadcast(this);
			CloseSocket(false);
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

uint32 UCNTcpIpSocket::WantSize()
{
	if (ReceiveMode == EReceiveMode::Size)
	{
		return 4;
	}

	return BodySize;
}

void UCNTcpIpSocket::OnReceivedSize()
{
	BodySize = ReceiveBuffer[0] << 24 | ReceiveBuffer[1] << 16 | ReceiveBuffer[2] << 8 | ReceiveBuffer[3];

	if (BodySize > (uint32)ReceiveBuffer.Num())
	{
		ReceiveBuffer.SetNum(BodySize, false);
	}

	ReceiveMode = EReceiveMode::Body;
}

void UCNTcpIpSocket::OnReceivedBody()
{
	ReceiveData.Broadcast(this, ReceiveBuffer, (int32)BodySize);

	ReceiveMode = EReceiveMode::Size;
}