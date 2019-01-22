#pragma once

#include "CoreMinimal.h"
#include "ObjectUtil.generated.h"

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
class OBJECTDELIVERER_API UObjectUtil : public UObject
{
	GENERATED_BODY()

public:
	UObjectUtil() {}
	~UObjectUtil() {}

	static EUPropertyType GetUPropertyType(UProperty* Property);


};
