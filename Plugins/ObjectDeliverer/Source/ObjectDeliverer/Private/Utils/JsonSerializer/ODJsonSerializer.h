// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "ODJsonSerializer.generated.h"

class UODOverrideJsonSerializer;

UCLASS()
class OBJECTDELIVERER_API UODJsonSerializer : public UObject
{
	GENERATED_BODY()

public:
	UODJsonSerializer();

	void AddOverrideJsonSerializers(const TMap<UClass*, UODOverrideJsonSerializer*>& OverrideObjectSerializers);

	TSharedPtr<FJsonObject> CreateJsonObject(const UObject* Obj, int64 CheckFlags = 0, int64 SkipFlags = 0);

	void AddJsonValue(TSharedPtr<FJsonObject> JsonObject, const UObject* Obj, UProperty* Property, int64 CheckFlags, int64 SkipFlags);

private:
	TSharedPtr<FJsonValue> ObjectJsonCallback(UProperty* Property, const void* Value);

protected:
	virtual TSharedPtr<FJsonObject> UObjectToJsonObject(const UObject* Obj, int64 CheckFlags = 0, int64 SkipFlags = 0);

private:
	UPROPERTY(Transient)
	UODOverrideJsonSerializer* DefaultObjectSerializer;

	UPROPERTY(Transient)
	TMap<UClass*, UODOverrideJsonSerializer*> ObjectSerializers;
};
