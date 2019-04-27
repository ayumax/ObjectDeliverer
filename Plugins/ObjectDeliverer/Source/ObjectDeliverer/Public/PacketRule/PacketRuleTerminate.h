// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "PacketRule.h"
#include "PacketRuleTerminate.generated.h"


UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UPacketRuleTerminate : public UPacketRule
{
	GENERATED_BODY()

public:
	UPacketRuleTerminate();
	~UPacketRuleTerminate();

	virtual void Initialize() override;
	virtual void MakeSendPacket(const TArray<uint8>& BodyBuffer) override;
	virtual void NotifyReceiveData(const TArray<uint8>& DataBuffer) override;
	virtual int32 GetWantSize() override;
	virtual UPacketRule* Clone() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|PacketRule")
	TArray<uint8> Terminate;

private:
	UPROPERTY(Transient)
	TArray<uint8> BufferForSend;
	UPROPERTY(Transient)
	TArray<uint8> ReceiveTempBuffer;
	UPROPERTY(Transient)
	TArray<uint8> BufferForReceive;

};
