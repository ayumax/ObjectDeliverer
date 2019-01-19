#include "ProtocolTcpIpClient.h"
#include "TcpSocketBuilder.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UProtocolTcpIpClient::UProtocolTcpIpClient()
{

}

UProtocolTcpIpClient::~UProtocolTcpIpClient()
{

}

void UProtocolTcpIpClient::Initialize(const FString& IpAddress, int32 Port, bool Retry/* = false*/)
{
	ServerIpAddress = IpAddress;
	ServerPort = Port;
	RetryConnect = Retry;
}

void UProtocolTcpIpClient::Start_Implementation()
{
	CloseSocket(true);

	auto socket = FTcpSocketBuilder(TEXT("ObjectDeliverer TcpIpClient"))
		.AsBlocking()
		.WithReceiveBufferSize(1024 * 1024)
		.Build();

	if (socket == nullptr) return;

	auto endPoint = GetIP4EndPoint(ServerIpAddress, ServerPort);
	if (!endPoint.Get<0>()) return;

	ConnectEndPoint = endPoint.Get<1>();
	InnerSocket = socket;

	GWorld->GetTimerManager().SetTimer(ConnectTimerHandle, this, &UProtocolTcpIpClient::TryConnect, 0.1f, false, 0.1f);
}

void UProtocolTcpIpClient::TryConnect()
{
	if (InnerSocket->Connect(ConnectEndPoint.ToInternetAddr().Get()))
	{
		DispatchConnected(this);

		OnConnected(InnerSocket);
	}
	else if(RetryConnect)
	{
		GWorld->GetTimerManager().SetTimer(ConnectTimerHandle, this, &UProtocolTcpIpClient::TryConnect, 1.0f, false, 1.0f);
	}
}

void UProtocolTcpIpClient::Close_Implementation()
{
	Super::Close_Implementation();

	if (GWorld)
	{
		GWorld->GetTimerManager().ClearTimer(ConnectTimerHandle);
	}
}