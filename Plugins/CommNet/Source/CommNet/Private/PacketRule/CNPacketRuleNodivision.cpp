#include "CNPacketRuleNodivision.h"
#include "CommNetProtocol.h"
#include "CNPacketRuleFactory.h"

UCNPacketRuleNodivision::UCNPacketRuleNodivision()
{
}

UCNPacketRuleNodivision::~UCNPacketRuleNodivision()
{
}

void UCNPacketRuleNodivision::Initialize_Implementation()
{
}

void UCNPacketRuleNodivision::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer)
{
	DispatchMadeSendBuffer(BodyBuffer);
}

void UCNPacketRuleNodivision::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer)
{
	DispatchMadeReceiveBuffer(DataBuffer);
}

int32 UCNPacketRuleNodivision::GetWantSize_Implementation()
{
	return 0;
}

UCNPacketRule* UCNPacketRuleNodivision::Clone_Implementation()
{
	return UCNPacketRuleFactory::CreateCNPacketRuleNodivision();
}