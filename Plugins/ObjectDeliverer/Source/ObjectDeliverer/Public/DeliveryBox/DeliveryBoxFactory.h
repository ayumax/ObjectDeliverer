// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "DeliveryBox/ODOverrideJsonSerializer.h"
#include "DeliveryBoxFactory.generated.h"

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UDeliveryBoxFactory : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * create delivery box (object with json serializer)
	 * @param TargetClass - target object class
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|DeliveryBox")
	static class UObjectDeliveryBoxUsingJson* CreateObjectDeliveryBoxUsingJson(UClass* TargetClass);

	/**
	 * create delivery box (object with json serializer)
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|DeliveryBox")
	static class UObjectDeliveryBoxUsingJson* CreateDynamicObjectDeliveryBoxUsingJson();

	/**
	 * create delivery box (object with json serializer)
	 * @param DefaultSerializerType -  default serialization method
	 * @param ObjectSerializerTypes - Set the object class and serialization method class of the property as a pair
	 * @param TargetClass - target object class
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|DeliveryBox")
	static class UObjectDeliveryBoxUsingJson* CreateCustomObjectDeliveryBoxUsingJson(EODJsonSerializeType DefaultSerializerType, const TMap<UClass*, EODJsonSerializeType>& ObjectSerializerTypes, UClass* TargetClass);

	/**
	 * create delivery box (utf-8 string)
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|DeliveryBox")
	static class UUtf8StringDeliveryBox* CreateUtf8StringDeliveryBox();

};

