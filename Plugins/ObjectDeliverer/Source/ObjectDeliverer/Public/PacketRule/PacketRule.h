// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Utils/ODGrowBuffer.h"
#include "PacketRule.generated.h"

UENUM(BlueprintType)
enum class ECNBufferEndian : uint8
{
	/** Big Endian */
	Big = 0,
	/** Little Endian */
	Little
};

DECLARE_DELEGATE_OneParam(FCNPacketRuleMadeSendBuffer, const TArray<uint8>&);
DECLARE_DELEGATE_OneParam(FCNPacketRuleMadeReceiveBuffer, const TArray<uint8>&);

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UPacketRule : public UObject
{
	GENERATED_BODY()

public:
	UPacketRule();
	~UPacketRule();

	virtual void Initialize();
	virtual void MakeSendPacket(const TArray<uint8>& BodyBuffer);
	virtual void NotifyReceiveData(const TArray<uint8>& DataBuffer);
	virtual int32 GetWantSize();
	virtual UPacketRule* Clone();
	
	FCNPacketRuleMadeSendBuffer MadeSendBuffer;
	FCNPacketRuleMadeReceiveBuffer MadeReceiveBuffer;

protected:
	void DispatchMadeSendBuffer(const TArray<uint8>& SendBuffer);
	void DispatchMadeReceiveBuffer(const TArray<uint8>& ReceiveBuffer);
};
