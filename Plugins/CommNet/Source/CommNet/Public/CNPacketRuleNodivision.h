#pragma once

#include "CoreMinimal.h"
#include "CNPacketRule.h"
#include "CNPacketRuleNodivision.generated.h"


UCLASS(BlueprintType)
class COMMNET_API UCNPacketRuleNodivision : public UCNPacketRule
{
	GENERATED_BODY()

public:
	UCNPacketRuleNodivision();
	~UCNPacketRuleNodivision();

	virtual void Initialize_Implementation() override;
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer) override;
	virtual void NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer) override;
	virtual int32 GetWantSize_Implementation() override;
	virtual UCNPacketRule* Clone_Implementation() override;

};
