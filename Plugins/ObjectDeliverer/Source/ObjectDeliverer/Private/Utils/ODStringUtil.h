// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ODStringUtil.generated.h"


UCLASS(BlueprintType)
class OBJECTDELIVERER_API UODStringUtil : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer")
	static void StringToBuffer(const FString& message, TArray<uint8>& DataBuffer);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer")
	static FString BufferToString(const TArray<uint8>& DataBuffer);

};
