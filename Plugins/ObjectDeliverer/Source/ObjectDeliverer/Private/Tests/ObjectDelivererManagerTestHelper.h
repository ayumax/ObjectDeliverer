// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "PacketRule/PacketRuleFactory.h"
#include "Protocol/ProtocolFactory.h"
#include "DeliveryBox/DeliveryBoxFactory.h"
#include "UObject/GCObject.h"
#include "DeliveryBox/IODConvertPropertyName.h"
#include "ObjectDelivererManagerTestHelper.generated.h"

class UObjectDelivererProtocol;

UCLASS()
class OBJECTDELIVERER_API UObjectDelivererManagerTestHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnConnect(const UObjectDelivererProtocol* ClientSocket);
	UFUNCTION()
	void OnDisConnect(const UObjectDelivererProtocol* ClientSocket);
	UFUNCTION()
	void OnReceive(const UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer);
	UFUNCTION()
	void OnReceiveString(const FString& StringValue, const UObjectDelivererProtocol* FromObject);

	UPROPERTY()
	TArray<const UObjectDelivererProtocol*> ConnectedSocket;

	UPROPERTY()
	TArray<const UObjectDelivererProtocol*> DisconnectedSocket;

	UPROPERTY()
	const UObjectDelivererProtocol* ReceiveSocket;

	TArray<TArray<uint8>> ReceiveBuffers;
	TArray<FString> ReceiveStrings;
};

UCLASS()
class OBJECTDELIVERER_API UJsonSerializerTestArrayElementObject1 : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 IntProperty;

};

UCLASS()
class OBJECTDELIVERER_API UJsonSerializerTestArrayElementObject2 : public UJsonSerializerTestArrayElementObject1
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 IntProperty2;
};

UCLASS()
class OBJECTDELIVERER_API UJsonSerializerTestObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 IntProperty;

	UPROPERTY()
	float FloatProperty;

	UPROPERTY()
	uint8 Uint8Property;

	UPROPERTY()
	bool BoolProperty;

	UPROPERTY()
	FString StringProperty;

	UPROPERTY()
	FVector VectorProperty;

	UPROPERTY()
	FRotator RotateProperty;

	UPROPERTY()
	TArray<UJsonSerializerTestArrayElementObject1*> ArrayProperty;
};

UCLASS()
class OBJECTDELIVERER_API UJsonSerializeConvertNameTestObject : public UObject, public IODConvertPropertyName
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 IntProperty;

	UPROPERTY()
	float FloatProperty;

	UPROPERTY()
	uint8 Uint8Property;

	FString ConvertFPropertyName_Implementation(const FName& PropertyName) const override
	{
		if (PropertyName == FName(TEXT("IntProperty")))
		{
			return TEXT("IntProperty_Converted");
		}
		else if (PropertyName == FName(TEXT("FloatProperty")))
		{
			return TEXT("FloatProperty_Converted");
		}
		else if (PropertyName == FName(TEXT("Uint8Property")))
		{
			return TEXT("Uint8Property_Converted");
		}

		return TEXT("");
	}

};