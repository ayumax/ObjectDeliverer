#include "CNTcpIpClient.h"
#include "TcpSocketBuilder.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UCNTcpIpClient::UCNTcpIpClient()
{

}

UCNTcpIpClient::~UCNTcpIpClient()
{

}

void UCNTcpIpClient::Initialize(const FString& IpAddress, int32 Port)
{
	ServerIpAddress = IpAddress;
	ServerPort = Port;
}

void UCNTcpIpClient::Start_Implementation()
{
	CloseSocket(true);

	auto socket = FTcpSocketBuilder(TEXT("CommNet TcpIpClient"))
		.AsBlocking()
		.WithReceiveBufferSize(1024 * 1024)
		.Build();

	if (socket == nullptr) return;

	auto endPoint = GetIP4EndPoint(ServerIpAddress, ServerPort);
	if (!endPoint.Get<0>()) return;

	if (!socket->Connect(endPoint.Get<1>().ToInternetAddr().Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCNTcpIpSocket::Failed to connect to the server"));
		return;
	}

	Connected.Broadcast(this);

	OnConnected(socket);
}