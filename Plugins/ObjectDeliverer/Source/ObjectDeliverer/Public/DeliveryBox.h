#pragma once

#include "CoreMinimal.h"
#include "DeliveryBox.generated.h"

DECLARE_DELEGATE_OneParam(FUDeliveryBoxRequestSend, const TArray<uint8>&);

UCLASS(BlueprintType)
class OBJECTDELIVERER_API UDeliveryBox : public UObject
{
	GENERATED_BODY()

public:
	UDeliveryBox();
	~UDeliveryBox();

	UFUNCTION(BlueprintNativeEvent, Category = "ObjectDeliverer|DeliveryBox")
	void NotifyReceiveBuffer(const TArray<uint8>& buffer);
	virtual void NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer);

	FUDeliveryBoxRequestSend RequestSend;

};
