// Copyright 2019 ayumax. All Rights Reserved.
#include "DeliveryBox/ODOverrideJsonSerializer.h"
#include "DeliveryBox/IODConvertPropertyName.h"
#include "../Utils/JsonSerializer/ODJsonSerializer.h"
#include "../Utils/JsonSerializer/ODJsonDeserializer.h"
#include "../Utils/ODObjectUtil.h"

TSharedPtr<FJsonObject> UODOverrideJsonSerializer::UObjectToJsonObject(UODJsonSerializer* JsonSerializer, const UObject* Obj, int64 CheckFlags, int64 SkipFlags) const
{
	// invalid
	return TSharedPtr<FJsonObject>();
}

UObject* UODOverrideJsonSerializer::JsonObjectTopUObject(UODJsonDeserializer* JsonDeserializer, const TSharedPtr<FJsonObject> JsonObject, UClass* TargetClass) const
{
	return nullptr;
}

TSharedPtr<FJsonObject> UODNoWriteTypeJsonSerializer::UObjectToJsonObject(UODJsonSerializer* JsonSerializer, const UObject* Obj, int64 CheckFlags, int64 SkipFlags) const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!Obj) return JsonObject;

	for (TFieldIterator<FProperty> PropIt(Obj->GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		JsonSerializer->AddJsonValue(JsonObject, Obj, *PropIt, CheckFlags, SkipFlags);
	}

	return JsonObject;
}

UObject* UODNoWriteTypeJsonSerializer::JsonObjectTopUObject(UODJsonDeserializer* JsonDeserializer, const TSharedPtr<FJsonObject> JsonObject, UClass* TargetClass) const
{
	if (!TargetClass) return nullptr;

	UObject* createdObj = NewObject<UObject>((UObject*)GetTransientPackage(), TargetClass);

	for (TFieldIterator<FProperty> PropIt(createdObj->GetClass()); PropIt; ++PropIt)
	{
		JsonDeserializer->JsonPropertyToFProperty(JsonObject, *PropIt, createdObj);
	}

	return createdObj;
}

TSharedPtr<FJsonObject> UODWriteTypeJsonSerializer::UObjectToJsonObject(UODJsonSerializer* JsonSerializer, const UObject* Obj, int64 CheckFlags, int64 SkipFlags) const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!Obj) return JsonObject;

	JsonObject->SetStringField(TEXT("Type"), Obj->GetClass()->GetName());

	TSharedPtr<FJsonObject> JsonObjectBody = MakeShareable(new FJsonObject());

	for (TFieldIterator<FProperty> PropIt(Obj->GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		JsonSerializer->AddJsonValue(JsonObjectBody, Obj, *PropIt, CheckFlags, SkipFlags);
	}

	JsonObject->SetObjectField(TEXT("Body"), JsonObjectBody);

	return JsonObject;
}

UObject* UODWriteTypeJsonSerializer::JsonObjectTopUObject(UODJsonDeserializer* JsonDeserializer, const TSharedPtr<FJsonObject> JsonObject, UClass* TargetClass) const
{
	FString typeValue;
	if (!JsonObject->TryGetStringField(TEXT("Type"), typeValue))
	{
		return nullptr;
	}

	UClass* targetType;
	if (!UODObjectUtil::FindClass(typeValue, targetType))
	{
		return nullptr;
	}

	UObject* createdObj = NewObject<UObject>((UObject*)GetTransientPackage(), targetType);

	const TSharedPtr<FJsonObject>* bodyJsonObject;
	if (!JsonObject->TryGetObjectField(TEXT("Body"), bodyJsonObject))
	{
		return nullptr;
	}

	for (TFieldIterator<FProperty> PropIt(createdObj->GetClass()); PropIt; ++PropIt)
	{
		JsonDeserializer->JsonPropertyToFProperty(*bodyJsonObject, *PropIt, createdObj);
	}

	return createdObj;
}