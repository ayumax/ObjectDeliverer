// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ODObjectUtil.generated.h"

UCLASS(BlueprintType)
class OBJECTDELIVERER_API UODObjectUtil : public UObject
{
	GENERATED_BODY()

public:
	static void EnumProperties(UObject* TargetObject, TFunction<bool(FProperty*)> EnumFunc);
	static bool FindClass(const FString& ClassName, UClass*& Class);

};
