#pragma once

struct FDeliveryDataType
{
	enum EMainType
	{
		Binary,
		String,
		Other
	} MainType;

	FString SubType;

	FDeliveryDataType(const EMainType Main, const FString& Sub)
	{
		MainType = Main;
		SubType = Sub;
	}

	static FDeliveryDataType Default()
	{
		return FDeliveryDataType(EMainType::Binary, FString());
	}
};
