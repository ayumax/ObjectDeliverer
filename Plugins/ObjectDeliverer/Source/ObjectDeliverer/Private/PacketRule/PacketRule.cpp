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

void UPacketRule::MakeSendPacket(const TArray<uint8>& BodyBuffer)
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

void UPacketRule::DispatchMadeSendBuffer(const TArray<uint8>& SendBuffer)
{
	MadeSendBuffer.ExecuteIfBound(SendBuffer);
}

void UPacketRule::DispatchMadeReceiveBuffer(const TArray<uint8>& ReceiveBuffer)
{
	MadeReceiveBuffer.ExecuteIfBound(ReceiveBuffer);
}