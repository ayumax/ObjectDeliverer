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

UUtf8StringDeliveryBox* UDeliveryBoxFactory::CreateUtf8StringDeliveryBox()
{
	return NewObject<UUtf8StringDeliveryBox>();
}