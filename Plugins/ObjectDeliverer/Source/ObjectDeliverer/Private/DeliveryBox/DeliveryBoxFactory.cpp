// Copyright 2019 ayumax. All Rights Reserved.
#include "DeliveryBox/DeliveryBoxFactory.h"
#include "DeliveryBox/ObjectDeliveryBoxUsingJson.h"
#include "DeliveryBox/Utf8StringDeliveryBox.h"

UObjectDeliveryBoxUsingJson* UDeliveryBoxFactory::CreateObjectDeliveryBoxUsingJson(UClass* TargetClass)
{
	auto deliveryBox = NewObject<UObjectDeliveryBoxUsingJson>();

	deliveryBox->Initialize(TargetClass);

	return deliveryBox;
}

UObjectDeliveryBoxUsingJson* UDeliveryBoxFactory::CreateDynamicObjectDeliveryBoxUsingJson()
{
	auto deliveryBox = NewObject<UObjectDeliveryBoxUsingJson>();

	TMap<UClass*, EODJsonSerializeType> OverrideObjectSerializerClasses;
	deliveryBox->InitializeCustom(EODJsonSerializeType::WriteType, OverrideObjectSerializerClasses, nullptr);

	return deliveryBox;
}

UObjectDeliveryBoxUsingJson* UDeliveryBoxFactory::CreateCustomObjectDeliveryBoxUsingJson(EODJsonSerializeType DefaultSerializerType, const TMap<UClass*, EODJsonSerializeType>& ObjectSerializerTypes, UClass* TargetClass/* = nullptr*/)
{
	auto deliveryBox = NewObject<UObjectDeliveryBoxUsingJson>();

	deliveryBox->InitializeCustom(DefaultSerializerType, ObjectSerializerTypes, TargetClass);

	return deliveryBox;
}

UUtf8StringDeliveryBox* UDeliveryBoxFactory::CreateUtf8StringDeliveryBox()
{
	return NewObject<UUtf8StringDeliveryBox>();
}