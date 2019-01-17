#include "CNPacketRule.h"

UCNPacketRule::UCNPacketRule()
{

}

UCNPacketRule::~UCNPacketRule()
{
}

void UCNPacketRule::Initialize_Implementation()
{

}

void UCNPacketRule::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer)
{
}

void UCNPacketRule::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer)
{
}

int32 UCNPacketRule::GetWantSize_Implementation()
{
	return 0;
}

UCNPacketRule* UCNPacketRule::Clone_Implementation()
{
	return nullptr;
}

void UCNPacketRule::DispatchMadeSendBuffer(const TArray<uint8>& SendBuffer)
{
	MadeSendBuffer.ExecuteIfBound(SendBuffer);
}

void UCNPacketRule::DispatchMadeReceiveBuffer(const TArray<uint8>& ReceiveBuffer)
{
	MadeReceiveBuffer.ExecuteIfBound(ReceiveBuffer);
}