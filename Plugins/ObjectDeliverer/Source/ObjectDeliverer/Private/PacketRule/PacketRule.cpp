#include "PacketRule.h"

UPacketRule::UPacketRule()
{

}

UPacketRule::~UPacketRule()
{
}

void UPacketRule::Initialize_Implementation()
{

}

void UPacketRule::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer)
{
}

void UPacketRule::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer)
{
}

int32 UPacketRule::GetWantSize_Implementation()
{
	return 0;
}

UPacketRule* UPacketRule::Clone_Implementation()
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