#pragma once

#include "CoreMinimal.h"
#include "CNPacketRule.h"
#include "CNPacketRuleFactory.generated.h"


UCLASS(BlueprintType)
class COMMNET_API UCNPacketRuleFactory : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "CommNet")
	static class UCNPacketRuleFixedLength* CreatePacketRuleFixedLength(int32 FixedSize = 128);

	UFUNCTION(BlueprintPure, Category = "CommNet")
	static class UCNPacketRuleSizeBody* CreateCNPacketRuleSizeBody(int32 SizeLength = 4, ECNBufferEndian SizeBufferEndian = ECNBufferEndian::Big);

	UFUNCTION(BlueprintPure, Category = "CommNet")
	static class UCNPacketRuleTerminate* CreateCNPacketRuleTerminate(const TArray<uint8>& Terminate);

	UFUNCTION(BlueprintPure, Category = "CommNet")
	static class UCNPacketRuleNodivision* CreateCNPacketRuleNodivision();
};
