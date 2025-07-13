// Copyright 2019 ayumax. All Rights Reserved.
#include "PacketRule/PacketRule.h"

UPacketRule::UPacketRule()
{

}

UPacketRule::~UPacketRule()
{
}

void UPacketRule::Initialize()
{

}

void UPacketRule::MakeSendPacket(const TArray<uint8>& BodyBuffer, const FDeliveryDataType& DataType)
{
}

void UPacketRule::NotifyReceiveData(const TArray<uint8>& DataBuffer)
{
}

int32 UPacketRule::GetWantSize()
{
	return 0;
}

UPacketRule* UPacketRule::Clone()
{

	return nullptr;
}

void UPacketRule::DispatchMadeSendBuffer(const TArray<uint8>& SendBuffer, const FDeliveryDataType& DataType)
{
	MadeSendBuffer.ExecuteIfBound(SendBuffer, DataType);
}

void UPacketRule::DispatchMadeReceiveBuffer(const TArray<uint8>& ReceiveBuffer)
{
	MadeReceiveBuffer.ExecuteIfBound(ReceiveBuffer);
}