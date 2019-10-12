// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GetIPV4Info.generated.h"

UINTERFACE(BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class OBJECTDELIVERER_API UGetIPV4Info : public UInterface
{
	GENERATED_BODY()
};


class OBJECTDELIVERER_API IGetIPV4Info 
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer")
	virtual bool GetIPAddress(TArray<uint8>& IPAddress) = 0;

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer")
	virtual bool GetIPAddressInString(FString& IPAddress) = 0;
};
