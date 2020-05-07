// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "ODJsonSerializerBase.h"
#include "Dom/JsonObject.h"
#include "ODJsonDeserializer.generated.h"

UCLASS()
class OBJECTDELIVERER_API UODJsonDeserializer : public UODJsonSerializerBase
{
	GENERATED_BODY()

public:
	UODJsonDeserializer();

	virtual UObject* JsonObjectToUObject(const TSharedPtr<FJsonObject>& JsonObject, UClass* TargetClass = nullptr);

	virtual bool JsonPropertyToFProperty(const TSharedPtr<FJsonObject>& JsonObject, FProperty* Property, UObject* OutObject);

protected:
	virtual bool JsonValueToFProperty(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue);
	virtual bool ConvertScalarJsonValueToFPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue);
	virtual bool JsonValueToFEnumProperty(const TSharedPtr<FJsonValue>& JsonValue, FEnumProperty* Property, void* OutValue);
	virtual bool JsonValueToFNumericProperty(const TSharedPtr<FJsonValue>& JsonValue, FNumericProperty* NumericProperty, void* OutValue);
	virtual bool JsonValueToFBoolProperty(const TSharedPtr<FJsonValue>& JsonValue, FBoolProperty* BoolProperty, void* OutValue);
	virtual bool JsonValueToFStrProperty(const TSharedPtr<FJsonValue>& JsonValue, FStrProperty* StringProperty, void* OutValue);
	virtual bool JsonValueToFArrayProperty(const TSharedPtr<FJsonValue>& JsonValue, FArrayProperty* ArrayProperty, void* OutValue);
	virtual bool JsonValueToFMapProperty(const TSharedPtr<FJsonValue>& JsonValue, FMapProperty* MapProperty, void* OutValue);
	virtual bool JsonValueToFSetProperty(const TSharedPtr<FJsonValue>& JsonValue, FSetProperty* SetProperty, void* OutValue);
	virtual bool JsonValueToFTextProperty(const TSharedPtr<FJsonValue>& JsonValue, FTextProperty* TextProperty, void* OutValue);
	virtual bool JsonValueToFStructProperty(const TSharedPtr<FJsonValue>& JsonValue, FStructProperty* StructProperty, void* OutValue);
	virtual bool JsonValueToFObjectProperty(const TSharedPtr<FJsonValue>& JsonValue, FObjectProperty* ObjectProperty, void* OutValue);

	virtual bool JsonObjectToUStruct(const TSharedPtr<FJsonObject>& JsonObject, const UStruct* StructDefinition, void* OutStruct);

};
