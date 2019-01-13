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

void UCNPacketRule::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer, TArray<uint8>& SendBuffer)
{
}

bool UCNPacketRule::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer)
{
	return false;
}

int32 UCNPacketRule::GetWantSize_Implementation()
{
	return 0;
}