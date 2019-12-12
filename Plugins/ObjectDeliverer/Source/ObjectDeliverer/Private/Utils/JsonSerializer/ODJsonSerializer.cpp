// Copyright 2019 ayumax. All Rights Reserved.
#include "ODJsonSerializer.h"
#include "Utils/ODObjectUtil.h"
#include "JsonObjectConverter.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "UObject/EnumProperty.h"
#include "UObject/TextProperty.h"
#include "UObject/PropertyPortFlags.h"



TSharedPtr<FJsonObject> ODJsonSerializer::CreateJsonObject(const UObject* Obj)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!Obj) return JsonObject;

	for (TFieldIterator<UProperty> PropIt(Obj->GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		UProperty* Property = *PropIt;
		FString PropertyName = Property->GetName();
		uint8* CurrentPropAddr = PropIt->ContainerPtrToValuePtr<uint8>((UObject*)Obj);

		FJsonObjectConverter::CustomExportCallback CustomCB;
		CustomCB.BindRaw(this, &ODJsonSerializer::ObjectJsonCallback);
		JsonObject->SetField(PropertyName, FJsonObjectConverter::UPropertyToJsonValue(Property, CurrentPropAddr, 0, 0, &CustomCB));

	}

	return JsonObject;
}

TSharedPtr<FJsonValue> ODJsonSerializer::ObjectJsonCallback(UProperty* Property, const void* Value)
{
	if (UObjectProperty * ObjectProperty = Cast<UObjectProperty>(Property))
	{
		if (!ObjectProperty->HasAnyFlags(RF_Transient)) // We are taking Transient to mean we don't want to serialize to Json either (could make a new flag if nessasary)
		{
			return MakeShareable(new FJsonValueObject(CreateJsonObject(ObjectProperty->GetObjectPropertyValue(Value))));
		}
	}

	// invalid
	return TSharedPtr<FJsonValue>();
}

bool ODJsonSerializer::JsonObjectToUObject(const TSharedPtr<FJsonObject>& JsonObject, UObject* OutObject)
{

	return true;
}
