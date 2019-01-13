#include "CNUdpSocket.h"
#include "UdpSocketBuilder.h"
#include "WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UCNUdpSocket::UCNUdpSocket()
{

}

UCNUdpSocket::~UCNUdpSocket()
{

}

void UCNUdpSocket::Initialize(const FString& IpAddress, int32 Port)
{
	DestinationIpAddress = IpAddress;
	DestinationPort = Port;
}

void UCNUdpSocket::Start_Implementation()
{
	auto endPoint = GetIP4EndPoint(DestinationIpAddress, DestinationPort);
	if (!endPoint.Get<0>()) return;

	DestinationEndpoint = endPoint.Get<1>();

	InnerSocket = FUdpSocketBuilder(TEXT("CommNet UdpSocket"))
		.AsBlocking()
		.WithReceiveBufferSize(1024 * 1024)
		.Build();

	OnStart();
}

void UCNUdpSocket::Close_Implementation()
{
	CloseInnerSocket();

	OnClose();
}

void UCNUdpSocket::Send_Implementation(const TArray<uint8>& DataBuffer)
{
	SendTo(DataBuffer, DestinationEndpoint);
}
