// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "ODJsonSerializerBase.h"
#include "Dom/JsonObject.h"
#include "ODJsonDeserializer.generated.h"

class UODOverrideJsonSerializer;

UCLASS()
class OBJECTDELIVERER_API UODJsonDeserializer : public UODJsonSerializerBase
{
	GENERATED_BODY()

public:
	UODJsonDeserializer();

	virtual UObject* JsonObjectToUObject(const TSharedPtr<FJsonObject>& JsonObject, UClass* TargetClass = nullptr);

	virtual bool JsonPropertyToUProperty(const TSharedPtr<FJsonObject>& JsonObject, UProperty* Property, UObject* OutObject);

protected:
	virtual bool JsonValueToUProperty(const TSharedPtr<FJsonValue>& JsonValue, UProperty* Property, void* OutValue);
	virtual bool ConvertScalarJsonValueToUPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, UProperty* Property, void* OutValue);
	virtual bool JsonValueToUEnumProperty(const TSharedPtr<FJsonValue>& JsonValue, UEnumProperty* Property, void* OutValue);
	virtual bool JsonValueToUNumericProperty(const TSharedPtr<FJsonValue>& JsonValue, UNumericProperty* NumericProperty, void* OutValue);
	virtual bool JsonValueToUBoolProperty(const TSharedPtr<FJsonValue>& JsonValue, UBoolProperty* BoolProperty, void* OutValue);
	virtual bool JsonValueToUStrProperty(const TSharedPtr<FJsonValue>& JsonValue, UStrProperty* StringProperty, void* OutValue);
	virtual bool JsonValueToUArrayProperty(const TSharedPtr<FJsonValue>& JsonValue, UArrayProperty* ArrayProperty, void* OutValue);
	virtual bool JsonValueToUMapProperty(const TSharedPtr<FJsonValue>& JsonValue, UMapProperty* MapProperty, void* OutValue);
	virtual bool JsonValueToUSetProperty(const TSharedPtr<FJsonValue>& JsonValue, USetProperty* SetProperty, void* OutValue);
	virtual bool JsonValueToUTextProperty(const TSharedPtr<FJsonValue>& JsonValue, UTextProperty* TextProperty, void* OutValue);
	virtual bool JsonValueToUStructProperty(const TSharedPtr<FJsonValue>& JsonValue, UStructProperty* StructProperty, void* OutValue);
	virtual bool JsonValueToUObjectProperty(const TSharedPtr<FJsonValue>& JsonValue, UObjectProperty* ObjectProperty, void* OutValue);

	virtual bool JsonObjectToUStruct(const TSharedPtr<FJsonObject>& JsonObject, const UStruct* StructDefinition, void* OutStruct);

};
