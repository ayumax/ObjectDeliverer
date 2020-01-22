// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "ODOverrideJsonSerializer.generated.h"

class UODJsonSerializer;
class UODJsonDeserializer;

UENUM(BlueprintType)
enum class EODJsonSerializeType : uint8
{
	/** Do not include the class name */
	NoWriteType,

	/** Include the class name */
	WriteType
};

UCLASS(BlueprintType)
class OBJECTDELIVERER_API UODOverrideJsonSerializer : public UObject
{
	GENERATED_BODY()

public:
	virtual TSharedPtr<FJsonObject> UObjectToJsonObject(UODJsonSerializer* JsonSerializer, const UObject* Obj, int64 CheckFlags = 0, int64 SkipFlags = 0) const;
	virtual UObject* JsonObjectTopUObject(UODJsonDeserializer* JsonDeserializer, const TSharedPtr<FJsonObject> JsonObject, UClass* TargetClass = nullptr) const;
};

UCLASS(BlueprintType)
class OBJECTDELIVERER_API UODNoWriteTypeJsonSerializer : public UODOverrideJsonSerializer
{
	GENERATED_BODY()

public:
	virtual TSharedPtr<FJsonObject> UObjectToJsonObject(UODJsonSerializer* JsonSerializer, const UObject* Obj, int64 CheckFlags = 0, int64 SkipFlags = 0) const override;
	virtual UObject* JsonObjectTopUObject(UODJsonDeserializer* JsonDeserializer, const TSharedPtr<FJsonObject> JsonObject, UClass* TargetClass = nullptr) const override;
};

UCLASS(BlueprintType)
class OBJECTDELIVERER_API UODWriteTypeJsonSerializer : public UODOverrideJsonSerializer
{
	GENERATED_BODY()

public:
	virtual TSharedPtr<FJsonObject> UObjectToJsonObject(UODJsonSerializer* JsonSerializer, const UObject* Obj, int64 CheckFlags = 0, int64 SkipFlags = 0) const override;
	virtual UObject* JsonObjectTopUObject(UODJsonDeserializer* JsonDeserializer, const TSharedPtr<FJsonObject> JsonObject, UClass* TargetClass = nullptr) const override;
};