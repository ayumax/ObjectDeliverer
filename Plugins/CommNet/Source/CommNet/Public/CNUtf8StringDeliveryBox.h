#pragma once

#include "CoreMinimal.h"
#include "CNDeliveryBox.h"
#include "CNUtf8StringDeliveryBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCNUtf8StringDeliveryBoxReceived, FString, ReceivedString);

UCLASS(BlueprintType)
class COMMNET_API UCNUtf8StringDeliveryBox : public UCNDeliveryBox
{
	GENERATED_BODY()

public:
	UCNUtf8StringDeliveryBox();
	~UCNUtf8StringDeliveryBox();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void Send(const FString& message);


	virtual void NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer) override;

	UPROPERTY(BlueprintAssignable, Category = "CommNet")
	FCNUtf8StringDeliveryBoxReceived Received;
};
