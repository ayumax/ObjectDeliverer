#pragma once

#include "CoreMinimal.h"
#include "PacketRule.h"
#include "PacketRuleFactory.generated.h"


UCLASS(BlueprintType)
class OBJECTDELIVERER_API UPacketRuleFactory : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|PacketRule")
	static class UPacketRuleFixedLength* CreatePacketRuleFixedLength(int32 FixedSize = 128);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|PacketRule")
	static class UPacketRuleSizeBody* CreatePacketRuleSizeBody(int32 SizeLength = 4, ECNBufferEndian SizeBufferEndian = ECNBufferEndian::Big);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|PacketRule")
	static class UPacketRuleTerminate* CreatePacketRuleTerminate(const TArray<uint8>& Terminate);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|PacketRule")
	static class UPacketRuleNodivision* CreatePacketRuleNodivision();
};
