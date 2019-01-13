#include "CNPacketRuleFactory.h"
#include "CNPacketRuleFixedLength.h"
#include "CNPacketRuleSizeBody.h"
#include "CNPacketRuleTerminate.h"

UCNPacketRuleFixedLength* UCNPacketRuleFactory::CreatePacketRuleFixedLength(int32 FixedSize)
{
	auto PacketRule = NewObject<UCNPacketRuleFixedLength>();
	PacketRule->FixedSize = FixedSize;
	return PacketRule;
}

UCNPacketRuleSizeBody* UCNPacketRuleFactory::CreateCNPacketRuleSizeBody(int32 SizeLength, ECNBufferEndian SizeBufferEndian)
{
	auto PacketRule = NewObject<UCNPacketRuleSizeBody>();
	PacketRule->SizeLength = SizeLength;
	PacketRule->SizeBufferEndian = SizeBufferEndian;
	return PacketRule;
}

UCNPacketRuleTerminate* UCNPacketRuleFactory::CreateCNPacketRuleTerminate(const TArray<uint8>& Terminate)
{
	auto PacketRule = NewObject<UCNPacketRuleTerminate>();
	PacketRule->Terminate = Terminate;
	return PacketRule;
}