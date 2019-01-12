#include "CNTcpIpClient.h"
#include "TcpSocketBuilder.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UCNTcpIpClient::UCNTcpIpClient()
{

}

UCNTcpIpClient::~UCNTcpIpClient()
{

}

bool UCNTcpIpClient::Connect(const FString& IpAddress, int32 Port)
{
	CloseSocket(true);

	auto socket = FTcpSocketBuilder(TEXT("CommNet TcpIpClient"))
		.AsBlocking()
		.WithReceiveBufferSize(1024 * 1024)
		.Build();

	if (socket == nullptr) return false;

	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(IpAddress, IP4Nums))
	{
		UE_LOG(LogTemp, Error, TEXT("UCNTcpIpSocket::ipaddress format violation"));
		return false;
	}

	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), Port);

	if (!socket->Connect(Endpoint.ToInternetAddr().Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCNTcpIpSocket::Failed to connect to the server"));
		return false;
	}
	
	Connected.Broadcast(this);

	OnConnected(socket);

	return true;
}
