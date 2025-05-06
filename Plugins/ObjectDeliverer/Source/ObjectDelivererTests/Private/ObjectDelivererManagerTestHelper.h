// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "DeliveryBox/IODConvertPropertyName.h"
#include "ObjectDelivererManagerTestHelper.generated.h"

class UObjectDelivererProtocol;

UCLASS()
class OBJECTDELIVERERTESTS_API UObjectDelivererManagerTestHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnConnect(const UObjectDelivererProtocol *ClientSocket);
	UFUNCTION()
	void OnDisConnect(const UObjectDelivererProtocol *ClientSocket);
	UFUNCTION()
	void OnReceive(const UObjectDelivererProtocol *ClientSocket, const TArray<uint8> &Buffer);
	UFUNCTION()
	void OnReceiveString(const FString &StringValue, const UObjectDelivererProtocol *FromObject);

	UPROPERTY()
	TArray<const UObjectDelivererProtocol *> ConnectedSocket;

	UPROPERTY()
	TArray<const UObjectDelivererProtocol *> DisconnectedSocket;

	UPROPERTY()
	const UObjectDelivererProtocol *ReceiveSocket;

	TArray<TArray<uint8>> ReceiveBuffers;
	TArray<FString> ReceiveStrings;
};

UCLASS()
class OBJECTDELIVERERTESTS_API UJsonSerializerTestArrayElementObject1 : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 IntProperty;
};

UCLASS()
class OBJECTDELIVERERTESTS_API UJsonSerializerTestArrayElementObject2 : public UJsonSerializerTestArrayElementObject1
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 IntProperty2;
};

UCLASS()
class OBJECTDELIVERERTESTS_API UJsonSerializerTestObject : public UObject
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
	TArray<UJsonSerializerTestArrayElementObject1 *> ArrayProperty;
};

UCLASS()
class OBJECTDELIVERERTESTS_API UJsonSerializeConvertNameTestObject : public UObject, public IODConvertPropertyName
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 IntProperty;

	UPROPERTY()
	float FloatProperty;

	UPROPERTY()
	uint8 Uint8Property;

	FString ConvertFPropertyName_Implementation(const FName &PropertyName) const override
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

UCLASS()
class OBJECTDELIVERERTESTS_API UJsonSerializerNestedObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	int32 Value;
};

UCLASS()
class OBJECTDELIVERERTESTS_API UJsonSerializerComplexObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FString, int32> MapProperty;

	UPROPERTY()
	TSet<FString> SetProperty;

	UPROPERTY()
	UJsonSerializerNestedObject *NestedObject;

	UPROPERTY()
	TArray<UJsonSerializerNestedObject *> NestedObjectArray;
};

// Class for testing inheritance relationships
UCLASS()
class OBJECTDELIVERERTESTS_API UJsonSerializerBaseClass : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 BaseValue;

	UPROPERTY()
	FString BaseName;
};

UCLASS()
class OBJECTDELIVERERTESTS_API UJsonSerializerDerivedClass : public UJsonSerializerBaseClass
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float DerivedValue;

	UPROPERTY()
	bool DerivedFlag;
};

// Class for testing UE4 basic types
UCLASS()
class OBJECTDELIVERERTESTS_API UJsonSerializerUE4TypesObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTransform TransformProperty;

	UPROPERTY()
	FColor ColorProperty;

	UPROPERTY()
	FDateTime DateTimeProperty;

	UPROPERTY()
	FGuid GuidProperty;
};

// Class for objects with circular references
UCLASS()
class OBJECTDELIVERERTESTS_API UJsonSerializerCircularObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	UJsonSerializerCircularObject *Reference;
};

UCLASS()
class UUtf8StringDeliveryBoxTestHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnReceiveString(const FString &StringValue, const UObjectDelivererProtocol *FromObject)
	{
		ReceivedStrings.Add(StringValue);
	}

	TArray<FString> ReceivedStrings;
};

UCLASS()
class UObjectDeliveryBoxUsingJsonTestHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnReceiveObject(UObject* ReceivedObject, const FString& ReceivedJsonString, const UObjectDelivererProtocol* FromObject)
	{
		ReceivedObjects.Add(ReceivedObject);
	}

	TArray<UObject*> ReceivedObjects;
};