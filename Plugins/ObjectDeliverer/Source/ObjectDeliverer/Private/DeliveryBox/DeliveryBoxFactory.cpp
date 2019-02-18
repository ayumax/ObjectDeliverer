// Copyright 2019 ayumax. All Rights Reserved.
#include "DeliveryBoxFactory.h"
#include "ObjectDeliveryBoxUsingJson.h"
#include "Utf8StringDeliveryBox.h"

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