#include "TcpIpSocket.h"
#include "TcpSocketBuilder.h"
#include "SocketPollingThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UTcpIpSocket::UTcpIpSocket()
	: ReceiveMode(EReceiveMode::Size)
	, BodySize(0)
{

}

UTcpIpSocket::~UTcpIpSocket()
{

}

bool UTcpIpSocket::Connect(const FString& IpAddress, int32 Port)
{
	Close();

	auto socket = FTcpSocketBuilder(TEXT("CommNet TcpIpClient"))
		.AsNonBlocking()
		.Build();

	if (socket == nullptr) return false;

	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(IpAddress, IP4Nums))
	{
		UE_LOG(LogTemp, Error, TEXT("UTcpIpSocket::ipaddress format violation"));
		return false;
	}

	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), Port);

	if (!socket->Connect(Endpoint.ToInternetAddr().Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UTcpIpSocket::Failed to connect to the server"));
		return false;
	}
	
	ConnectedSocket = socket;
	int32 newSize;
	ConnectedSocket->SetReceiveBufferSize(1024 * 1024, newSize);

	Connected.Broadcast();

	StartPollilng();

	return true;
}

void UTcpIpSocket::Close()
{
	if (!ConnectedSocket) return;

	CurrentThread->Kill(true);
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

bool UTcpIpSocket::FormatIP4ToNumber(const FString& IpAddress, uint8(&Out)[4])
{
	auto _ip = IpAddress.ToLower();
	if (_ip == TEXT("localhost"))
	{
		Out[0] = 127;
		Out[1] = 0;
		Out[2] = 0;
		Out[3] = 1;
		return true;
	}

	_ip.Replace(TEXT(" "), TEXT(""));

	const TCHAR* Delims[] = { TEXT(".") };
	TArray<FString> Parts;
	_ip.ParseIntoArray(Parts, Delims, true);
	if (Parts.Num() != 4)
		return false;

	for (int32 i = 0; i < 4; ++i)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}

	return true;
}

void UTcpIpSocket::StartPollilng()
{
	ReceiveMode = EReceiveMode::Size;
	ReceiveBuffer.Reset(1024);

	PollingThread = new FSocketPollingThread([this] { ReceivedData(); });
	CurrentThread = FRunnableThread::Create(PollingThread, TEXT("CommNet TcpIpClient PollingThread"));

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
			CurrentThread->Kill(false);
			ConnectedSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectedSocket);

			ConnectedSocket = nullptr;
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