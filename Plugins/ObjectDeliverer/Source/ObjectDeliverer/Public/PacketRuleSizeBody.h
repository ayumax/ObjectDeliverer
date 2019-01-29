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

	virtual void Initialize_Implementation() override;
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer) override;
	virtual void NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer) override;
	virtual int32 GetWantSize_Implementation() override;
	virtual UPacketRule* Clone_Implementation() override;

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
