#pragma once

#include "CoreMinimal.h"
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

UCLASS(BlueprintType)
class OBJECTDELIVERER_API UPacketRule : public UObject
{
	GENERATED_BODY()

public:
	UPacketRule();
	~UPacketRule();

	UFUNCTION(BlueprintNativeEvent, Category = "ObjectDeliverer|PacketRule")
	void Initialize();
	virtual void Initialize_Implementation();


	UFUNCTION(BlueprintNativeEvent, Category = "ObjectDeliverer|PacketRule")
	void MakeSendPacket(const TArray<uint8>& BodyBuffer);
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer);

	UFUNCTION(BlueprintNativeEvent, Category = "ObjectDeliverer|PacketRule")
	void NotifyReceiveData(const TArray<uint8>& DataBuffer);
	virtual void NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer);

	UFUNCTION(BlueprintNativeEvent, Category = "ObjectDeliverer|PacketRule")
	int32 GetWantSize();
	virtual int32 GetWantSize_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "ObjectDeliverer|PacketRule")
	UPacketRule* Clone();
	virtual UPacketRule* Clone_Implementation();
	
	FCNPacketRuleMadeSendBuffer MadeSendBuffer;
	FCNPacketRuleMadeReceiveBuffer MadeReceiveBuffer;

protected:
	void DispatchMadeSendBuffer(const TArray<uint8>& SendBuffer);
	void DispatchMadeReceiveBuffer(const TArray<uint8>& ReceiveBuffer);
};
