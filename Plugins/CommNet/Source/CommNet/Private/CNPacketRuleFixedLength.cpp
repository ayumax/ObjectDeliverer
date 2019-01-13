#include "CNPacketRuleFixedLength.h"
#include "CommNetProtocol.h"
#include "CNPacketRuleFactory.h"

UCNPacketRuleFixedLength::UCNPacketRuleFixedLength()
{

}

UCNPacketRuleFixedLength::~UCNPacketRuleFixedLength()
{
}

void UCNPacketRuleFixedLength::Initialize_Implementation()
{
	BufferForSend.SetNum(FixedSize);
}

void UCNPacketRuleFixedLength::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer)
{
	FMemory::Memzero(BufferForSend.GetData(), BufferForSend.Num());
	FMemory::Memcpy(BufferForSend.GetData(), BodyBuffer.GetData(), FMath::Min(BodyBuffer.Num(), FixedSize));

	DispatchMadeSendBuffer(BufferForSend);
}

void UCNPacketRuleFixedLength::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer)
{
	DispatchMadeReceiveBuffer(DataBuffer);
}

int32 UCNPacketRuleFixedLength::GetWantSize_Implementation()
{
	return FixedSize;
}

UCNPacketRule* UCNPacketRuleFixedLength::Clone_Implementation()
{
	return UCNPacketRuleFactory::CreatePacketRuleFixedLength(FixedSize);
}