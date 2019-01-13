#pragma once

#include "CoreMinimal.h"
#include "CNPacketRule.generated.h"

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
class COMMNET_API UCNPacketRule : public UObject
{
	GENERATED_BODY()

public:
	UCNPacketRule();
	~UCNPacketRule();

	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	void Initialize();
	virtual void Initialize_Implementation();


	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	void MakeSendPacket(const TArray<uint8>& BodyBuffer);
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer);

	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	void NotifyReceiveData(const TArray<uint8>& DataBuffer);
	virtual void NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer);

	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	int32 GetWantSize();
	virtual int32 GetWantSize_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	UCNPacketRule* Clone();
	virtual UCNPacketRule* Clone_Implementation();
	
	FCNPacketRuleMadeSendBuffer MadeSendBuffer;
	FCNPacketRuleMadeReceiveBuffer MadeReceiveBuffer;

protected:
	void DispatchMadeSendBuffer(const TArray<uint8>& SendBuffer);
	void DispatchMadeReceiveBuffer(const TArray<uint8>& ReceiveBuffer);
};
