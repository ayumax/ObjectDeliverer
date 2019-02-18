// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "StringUtil.generated.h"


UCLASS(BlueprintType)
class OBJECTDELIVERER_API UStringUtil : public UObject
{
	GENERATED_BODY()

public:
	UStringUtil() {}
	~UStringUtil() {}

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer")
	static void StringToBuffer(const FString& message, TArray<uint8>& DataBuffer);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer")
	static FString BufferToString(const TArray<uint8>& DataBuffer);

};
