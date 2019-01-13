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
	void MakeSendPacket(const TArray<uint8>& BodyBuffer, TArray<uint8>& SendBuffer);
	virtual void MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer, TArray<uint8>& SendBuffer);

	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	bool NotifyReceiveData(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer);
	virtual bool NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer);

	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	int32 GetWantSize();
	virtual int32 GetWantSize_Implementation();


};
