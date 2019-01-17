#include "CNUdpSocketSender.h"
#include "UdpSocketBuilder.h"
#include "Utils/WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "CNPacketRule.h"

UCNUdpSocketSender::UCNUdpSocketSender()
{

}

UCNUdpSocketSender::~UCNUdpSocketSender()
{

}

void UCNUdpSocketSender::Initialize(const FString& IpAddress, int32 Port)
{
	DestinationIpAddress = IpAddress;
	DestinationPort = Port;
}

void UCNUdpSocketSender::Start_Implementation()
{
	auto endPoint = GetIP4EndPoint(DestinationIpAddress, DestinationPort);
	if (!endPoint.Get<0>()) return;

	DestinationEndpoint = endPoint.Get<1>();

	InnerSocket = FUdpSocketBuilder(TEXT("CommNet UdpSocket"))
		.WithReceiveBufferSize(1024 * 1024)
		.Build();
}

void UCNUdpSocketSender::Close_Implementation()
{
	CloseInnerSocket();
}

void UCNUdpSocketSender::Send_Implementation(const TArray<uint8>& DataBuffer)
{
	PacketRule->MakeSendPacket(DataBuffer);
}

void UCNUdpSocketSender::RequestSend(const TArray<uint8>& DataBuffer)
{
	SendTo(DataBuffer, DestinationEndpoint);
}