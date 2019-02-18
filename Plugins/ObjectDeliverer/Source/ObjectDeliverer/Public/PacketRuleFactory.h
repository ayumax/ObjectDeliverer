// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "PacketRule.h"
#include "PacketRuleFactory.generated.h"


UCLASS(BlueprintType)
class OBJECTDELIVERER_API UPacketRuleFactory : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * create packet rule (fixed length)
	 * @param FixedSize - Fixed size buffer length
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|PacketRule")
	static class UPacketRuleFixedLength* CreatePacketRuleFixedLength(int32 FixedSize = 128);

	/**
	 * create packet rule (A header indicating the size is followed by a body part)
	 * @param SizeLength - size header length(byte)
	 * @param SizeBufferEndian - Endian of size header
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|PacketRule")
	static class UPacketRuleSizeBody* CreatePacketRuleSizeBody(int32 SizeLength = 4, ECNBufferEndian SizeBufferEndian = ECNBufferEndian::Big);

	/**
	 * create packet rule (Separate by terminal symbol)
	 * @param Terminate - terminate values
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|PacketRule")
	static class UPacketRuleTerminate* CreatePacketRuleTerminate(const TArray<uint8>& Terminate);

	/**
	 * create packet rule (no division)
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|PacketRule")
	static class UPacketRuleNodivision* CreatePacketRuleNodivision();
};
