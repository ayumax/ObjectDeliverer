// Copyright 2019 ayumax. All Rights Reserved.
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

	virtual void Initialize() override;
	virtual void MakeSendPacket(const TArray<uint8>& BodyBuffer) override;
	virtual void NotifyReceiveData(const TArray<uint8>& DataBuffer) override;
	virtual int32 GetWantSize() override;
	virtual UPacketRule* Clone() override;

};
