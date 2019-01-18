#pragma once

#include "CoreMinimal.h"
#include "CNDeliveryBox.h"
#include "CNObjectDeliveryBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCNObjectDeliveryBoxReceived, const UObject*, ReceivedObject);

UCLASS(BlueprintType)
class COMMNET_API UCNObjectDeliveryBox : public UCNDeliveryBox
{
	GENERATED_BODY()

public:
	UCNObjectDeliveryBox();
	~UCNObjectDeliveryBox();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void Initialize(UClass* TargetClass);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CommNet")
	void Send(const UObject* message);
	virtual void Send_Implementation(const UObject* message);


	virtual void NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer) override;

	UPROPERTY(BlueprintAssignable, Category = "CommNet")
	FCNObjectDeliveryBoxReceived Received;

protected:
	UPROPERTY()
	UObject* DeliveryObject;

	UPROPERTY()
	TMap<FString, UProperty*> PropertyMap;
};
