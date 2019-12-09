// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ODObjectUtil.generated.h"

UENUM()
enum class EUPropertyType : uint8
{
	None,

	Bool,
	Int,
	Float,
	Double,
	UInt8,
	Enum,
	String,
	Array,
	Map,
	Set,
	Struct,
	Object
};


UCLASS(BlueprintType)
class OBJECTDELIVERER_API UODObjectUtil : public UObject
{
	GENERATED_BODY()

public:
	static EUPropertyType GetUPropertyType(UProperty* Property);
	static void EnumProperties(UObject* TargetObject, TFunction<bool(UProperty*)> EnumFunc);

};
