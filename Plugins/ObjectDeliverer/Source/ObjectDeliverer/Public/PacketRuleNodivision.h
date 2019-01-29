#pragma once

#include "CoreMinimal.h"
#include "PacketRule.h"
#include "PacketRuleNodivision.generated.h"


UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UPacketRuleNodivision : public UPacketRule
{
	GENERATED_BODY()

public:
	UPacketRuleNodivision();
	~UPacketRuleNodivision();

	virtual void Initialize_Implementation() override;
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer) override;
	virtual void NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer) override;
	virtual int32 GetWantSize_Implementation() override;
	virtual UPacketRule* Clone_Implementation() override;

};
