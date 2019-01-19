#include "PacketRuleNodivision.h"
#include "ObjectDelivererProtocol.h"
#include "PacketRuleFactory.h"

UPacketRuleNodivision::UPacketRuleNodivision()
{
}

UPacketRuleNodivision::~UPacketRuleNodivision()
{
}

void UPacketRuleNodivision::Initialize_Implementation()
{
}

void UPacketRuleNodivision::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer)
{
	DispatchMadeSendBuffer(BodyBuffer);
}

void UPacketRuleNodivision::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer)
{
	DispatchMadeReceiveBuffer(DataBuffer);
}

int32 UPacketRuleNodivision::GetWantSize_Implementation()
{
	return 0;
}

UPacketRule* UPacketRuleNodivision::Clone_Implementation()
{
	return UPacketRuleFactory::CreatePacketRuleNodivision();
}