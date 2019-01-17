#pragma once

#include "CoreMinimal.h"
#include "CNDeliveryBox.generated.h"

DECLARE_DELEGATE_OneParam(FUCNDeliveryBoxRequestSend, const TArray<uint8>&);

UCLASS(BlueprintType)
class COMMNET_API UCNDeliveryBox : public UObject
{
	GENERATED_BODY()

public:
	UCNDeliveryBox();
	~UCNDeliveryBox();

	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	void NotifyReceiveBuffer(const TArray<uint8>& buffer);
	virtual void NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer);

	FUCNDeliveryBoxRequestSend RequestSend;

};
