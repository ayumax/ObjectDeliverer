#pragma once

#include "CoreMinimal.h"
#include "PacketRule.h"
#include "PacketRuleTerminate.generated.h"


UCLASS(BlueprintType)
class OBJECTDELIVERER_API UPacketRuleTerminate : public UPacketRule
{
	GENERATED_BODY()

public:
	UPacketRuleTerminate();
	~UPacketRuleTerminate();

	virtual void Initialize_Implementation() override;
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer) override;
	virtual void NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer) override;
	virtual int32 GetWantSize_Implementation() override;
	virtual UPacketRule* Clone_Implementation() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|PacketRule")
		TArray<uint8> Terminate;

private:
	TArray<uint8> BufferForSend;
	TArray<uint8> ReceiveTempBuffer;
	TArray<uint8> BufferForReceive;

};
