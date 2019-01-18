#pragma once

#include "CoreMinimal.h"
#include "CNObjectUtil.generated.h"

UENUM()
enum class EUPropertyType : uint8
{
	None,

	Bool,
	Int32,
	Float,
	Double,
	UInt8,
	Enum,
	Array,
	Map,
	Set,
	Struct,
	Object
};

UCLASS(BlueprintType)
class COMMNET_API UCNObjectUtil : public UObject
{
	GENERATED_BODY()

public:
	UCNObjectUtil() {}
	~UCNObjectUtil() {}

	static EUPropertyType GetUPropertyType(UProperty* Property);


};
