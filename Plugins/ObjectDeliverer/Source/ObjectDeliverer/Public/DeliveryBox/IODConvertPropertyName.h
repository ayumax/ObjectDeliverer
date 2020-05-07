// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IODConvertPropertyName.generated.h"

UINTERFACE(BlueprintType)
class OBJECTDELIVERER_API UODConvertPropertyName : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class OBJECTDELIVERER_API IODConvertPropertyName
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ObjectDeliverer")
	FString ConvertFPropertyName(const FName& PropertyName) const;
};