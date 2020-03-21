// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "ODJsonSerializerBase.h"
#include "Dom/JsonObject.h"
#include "ODJsonSerializer.generated.h"

UCLASS()
class OBJECTDELIVERER_API UODJsonSerializer : public UODJsonSerializerBase
{
	GENERATED_BODY()

public:
	UODJsonSerializer();

	TSharedPtr<FJsonObject> CreateJsonObject(const UObject* Obj, int64 CheckFlags = 0, int64 SkipFlags = 0);

	void AddJsonValue(TSharedPtr<FJsonObject> JsonObject, const UObject* Obj, FProperty* Property, int64 CheckFlags, int64 SkipFlags);

private:
	TSharedPtr<FJsonValue> ObjectJsonCallback(FProperty* Property, const void* Value);

protected:
	virtual TSharedPtr<FJsonObject> UObjectToJsonObject(UClass* ObjectClass, const UObject* Obj, int64 CheckFlags = 0, int64 SkipFlags = 0);

};
