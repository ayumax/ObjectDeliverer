#include "CNTcpIpClient.h"
#include "TcpSocketBuilder.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UCNTcpIpClient::UCNTcpIpClient()
{

}

UCNTcpIpClient::~UCNTcpIpClient()
{

}

void UCNTcpIpClient::Initialize(const FString& IpAddress, int32 Port, bool Retry/* = false*/)
{
	ServerIpAddress = IpAddress;
	ServerPort = Port;
	RetryConnect = Retry;
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

	ConnectEndPoint = endPoint.Get<1>();
	InnerSocket = socket;

	TryConnect();
}

void UCNTcpIpClient::TryConnect()
{
	if (InnerSocket->Connect(ConnectEndPoint.ToInternetAddr().Get()))
	{
		DispatchConnected(this);

		OnConnected(InnerSocket);
	}
	else if(RetryConnect)
	{
		GetWorld()->GetTimerManager().SetTimer(ConnectTimerHandle, this, &UCNTcpIpClient::TryConnect, 1.0f, false, 1.0f);
	}
}

void UCNTcpIpClient::Close_Implementation()
{
	Super::Close_Implementation();

	auto world = GetWorld();
	if (world)
	{
		world->GetTimerManager().ClearTimer(ConnectTimerHandle);
	}
}