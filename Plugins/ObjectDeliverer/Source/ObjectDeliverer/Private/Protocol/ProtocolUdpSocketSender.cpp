// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolUdpSocketSender.h"
#include "Common/UdpSocketBuilder.h"
#include "HAL/RunnableThread.h"
#include "PacketRule/PacketRule.h"

UProtocolUdpSocketSender::UProtocolUdpSocketSender()
{
}

UProtocolUdpSocketSender::~UProtocolUdpSocketSender()
{
}

void UProtocolUdpSocketSender::Initialize(const FString &IpAddress, int32 Port)
{
	DestinationIpAddress = IpAddress;
	DestinationPort = Port;
}

UProtocolUdpSocketSender *UProtocolUdpSocketSender::WithSendBufferSize(int32 SizeInBytes)
{
	SendBufferSize = SizeInBytes;

	return this;
}

UProtocolUdpSocketSender *UProtocolUdpSocketSender::WithBroadcast(bool InEnableBroadcast)
{
	bEnableBroadcast = InEnableBroadcast;

	return this;
}

void UProtocolUdpSocketSender::Start()
{
	auto endPoint = GetIP4EndPoint(DestinationIpAddress, DestinationPort);
	if (!endPoint.Get<0>())
		return;

	DestinationEndpoint = endPoint.Get<1>();

	InnerSocket = FUdpSocketBuilder(TEXT("ObjectDeliverer UdpSocket"))
					  .WithSendBufferSize(SendBufferSize)
					  .Build();

	if (InnerSocket)
	{
		if (bEnableBroadcast)
		{
			InnerSocket->SetBroadcast(true);
		}

		DispatchConnected(this);
	}
}

void UProtocolUdpSocketSender::Close()
{
	CloseInnerSocket();
}

void UProtocolUdpSocketSender::Send(const TArray<uint8> &DataBuffer, const FDeliveryDataType& KindOfData) const
{
	PacketRule->MakeSendPacket(DataBuffer, KindOfData);
}

void UProtocolUdpSocketSender::RequestSend(const TArray<uint8> &DataBuffer, const FDeliveryDataType& DataType)
{
	SendTo(DataBuffer, DestinationEndpoint);
}