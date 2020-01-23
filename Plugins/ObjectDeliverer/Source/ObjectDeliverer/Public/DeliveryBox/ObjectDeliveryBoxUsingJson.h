// Copyright 2019 ayumax. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "DeliveryBox/ODOverrideJsonSerializer.h"
#include "DeliveryBox.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "ObjectDeliveryBoxUsingJson.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCNObjectDeliveryBoxReceived, UObject*, ReceivedObject, const FString&, ReceivedJsonString, const UObjectDelivererProtocol*, FromObject);

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UObjectDeliveryBoxUsingJson : public UDeliveryBox
{
	GENERATED_BODY()

public:
	UObjectDeliveryBoxUsingJson();
	~UObjectDeliveryBoxUsingJson();

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|DeliveryBox")
	void Initialize(UClass* TargetClass);

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|DeliveryBox")
	void InitializeCustom(EODJsonSerializeType DefaultSerializerType, const TMap<UClass*, EODJsonSerializeType>& ObjectSerializerTypes, UClass* TargetClass);


	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|DeliveryBox")
	virtual void Send(const UObject* message, FString& makedJson);

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|DeliveryBox")
	virtual void SendTo(const UObject* message, const UObjectDelivererProtocol* Destination, FString& makedJson);

	virtual void NotifyReceiveBuffer(const UObjectDelivererProtocol* FromObject, const TArray<uint8>& buffer) override;



	UPROPERTY(BlueprintAssignable, Category = "ObjectDeliverer|DeliveryBox")
	FCNObjectDeliveryBoxReceived Received;

protected:
	UPROPERTY()
	UClass* TargetClass;

	UPROPERTY()
	class UODJsonSerializer* Serializer;
	UPROPERTY()
	class UODJsonDeserializer* Deserializer;
};
