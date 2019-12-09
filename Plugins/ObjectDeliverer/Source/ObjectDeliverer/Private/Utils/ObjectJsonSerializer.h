// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "ObjectJsonSerializer.generated.h"

UCLASS(BlueprintType)
class OBJECTDELIVERER_API UObjectJsonSerializer : public UObject
{
	GENERATED_BODY()

public:

	static TSharedPtr<FJsonObject> CreateJsonObject(const UObject* Obj);
	static bool JsonObjectToUObject(const TSharedPtr<FJsonObject>& JsonObject, UObject* OutObject);

private:
	TSharedPtr<FJsonValue> ObjectJsonCallback(UProperty* Property, const void* Value);
	
};
