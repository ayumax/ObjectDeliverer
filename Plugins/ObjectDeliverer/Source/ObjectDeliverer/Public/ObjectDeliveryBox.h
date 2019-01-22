#pragma once

#include "CoreMinimal.h"
#include "DeliveryBox.h"
#include "ObjectDeliveryBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCNObjectDeliveryBoxReceived, const UObject*, ReceivedObject);

UCLASS(BlueprintType)
class OBJECTDELIVERER_API UObjectDeliveryBox : public UDeliveryBox
{
	GENERATED_BODY()

public:
	UObjectDeliveryBox();
	~UObjectDeliveryBox();

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|DeliveryBox")
	void Initialize(UClass* TargetClass);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ObjectDeliverer|DeliveryBox")
	void Send(const UObject* message);
	virtual void Send_Implementation(const UObject* message);


	virtual void NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer) override;

	UPROPERTY(BlueprintAssignable, Category = "ObjectDeliverer|DeliveryBox")
	FCNObjectDeliveryBoxReceived Received;

protected:
	UPROPERTY()
	UClass* TargetClass;
};
