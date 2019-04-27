// Copyright 2019 ayumax. All Rights Reserved.
#include "PacketRule/PacketRuleFactory.h"
#include "PacketRule/PacketRuleFixedLength.h"
#include "PacketRule/PacketRuleSizeBody.h"
#include "PacketRule/PacketRuleTerminate.h"
#include "PacketRule/PacketRuleNodivision.h"

UPacketRuleFixedLength* UPacketRuleFactory::CreatePacketRuleFixedLength(int32 FixedSize)
{
	auto PacketRule = NewObject<UPacketRuleFixedLength>();
	PacketRule->FixedSize = FixedSize;
	return PacketRule;
}

UPacketRuleSizeBody* UPacketRuleFactory::CreatePacketRuleSizeBody(int32 SizeLength, ECNBufferEndian SizeBufferEndian)
{
	auto PacketRule = NewObject<UPacketRuleSizeBody>();
	PacketRule->SizeLength = SizeLength;
	PacketRule->SizeBufferEndian = SizeBufferEndian;
	return PacketRule;
}

UPacketRuleTerminate* UPacketRuleFactory::CreatePacketRuleTerminate(const TArray<uint8>& Terminate)
{
	auto PacketRule = NewObject<UPacketRuleTerminate>();

	if (Terminate.Num() > 0)
	{
		PacketRule->Terminate = Terminate;
	}
	return PacketRule;
}

UPacketRuleNodivision* UPacketRuleFactory::CreatePacketRuleNodivision()
{
	return NewObject<UPacketRuleNodivision>();
}