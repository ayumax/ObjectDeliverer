#pragma once

#include "CoreMinimal.h"
#include "DeliveryBox.h"
#include "Utf8StringDeliveryBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCNUtf8StringDeliveryBoxReceived, FString, ReceivedString);

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UUtf8StringDeliveryBox : public UDeliveryBox
{
	GENERATED_BODY()

public:
	UUtf8StringDeliveryBox();
	~UUtf8StringDeliveryBox();

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|DeliveryBox")
	void Send(const FString& message);


	virtual void NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer) override;

	UPROPERTY(BlueprintAssignable, Category = "ObjectDeliverer|DeliveryBox")
	FCNUtf8StringDeliveryBoxReceived Received;
};
