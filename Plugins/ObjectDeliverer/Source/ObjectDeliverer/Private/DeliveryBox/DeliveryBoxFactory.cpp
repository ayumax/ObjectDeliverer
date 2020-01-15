// Copyright 2019 ayumax. All Rights Reserved.
#include "DeliveryBox/DeliveryBoxFactory.h"
#include "DeliveryBox/ObjectDeliveryBoxUsingJson.h"
#include "DeliveryBox/Utf8StringDeliveryBox.h"
#include "Private/Utils/JsonSerializer/ODOverrideJsonSerializer.h"

UObjectDeliveryBoxUsingJson* UDeliveryBoxFactory::CreateObjectDeliveryBoxUsingJson(UClass* TargetClass)
{
	auto deliveryBox = NewObject<UObjectDeliveryBoxUsingJson>();

	deliveryBox->Initialize(TargetClass);

	return deliveryBox;
}

UObjectDeliveryBoxUsingJson* UDeliveryBoxFactory::CreateCustomObjectDeliveryBoxUsingJson(TSubclassOf<UODOverrideJsonSerializer> DefaultObjectSerializerClass, const TMap<UClass*, TSubclassOf<UODOverrideJsonSerializer>>& OverrideObjectSerializerClasses, UClass* TargetClass/* = nullptr*/)
{
	auto deliveryBox = NewObject<UObjectDeliveryBoxUsingJson>();

	deliveryBox->InitializeCustom(DefaultObjectSerializerClass, OverrideObjectSerializerClasses, TargetClass);

	return deliveryBox;
}

UUtf8StringDeliveryBox* UDeliveryBoxFactory::CreateUtf8StringDeliveryBox()
{
	return NewObject<UUtf8StringDeliveryBox>();
}