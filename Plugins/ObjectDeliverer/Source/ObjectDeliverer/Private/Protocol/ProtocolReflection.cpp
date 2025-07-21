// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolReflection.h"
#include "PacketRule/PacketRule.h"


UProtocolReflection::UProtocolReflection()
{

}

UProtocolReflection::~UProtocolReflection()
{

}

void UProtocolReflection::Send(const TArray<uint8>& DataBuffer, const FDeliveryDataType& KindOfData) const
{
	PacketRule->MakeSendPacket(DataBuffer, KindOfData);
}

void UProtocolReflection::RequestSend(const TArray<uint8>& DataBuffer, const FDeliveryDataType& DataType)
{
	DispatchReceiveData(this, DataBuffer);
}
