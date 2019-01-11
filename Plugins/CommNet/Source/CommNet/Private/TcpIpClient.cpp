#include "TcpIpClient.h"
#include "TcpSocketBuilder.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UTcpIpClient::UTcpIpClient()
{

}

UTcpIpClient::~UTcpIpClient()
{

}

bool UTcpIpClient::Connect(const FString& IpAddress, int32 Port)
{
	Close();

	auto socket = FTcpSocketBuilder(TEXT("CommNet TcpIpClient"))
		.AsBlocking()
		.WithReceiveBufferSize(1024 * 1024)
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
	
	Connected.Broadcast();

	OnConnected(socket);

	return true;
}

bool UTcpIpClient::FormatIP4ToNumber(const FString& IpAddress, uint8(&Out)[4])
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
