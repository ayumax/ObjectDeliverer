#pragma once

#include "CoreMinimal.h"
#include "CNPacketRule.h"
#include "CNPacketRuleTerminate.generated.h"


UCLASS(BlueprintType)
class COMMNET_API UCNPacketRuleTerminate : public UCNPacketRule
{
	GENERATED_BODY()

public:
	UCNPacketRuleTerminate();
	~UCNPacketRuleTerminate();

	virtual void Initialize_Implementation() override;
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer, TArray<uint8>& SendBuffer) override;
	virtual bool NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer) override;
	virtual int32 GetWantSize_Implementation() override;


};
