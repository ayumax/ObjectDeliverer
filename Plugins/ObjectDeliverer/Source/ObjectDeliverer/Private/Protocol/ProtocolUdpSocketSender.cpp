#include "ProtocolUdpSocketSender.h"
#include "UdpSocketBuilder.h"
#include "Utils/WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "PacketRule.h"

UProtocolUdpSocketSender::UProtocolUdpSocketSender()
{

}

UProtocolUdpSocketSender::~UProtocolUdpSocketSender()
{

}

void UProtocolUdpSocketSender::Initialize(const FString& IpAddress, int32 Port)
{
	DestinationIpAddress = IpAddress;
	DestinationPort = Port;
}

void UProtocolUdpSocketSender::Start_Implementation()
{
	auto endPoint = GetIP4EndPoint(DestinationIpAddress, DestinationPort);
	if (!endPoint.Get<0>()) return;

	DestinationEndpoint = endPoint.Get<1>();

	InnerSocket = FUdpSocketBuilder(TEXT("ObjectDeliverer UdpSocket"))
		.WithReceiveBufferSize(1024 * 1024)
		.Build();

	if (InnerSocket)
	{
		DispatchConnected(this);
	}
}

void UProtocolUdpSocketSender::Close_Implementation()
{
	CloseInnerSocket();
}

void UProtocolUdpSocketSender::Send_Implementation(const TArray<uint8>& DataBuffer)
{
	PacketRule->MakeSendPacket(DataBuffer);
}

void UProtocolUdpSocketSender::RequestSend(const TArray<uint8>& DataBuffer)
{
	SendTo(DataBuffer, DestinationEndpoint);
}