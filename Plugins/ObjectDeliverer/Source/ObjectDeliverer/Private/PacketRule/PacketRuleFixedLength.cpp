#include "PacketRuleFixedLength.h"
#include "ObjectDelivererProtocol.h"
#include "PacketRuleFactory.h"

UPacketRuleFixedLength::UPacketRuleFixedLength()
{

}

UPacketRuleFixedLength::~UPacketRuleFixedLength()
{
}

void UPacketRuleFixedLength::Initialize_Implementation()
{
	BufferForSend.SetNum(FixedSize);
}

void UPacketRuleFixedLength::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer)
{
	FMemory::Memzero(BufferForSend.GetData(), BufferForSend.Num());
	FMemory::Memcpy(BufferForSend.GetData(), BodyBuffer.GetData(), FMath::Min(BodyBuffer.Num(), FixedSize));

	DispatchMadeSendBuffer(BufferForSend);
}

void UPacketRuleFixedLength::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer)
{
	DispatchMadeReceiveBuffer(DataBuffer);
}

int32 UPacketRuleFixedLength::GetWantSize_Implementation()
{
	return FixedSize;
}

UPacketRule* UPacketRuleFixedLength::Clone_Implementation()
{
	return UPacketRuleFactory::CreatePacketRuleFixedLength(FixedSize);
}