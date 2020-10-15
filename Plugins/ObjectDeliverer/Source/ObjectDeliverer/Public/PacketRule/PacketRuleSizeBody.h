// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "PacketRule.h"
#include "PacketRuleSizeBody.generated.h"



UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UPacketRuleSizeBody : public UPacketRule
{
	GENERATED_BODY()

public:
	UPacketRuleSizeBody();
	~UPacketRuleSizeBody();

	virtual void Initialize() override;
	virtual void MakeSendPacket(const TArray<uint8>& BodyBuffer) override;
	virtual void NotifyReceiveData(const TArray<uint8>& DataBuffer) override;
	virtual int32 GetWantSize() override;
	virtual UPacketRule* Clone() override;

private:
	void OnReceivedSize(const TArray<uint8>& DataBuffer);
	void OnReceivedBody(const TArray<uint8>& DataBuffer);

private:
	enum EReceiveMode
	{
		Size,
		Body
	} ReceiveMode;

	uint32 BodySize;
	int32 ReceiveBufferPosition;

	UPROPERTY(Transient)
	TArray<uint8> BufferForSend;

public:

	/**
	* size buffer length(1 - 4)
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|PacketRule")
	int32 SizeLength = 4;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|PacketRule")
	ECNBufferEndian SizeBufferEndian = ECNBufferEndian::Big;
};
