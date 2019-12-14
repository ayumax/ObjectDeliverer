// Copyright 2019 ayumax. All Rights Reserved.
#include "ODOverrideJsonSerializer.h"
#include "IODConvertPropertyName.h"
#include "ODJsonSerializer.h"

TSharedPtr<FJsonObject> UODOverrideJsonSerializer::UObjectToJsonObject(UODJsonSerializer* JsonSerializer, const UObject* Obj, int64 CheckFlags, int64 SkipFlags) const
{
	// invalid
	return TSharedPtr<FJsonObject>();
}


TSharedPtr<FJsonObject> UODNoWriteTypeJsonSerializer::UObjectToJsonObject(UODJsonSerializer* JsonSerializer, const UObject* Obj, int64 CheckFlags, int64 SkipFlags) const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!Obj) return JsonObject;

	for (TFieldIterator<UProperty> PropIt(Obj->GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		JsonSerializer->AddJsonValue(JsonObject, Obj, *PropIt, CheckFlags, SkipFlags);
	}

	return JsonObject;
}


TSharedPtr<FJsonObject> UODWriteTypeJsonSerializer::UObjectToJsonObject(UODJsonSerializer* JsonSerializer, const UObject* Obj, int64 CheckFlags, int64 SkipFlags) const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!Obj) return JsonObject;

	JsonObject->SetStringField("Type", Obj->GetName());

	TSharedPtr<FJsonObject> JsonObjectBody = MakeShareable(new FJsonObject());

	for (TFieldIterator<UProperty> PropIt(Obj->GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		JsonSerializer->AddJsonValue(JsonObjectBody, Obj, *PropIt, CheckFlags, SkipFlags);
	}

	JsonObject->SetObjectField("Body", JsonObjectBody);

	return JsonObject;
}
