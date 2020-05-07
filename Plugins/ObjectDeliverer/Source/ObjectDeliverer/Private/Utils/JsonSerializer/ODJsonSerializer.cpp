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
#include "DeliveryBox/IODConvertPropertyName.h"

UODJsonSerializer::UODJsonSerializer()
{
}

TSharedPtr<FJsonObject> UODJsonSerializer::CreateJsonObject(const UObject* Obj, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/)
{
	return UObjectToJsonObject(Obj->StaticClass(), Obj, CheckFlags, SkipFlags);
}

TSharedPtr<FJsonValue> UODJsonSerializer::ObjectJsonCallback(FProperty* Property, const void* Value)
{
	if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
	{
		if (!ObjectProperty->HasAnyFlags(RF_Transient)) // We are taking Transient to mean we don't want to serialize to Json either (could make a new flag if nessasary)
		{
			return MakeShareable(new FJsonValueObject(UObjectToJsonObject(ObjectProperty->PropertyClass, ObjectProperty->GetObjectPropertyValue(Value))));
		}
	}

	// invalid
	return TSharedPtr<FJsonValue>();
}

TSharedPtr<FJsonObject> UODJsonSerializer::UObjectToJsonObject(UClass* ObjectClass, const UObject* Obj, int64 CheckFlags, int64 SkipFlags)
{
	if (!Obj) return MakeShareable(new FJsonObject());

	auto objectSerializer = DefaultSerializer;

	if (ObjectSerializeres.Contains(ObjectClass))
	{
		objectSerializer = ObjectSerializeres[ObjectClass];
	}

	return objectSerializer->UObjectToJsonObject(this, Obj, CheckFlags, SkipFlags);
}

void UODJsonSerializer::AddJsonValue(TSharedPtr<FJsonObject> JsonObject, const UObject* Obj, FProperty* Property, int64 CheckFlags, int64 SkipFlags)
{
	FString PropertyName = Property->GetName();
	if (Obj->GetClass()->ImplementsInterface(UODConvertPropertyName::StaticClass()))
	{
		FString ConvertedPropertyName = IODConvertPropertyName::Execute_ConvertFPropertyName(Obj, Property->GetFName());
		if (!ConvertedPropertyName.IsEmpty())
		{
			PropertyName = ConvertedPropertyName;
		}
	}

	uint8* CurrentPropAddr = Property->ContainerPtrToValuePtr<uint8>((UObject*)Obj);

	FJsonObjectConverter::CustomExportCallback CustomCB;
	CustomCB.BindLambda([this](FProperty* _Property, const void* Value)
		{
			return ObjectJsonCallback(_Property, Value);
		});

	JsonObject->SetField(PropertyName, FJsonObjectConverter::UPropertyToJsonValue(Property, CurrentPropAddr, CheckFlags, SkipFlags, &CustomCB));
}