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
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer) override;
	virtual void NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer) override;
	virtual int32 GetWantSize_Implementation() override;
	virtual UCNPacketRule* Clone_Implementation() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
		TArray<uint8> Terminate;

private:
	TArray<uint8> BufferForSend;
	TArray<uint8> ReceiveTempBuffer;
	TArray<uint8> BufferForReceive;

};
