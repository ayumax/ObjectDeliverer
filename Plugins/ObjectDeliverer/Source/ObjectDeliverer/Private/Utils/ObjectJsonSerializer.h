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

	TSharedPtr<FJsonObject> CreateJsonObject(const UObject* Obj);
	bool JsonObjectToUObject(const TSharedPtr<FJsonObject>& JsonObject, UObject* OutObject);

private:
	TSharedPtr<FJsonValue> ObjectJsonCallback(UProperty* Property, const void* Value);
	bool JsonObjectToUStruct(const TSharedPtr<FJsonObject>& JsonObject, const UStruct* StructDefinition, void* OutStruct);
	bool JsonValueToUProperty(const TSharedPtr<FJsonValue>& JsonValue, UProperty* Property, void* OutValue);
	bool ConvertScalarJsonValueToUPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, UProperty* Property, void* OutValue);

	bool JsonValueToUEnumProperty(const TSharedPtr<FJsonValue>& JsonValue, UEnumProperty* Property, void* OutValue);
	bool JsonValueToUNumericProperty(const TSharedPtr<FJsonValue>& JsonValue, UNumericProperty* NumericProperty, void* OutValue);
	bool JsonValueToUBoolProperty(const TSharedPtr<FJsonValue>& JsonValue, UBoolProperty* BoolProperty, void* OutValue);
	bool JsonValueToUStrProperty(const TSharedPtr<FJsonValue>& JsonValue, UStrProperty* StringProperty, void* OutValue);
	bool JsonValueToUArrayProperty(const TSharedPtr<FJsonValue>& JsonValue, UArrayProperty* ArrayProperty, void* OutValue);
	bool JsonValueToUMapProperty(const TSharedPtr<FJsonValue>& JsonValue, UMapProperty* MapProperty, void* OutValue);
	bool JsonValueToUSetProperty(const TSharedPtr<FJsonValue>& JsonValue, USetProperty* SetProperty, void* OutValue);
	bool JsonValueToUTextProperty(const TSharedPtr<FJsonValue>& JsonValue, UTextProperty* TextProperty, void* OutValue);
	bool JsonValueToUStructProperty(const TSharedPtr<FJsonValue>& JsonValue, UStructProperty* StructProperty, void* OutValue);
	bool JsonValueToUObjectProperty(const TSharedPtr<FJsonValue>& JsonValue, UObjectProperty* ObjectProperty, void* OutValue);


};
