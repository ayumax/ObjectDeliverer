// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "DeliveryBox.h"
#include "Utf8StringDeliveryBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCNUtf8StringDeliveryBoxReceived, const FString&, ReceivedString, const UObjectDelivererProtocol*, FromObject);

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UUtf8StringDeliveryBox : public UDeliveryBox
{
	GENERATED_BODY()

public:
	UUtf8StringDeliveryBox();
	~UUtf8StringDeliveryBox();

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|DeliveryBox")
	void Send(const FString& message);

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|DeliveryBox")
	void SendTo(const FString& message, const UObjectDelivererProtocol* Destination);

	virtual void NotifyReceiveBuffer(const UObjectDelivererProtocol* FromObject, const TArray<uint8>& buffer) override;

	UPROPERTY(BlueprintAssignable, Category = "ObjectDeliverer|DeliveryBox")
	FCNUtf8StringDeliveryBoxReceived Received;
};
