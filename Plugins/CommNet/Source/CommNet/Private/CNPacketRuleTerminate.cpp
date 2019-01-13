#include "CNPacketRuleTerminate.h"
#include "CommNetProtocol.h"

UCNPacketRuleTerminate::UCNPacketRuleTerminate()
{

}

UCNPacketRuleTerminate::~UCNPacketRuleTerminate()
{
}

void UCNPacketRuleTerminate::Initialize_Implementation()
{
}

void UCNPacketRuleTerminate::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer, TArray<uint8>& SendBuffer)
{
}

bool UCNPacketRuleTerminate::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer)
{
	return true;
}

int32 UCNPacketRuleTerminate::GetWantSize_Implementation()
{
	return 0;
}