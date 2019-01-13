#pragma once

#include "CoreMinimal.h"
#include "CNPacketRule.h"
#include "CNPacketRuleSizeBody.generated.h"



UCLASS(BlueprintType)
class COMMNET_API UCNPacketRuleSizeBody : public UCNPacketRule
{
	GENERATED_BODY()

public:
	UCNPacketRuleSizeBody();
	~UCNPacketRuleSizeBody();

	virtual void Initialize_Implementation() override;
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer, TArray<uint8>& SendBuffer) override;
	virtual bool NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer) override;
	virtual int32 GetWantSize_Implementation() override;

private:
	void OnReceivedSize(const TArray<uint8>& DataBuffer);
	void OnReceivedBody(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer);

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	int32 SizeLength = 4;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	ECNBufferEndian SizeBufferEndian = ECNBufferEndian::Big;
};
