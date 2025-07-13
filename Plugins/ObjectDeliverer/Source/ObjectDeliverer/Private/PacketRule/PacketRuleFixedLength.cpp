// Copyright 2019 ayumax. All Rights Reserved.
#include "PacketRule/PacketRuleFixedLength.h"
#include "PacketRule/PacketRuleFactory.h"

UPacketRuleFixedLength::UPacketRuleFixedLength()
{

}

UPacketRuleFixedLength::~UPacketRuleFixedLength()
{
}

void UPacketRuleFixedLength::Initialize()
{
	BufferForSend.SetNum(FixedSize);
}

void UPacketRuleFixedLength::MakeSendPacket(const TArray<uint8>& BodyBuffer, const FDeliveryDataType& DataType)
{
	FMemory::Memzero(BufferForSend.GetData(), BufferForSend.Num());
	FMemory::Memcpy(BufferForSend.GetData(), BodyBuffer.GetData(), FMath::Min(BodyBuffer.Num(), FixedSize));

	DispatchMadeSendBuffer(BufferForSend, DataType);
}

void UPacketRuleFixedLength::NotifyReceiveData(const TArray<uint8>& DataBuffer)
{
	DispatchMadeReceiveBuffer(DataBuffer);
}

int32 UPacketRuleFixedLength::GetWantSize()
{
	return FixedSize;
}

UPacketRule* UPacketRuleFixedLength::Clone()
{
	return UPacketRuleFactory::CreatePacketRuleFixedLength(FixedSize);
}