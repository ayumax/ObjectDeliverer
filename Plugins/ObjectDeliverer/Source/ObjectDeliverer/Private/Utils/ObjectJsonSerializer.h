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

  /* * Serializes UObject as FJsonObject
   *
   * @param Obj			      The object to serialize
   * @param CheckFlags		Only convert properties that match at least one of these flags. If 0 check all properties.
   * @param SkipFlags			Skip properties that match any of these flags
   *
   * @return					    The constructed FJsonObject shared ptr from the Obj
   */
	static TSharedPtr<FJsonObject> CreateJsonObject(const UObject* Obj, int64 CheckFlags=0, int64 SkipFlags=0);

  /* * Create UObject from serialized FJsonObject
  *
  * @param JsonObject		  The Json with serailized UObject information
  * @param OutObject		  Created object
  *
  * @return					      If created successfully
  */
	static bool JsonObjectToUObject(const TSharedPtr<FJsonObject>& JsonObject, UObject*& OutObject);

private:
	static TSharedPtr<FJsonValue> ObjectJsonCallback(UProperty* Property, const void* Value);
	static bool JsonObjectToUStruct(const TSharedPtr<FJsonObject>& JsonObject, const UStruct* StructDefinition, void* OutStruct);
	static bool JsonValueToUProperty(const TSharedPtr<FJsonValue>& JsonValue, UProperty* Property, void* OutValue);
	static bool ConvertScalarJsonValueToUPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, UProperty* Property, void* OutValue);

	static bool JsonValueToUEnumProperty(const TSharedPtr<FJsonValue>& JsonValue, UEnumProperty* Property, void* OutValue);
	static bool JsonValueToUNumericProperty(const TSharedPtr<FJsonValue>& JsonValue, UNumericProperty* NumericProperty, void* OutValue);
	static bool JsonValueToUBoolProperty(const TSharedPtr<FJsonValue>& JsonValue, UBoolProperty* BoolProperty, void* OutValue);
	static bool JsonValueToUStrProperty(const TSharedPtr<FJsonValue>& JsonValue, UStrProperty* StringProperty, void* OutValue);
	static bool JsonValueToUArrayProperty(const TSharedPtr<FJsonValue>& JsonValue, UArrayProperty* ArrayProperty, void* OutValue);
	static bool JsonValueToUMapProperty(const TSharedPtr<FJsonValue>& JsonValue, UMapProperty* MapProperty, void* OutValue);
	static bool JsonValueToUSetProperty(const TSharedPtr<FJsonValue>& JsonValue, USetProperty* SetProperty, void* OutValue);
	static bool JsonValueToUTextProperty(const TSharedPtr<FJsonValue>& JsonValue, UTextProperty* TextProperty, void* OutValue);
	static bool JsonValueToUStructProperty(const TSharedPtr<FJsonValue>& JsonValue, UStructProperty* StructProperty, void* OutValue);
	static bool JsonValueToUObjectProperty(const TSharedPtr<FJsonValue>& JsonValue, UObjectProperty* ObjectProperty, void* OutValue);

  static const FString objectClassNameKey;
  static const FString objectPropertiesKey;
};
