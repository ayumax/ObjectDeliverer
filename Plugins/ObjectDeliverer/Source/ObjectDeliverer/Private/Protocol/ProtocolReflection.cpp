// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolReflection.h"
#include "PacketRule/PacketRule.h"


UProtocolReflection::UProtocolReflection()
{

}

UProtocolReflection::~UProtocolReflection()
{

}

void UProtocolReflection::Send(const TArray<uint8>& DataBuffer) const
{
	PacketRule->MakeSendPacket(DataBuffer);
}

void UProtocolReflection::RequestSend(const TArray<uint8>& DataBuffer)
{
	DispatchReceiveData(this, DataBuffer);
}
