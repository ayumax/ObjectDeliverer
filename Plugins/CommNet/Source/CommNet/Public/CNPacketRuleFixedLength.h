#pragma once

#include "CoreMinimal.h"
#include "CNPacketRule.h"
#include "CNPacketRuleFixedLength.generated.h"


UCLASS(BlueprintType)
class COMMNET_API UCNPacketRuleFixedLength : public UCNPacketRule
{
	GENERATED_BODY()

public:
	UCNPacketRuleFixedLength();
	~UCNPacketRuleFixedLength();

	virtual void Initialize_Implementation() override;
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer, TArray<uint8>& SendBuffer) override;
	virtual bool NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer) override;
	virtual int32 GetWantSize_Implementation() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	int32 FixedSize = 128;

private:
	TArray<uint8> BufferForSend;
};
