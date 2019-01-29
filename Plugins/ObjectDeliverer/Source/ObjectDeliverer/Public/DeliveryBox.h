#pragma once

#include "CoreMinimal.h"
#include "DeliveryBox.generated.h"

DECLARE_DELEGATE_OneParam(FUDeliveryBoxRequestSend, const TArray<uint8>&);

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UDeliveryBox : public UObject
{
	GENERATED_BODY()

public:
	UDeliveryBox();
	~UDeliveryBox();

	virtual void NotifyReceiveBuffer(const TArray<uint8>& buffer);

	FUDeliveryBoxRequestSend RequestSend;

};
