#pragma once

#include "CoreMinimal.h"
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
	 * create delivery box (utf-8 string)
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|DeliveryBox")
	static class UUtf8StringDeliveryBox* CreateUtf8StringDeliveryBox();

};

