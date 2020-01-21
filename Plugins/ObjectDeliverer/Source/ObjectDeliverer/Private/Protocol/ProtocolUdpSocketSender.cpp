// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolUdpSocketSender.h"
#include "Common/UdpSocketBuilder.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "PacketRule/PacketRule.h"

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

void UProtocolUdpSocketSender::Start()
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

void UProtocolUdpSocketSender::Close()
{
	CloseInnerSocket();
}

void UProtocolUdpSocketSender::Send(const TArray<uint8>& DataBuffer) const
{
	PacketRule->MakeSendPacket(DataBuffer);
}

void UProtocolUdpSocketSender::RequestSend(const TArray<uint8>& DataBuffer)
{
	SendTo(DataBuffer, DestinationEndpoint);
}