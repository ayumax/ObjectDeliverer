#include "CNPacketRuleFixedLength.h"
#include "CommNetProtocol.h"

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

void UCNPacketRuleFixedLength::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer, TArray<uint8>& SendBuffer)
{
	FMemory::Memzero(BufferForSend);
	FMemory::Memcpy(BufferForSend.GetData(), BodyBuffer.GetData(), FMath::Min(BodyBuffer.Num(), FixedSize));
}

bool UCNPacketRuleFixedLength::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer)
{
	BodyBuffer = DataBuffer;
	return true;
}

int32 UCNPacketRuleFixedLength::GetWantSize_Implementation()
{
	return FixedSize;
}