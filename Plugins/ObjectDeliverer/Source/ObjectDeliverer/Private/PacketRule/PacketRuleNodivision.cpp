// Copyright 2019 ayumax. All Rights Reserved.
#include "PacketRuleNodivision.h"
#include "PacketRuleFactory.h"

UPacketRuleNodivision::UPacketRuleNodivision()
{
}

UPacketRuleNodivision::~UPacketRuleNodivision()
{
}

void UPacketRuleNodivision::Initialize()
{
}

void UPacketRuleNodivision::MakeSendPacket(const TArray<uint8>& BodyBuffer)
{
	DispatchMadeSendBuffer(BodyBuffer);
}

void UPacketRuleNodivision::NotifyReceiveData(const TArray<uint8>& DataBuffer)
{
	DispatchMadeReceiveBuffer(DataBuffer);
}

int32 UPacketRuleNodivision::GetWantSize()
{
	return 0;
}

UPacketRule* UPacketRuleNodivision::Clone()
{
	return UPacketRuleFactory::CreatePacketRuleNodivision();
}