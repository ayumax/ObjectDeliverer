#pragma once

#include "CoreMinimal.h"
#include "CNStringUtil.generated.h"


UCLASS(BlueprintType)
class COMMNET_API UCNStringUtil : public UObject
{
	GENERATED_BODY()

public:
	UCNStringUtil() {}
	~UCNStringUtil() {}

	UFUNCTION(BlueprintPure, Category = "CommNet")
	static void StringToBuffer(const FString& message, TArray<uint8>& DataBuffer);

	UFUNCTION(BlueprintPure, Category = "CommNet")
	static FString BufferToString(const TArray<uint8>& DataBuffer);

};
