// Copyright 2019 ayumax. All Rights Reserved.
#include "ODJsonDeserializer.h"
#include "Utils/ODObjectUtil.h"
#include "JsonObjectConverter.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "UObject/EnumProperty.h"
#include "UObject/TextProperty.h"
#include "UObject/PropertyPortFlags.h"
#include "DeliveryBox/IODConvertPropertyName.h"
#include "Runtime/Launch/Resources/Version.h"

#define CHECK_UE_VERSION_ORUNDER_5_0 (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION <= 0) || ENGINE_MAJOR_VERSION == 4

UODJsonDeserializer::UODJsonDeserializer()
{
}


UObject* UODJsonDeserializer::JsonObjectToUObject(const TSharedPtr<FJsonObject>& JsonObject, UClass* TargetClass /*=nullptr*/)
{
	auto objectSerializer = DefaultSerializer;

	if (TargetClass)
	{
		if (ObjectSerializeres.Contains(TargetClass))
		{
			objectSerializer = ObjectSerializeres[TargetClass];
		}
	}

	return objectSerializer->JsonObjectTopUObject(this, JsonObject, TargetClass);
}

bool UODJsonDeserializer::JsonPropertyToFProperty(const TSharedPtr<FJsonObject>& JsonObject, FProperty* Property, UObject* OutObject)
{
	FString PropertyName = Property->GetName();
	if (OutObject->GetClass()->ImplementsInterface(UODConvertPropertyName::StaticClass()))
	{
		FString ConvertedPropertyName = IODConvertPropertyName::Execute_ConvertFPropertyName(OutObject, Property->GetFName());
		if (!ConvertedPropertyName.IsEmpty())
		{
			PropertyName = ConvertedPropertyName;
		}
	}

	auto& JsonAttributes = JsonObject->Values;
	const TSharedPtr<FJsonValue>* JsonValue = JsonAttributes.Find(PropertyName);
	if (!JsonValue)
	{
		return false;
	}

	if (JsonValue->IsValid() && !(*JsonValue)->IsNull())
	{
		void* Value = Property->ContainerPtrToValuePtr<uint8>(OutObject);

		if (JsonValueToFProperty(*JsonValue, Property, Value))
		{
			return true;
		}
	}

	return false;
}

bool UODJsonDeserializer::JsonObjectToUStruct(const TSharedPtr<FJsonObject>& JsonObject, const UStruct* StructDefinition, void* OutStruct)
{
	auto& JsonAttributes = JsonObject->Values;

	int32 NumUnclaimedProperties = JsonAttributes.Num();
	if (NumUnclaimedProperties <= 0)
	{
		return true;
	}

	for (TFieldIterator<FProperty> PropIt(StructDefinition); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;

		const TSharedPtr<FJsonValue>* JsonValue = JsonAttributes.Find(Property->GetName());
		if (!JsonValue)
		{
			continue;
		}

		if (JsonValue->IsValid() && !(*JsonValue)->IsNull())
		{
			void* Value = Property->ContainerPtrToValuePtr<uint8>(OutStruct);

			if (!JsonValueToFProperty(*JsonValue, Property, Value))
			{
				UE_LOG(LogJson, Error, TEXT("JsonObjectToUStruct - Unable to parse %s.%s from JSON"), *StructDefinition->GetName(), *Property->GetName());
				return false;
			}
		}

		if (--NumUnclaimedProperties <= 0)
		{
			break;
		}
	}

	return true;
}

bool UODJsonDeserializer::JsonValueToFProperty(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue)
{
	if (!JsonValue.IsValid())
	{
		UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Invalid value JSON key"));
		return false;
	}

	bool bArrayOrSetProperty = Property->IsA<FArrayProperty>() || Property->IsA<FSetProperty>();
	bool bJsonArray = JsonValue->Type == EJson::Array;

	if (!bJsonArray)
	{
		if (bArrayOrSetProperty)
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import TArray from non-array JSON key"));
			return false;
		}

		if (Property->ArrayDim != 1)
		{
			UE_LOG(LogJson, Warning, TEXT("Ignoring excess properties when deserializing %s"), *Property->GetName());
		}

		return ConvertScalarJsonValueToFPropertyWithContainer(JsonValue, Property, OutValue);
	}

	if (bArrayOrSetProperty && Property->ArrayDim == 1)
	{
		return ConvertScalarJsonValueToFPropertyWithContainer(JsonValue, Property, OutValue);
	}

	const auto& ArrayValue = JsonValue->AsArray();
	if (Property->ArrayDim < ArrayValue.Num())
	{
		UE_LOG(LogJson, Warning, TEXT("Ignoring excess properties when deserializing %s"), *Property->GetName());
	}

	int ItemsToRead = FMath::Clamp(ArrayValue.Num(), 0, Property->ArrayDim);
	for (int Index = 0; Index != ItemsToRead; ++Index)
	{
		if (!ConvertScalarJsonValueToFPropertyWithContainer(ArrayValue[Index], Property, (char*)OutValue + Index * Property->ElementSize))
		{
			return false;
		}
	}

	return true;
}

bool UODJsonDeserializer::ConvertScalarJsonValueToFPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue)
{
	if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
	{
		JsonValueToFEnumProperty(JsonValue, EnumProperty, OutValue);
	}
	else if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property))
	{
		JsonValueToFNumericProperty(JsonValue, NumericProperty, OutValue);
	}
	else if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
	{
		JsonValueToFBoolProperty(JsonValue, BoolProperty, OutValue);
	}
	else if (FStrProperty* StringProperty = CastField<FStrProperty>(Property))
	{
		JsonValueToFStrProperty(JsonValue, StringProperty, OutValue);
	}
	else if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
	{
		JsonValueToFArrayProperty(JsonValue, ArrayProperty, OutValue);
	}
	else if (FMapProperty* MapProperty = CastField<FMapProperty>(Property))
	{
		JsonValueToFMapProperty(JsonValue, MapProperty, OutValue);
	}
	else if (FSetProperty* SetProperty = CastField<FSetProperty>(Property))
	{
		JsonValueToFSetProperty(JsonValue, SetProperty, OutValue);
	}
	else if (FTextProperty* TextProperty = CastField<FTextProperty>(Property))
	{
		JsonValueToFTextProperty(JsonValue, TextProperty, OutValue);
	}
	else if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
	{
		JsonValueToFStructProperty(JsonValue, StructProperty, OutValue);
	}
	else if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
	{
		JsonValueToFObjectProperty(JsonValue, ObjectProperty, OutValue);
	}
	else
	{
		// Default to expect a string for everything else
#if CHECK_UE_VERSION_ORUNDER_5_0
		if (Property->ImportText(*JsonValue->AsString(), OutValue, 0, NULL) == NULL)
#else
		if (Property->ImportText_Direct(*JsonValue->AsString(), OutValue, NULL, 0) == NULL)
#endif
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable import property type %s from string value for property %s"), *Property->GetClass()->GetName(), *Property->GetNameCPP());
			return false;
		}
	}

	return true;
}

bool UODJsonDeserializer::JsonValueToFEnumProperty(const TSharedPtr<FJsonValue>& JsonValue, FEnumProperty* Property, void* OutValue)
{
	if (JsonValue->Type == EJson::String)
	{
		// see if we were passed a string for the enum
		const UEnum* Enum = Property->GetEnum();
		check(Enum);
		FString StrValue = JsonValue->AsString();
		int64 IntValue = Enum->GetValueByName(FName(*StrValue));
		if (IntValue == INDEX_NONE)
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable import enum %s from string value %s for property %s"), *Enum->CppType, *StrValue, *Property->GetNameCPP());
			return false;
		}
		Property->GetUnderlyingProperty()->SetIntPropertyValue(OutValue, IntValue);
	}
	else
	{
		// AsNumber will log an error for completely inappropriate types (then give us a default)
		Property->GetUnderlyingProperty()->SetIntPropertyValue(OutValue, (int64)JsonValue->AsNumber());
	}

	return true;
}

bool UODJsonDeserializer::JsonValueToFNumericProperty(const TSharedPtr<FJsonValue>& JsonValue, FNumericProperty* NumericProperty, void* OutValue)
{
	if (NumericProperty->IsEnum() && JsonValue->Type == EJson::String)
	{
		// see if we were passed a string for the enum
		const UEnum* Enum = NumericProperty->GetIntPropertyEnum();
		check(Enum); // should be assured by IsEnum()
		FString StrValue = JsonValue->AsString();
		int64 IntValue = Enum->GetValueByName(FName(*StrValue));
		if (IntValue == INDEX_NONE)
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable import enum %s from string value %s for property %s"), *Enum->CppType, *StrValue, *NumericProperty->GetNameCPP());
			return false;
		}
		NumericProperty->SetIntPropertyValue(OutValue, IntValue);
	}
	else if (NumericProperty->IsFloatingPoint())
	{
		// AsNumber will log an error for completely inappropriate types (then give us a default)
		NumericProperty->SetFloatingPointPropertyValue(OutValue, JsonValue->AsNumber());
	}
	else if (NumericProperty->IsInteger())
	{
		if (JsonValue->Type == EJson::String)
		{
			// parse string -> int64 ourselves so we don't lose any precision going through AsNumber (aka double)
			NumericProperty->SetIntPropertyValue(OutValue, FCString::Atoi64(*JsonValue->AsString()));
		}
		else
		{
			// AsNumber will log an error for completely inappropriate types (then give us a default)
			NumericProperty->SetIntPropertyValue(OutValue, (int64)JsonValue->AsNumber());
		}
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable to set numeric property type %s for property %s"), *NumericProperty->GetClass()->GetName(), *NumericProperty->GetNameCPP());
		return false;
	}

	return true;
}

bool UODJsonDeserializer::JsonValueToFBoolProperty(const TSharedPtr<FJsonValue>& JsonValue, FBoolProperty* BoolProperty, void* OutValue)
{
	// AsBool will log an error for completely inappropriate types (then give us a default)
	BoolProperty->SetPropertyValue(OutValue, JsonValue->AsBool());
	return true;
}

bool UODJsonDeserializer::JsonValueToFStrProperty(const TSharedPtr<FJsonValue>& JsonValue, FStrProperty* StringProperty, void* OutValue)
{
	// AsString will log an error for completely inappropriate types (then give us a default)
	StringProperty->SetPropertyValue(OutValue, JsonValue->AsString());
	return true;
}

bool UODJsonDeserializer::JsonValueToFArrayProperty(const TSharedPtr<FJsonValue>& JsonValue, FArrayProperty* ArrayProperty, void* OutValue)
{
	if (JsonValue->Type == EJson::Array)
	{
		TArray< TSharedPtr<FJsonValue> > ArrayValue = JsonValue->AsArray();
		int32 ArrLen = ArrayValue.Num();

		// make the output array size match
		FScriptArrayHelper Helper(ArrayProperty, OutValue);
		Helper.Resize(ArrLen);

		// set the property values
		for (int32 i = 0; i < ArrLen; ++i)
		{
			const TSharedPtr<FJsonValue>& ArrayValueItem = ArrayValue[i];
			if (ArrayValueItem.IsValid() && !ArrayValueItem->IsNull())
			{
				if (!JsonValueToFProperty(ArrayValueItem, ArrayProperty->Inner, Helper.GetRawPtr(i)))
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable to deserialize array element [%d] for property %s"), i, *ArrayProperty->GetNameCPP());
					return false;
				}
			}
		}
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import TArray from non-array JSON key for property %s"), *ArrayProperty->GetNameCPP());
		return false;
	}

	return true;
}

bool UODJsonDeserializer::JsonValueToFMapProperty(const TSharedPtr<FJsonValue>& JsonValue, FMapProperty* MapProperty, void* OutValue)
{
	if (JsonValue->Type == EJson::Object)
	{
		TSharedPtr<FJsonObject> ObjectValue = JsonValue->AsObject();

		FScriptMapHelper Helper(MapProperty, OutValue);

		// set the property values
		for (const auto& Entry : ObjectValue->Values)
		{
			if (Entry.Value.IsValid() && !Entry.Value->IsNull())
			{
				int32 NewIndex = Helper.AddDefaultValue_Invalid_NeedsRehash();

				TSharedPtr<FJsonValueString> TempKeyValue = MakeShareable(new FJsonValueString(Entry.Key));

				const bool bKeySuccess = JsonValueToFProperty(TempKeyValue, MapProperty->KeyProp, Helper.GetKeyPtr(NewIndex));
				const bool bValueSuccess = JsonValueToFProperty(Entry.Value, MapProperty->ValueProp, Helper.GetValuePtr(NewIndex));

				if (!(bKeySuccess && bValueSuccess))
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable to deserialize map element [key: %s] for property %s"), *Entry.Key, *MapProperty->GetNameCPP());
					return false;
				}
			}
		}

		Helper.Rehash();
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import TMap from non-object JSON key for property %s"), *MapProperty->GetNameCPP());
		return false;
	}

	return true;
}

bool UODJsonDeserializer::JsonValueToFSetProperty(const TSharedPtr<FJsonValue>& JsonValue, FSetProperty* SetProperty, void* OutValue)
{
	if (JsonValue->Type == EJson::Array)
	{
		TArray< TSharedPtr<FJsonValue> > ArrayValue = JsonValue->AsArray();
		int32 ArrLen = ArrayValue.Num();

		FScriptSetHelper Helper(SetProperty, OutValue);

		// set the property values
		for (int32 i = 0; i < ArrLen; ++i)
		{
			const TSharedPtr<FJsonValue>& ArrayValueItem = ArrayValue[i];
			if (ArrayValueItem.IsValid() && !ArrayValueItem->IsNull())
			{
				int32 NewIndex = Helper.AddDefaultValue_Invalid_NeedsRehash();
				if (!JsonValueToFProperty(ArrayValueItem, SetProperty->ElementProp, Helper.GetElementPtr(NewIndex)))
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable to deserialize set element [%d] for property %s"), i, *SetProperty->GetNameCPP());
					return false;
				}
			}
		}

		Helper.Rehash();
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import TSet from non-array JSON key for property %s"), *SetProperty->GetNameCPP());
		return false;
	}

	return true;
}

bool UODJsonDeserializer::JsonValueToFTextProperty(const TSharedPtr<FJsonValue>& JsonValue, FTextProperty* TextProperty, void* OutValue)
{
	if (JsonValue->Type == EJson::String)
	{
		// assume this string is already localized, so import as invariant
		TextProperty->SetPropertyValue(OutValue, FText::FromString(JsonValue->AsString()));
	}
	else if (JsonValue->Type == EJson::Object)
	{
		TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();
		check(Obj.IsValid()); // should not fail if Type == EJson::Object

							  // import the subvalue as a culture invariant string
		FText Text;
		if (!FJsonObjectConverter::GetTextFromObject(Obj.ToSharedRef(), Text))
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import FText from JSON object with invalid keys for property %s"), *TextProperty->GetNameCPP());
			return false;
		}
		TextProperty->SetPropertyValue(OutValue, Text);
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import FText from JSON that was neither string nor object for property %s"), *TextProperty->GetNameCPP());
		return false;
	}

	return true;
}

bool UODJsonDeserializer::JsonValueToFStructProperty(const TSharedPtr<FJsonValue>& JsonValue, FStructProperty* StructProperty, void* OutValue)
{
	static const FName NAME_DateTime(TEXT("DateTime"));
	static const FName NAME_ColorProperty(TEXT("Color"));
	static const FName NAME_LinearColorProperty(TEXT("LinearColor"));

	if (JsonValue->Type == EJson::Object)
	{
		TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();
		check(Obj.IsValid()); // should not fail if Type == EJson::Object
		if (!JsonObjectToUStruct(Obj, StructProperty->Struct, OutValue))
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - FJsonObjectConverter::JsonObjectToUStruct failed for property %s"), *StructProperty->GetNameCPP());
			return false;
		}
	}
	else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetFName() == NAME_LinearColorProperty)
	{
		FLinearColor& ColorOut = *(FLinearColor*)OutValue;
		FString ColorString = JsonValue->AsString();

		FColor IntermediateColor;
		IntermediateColor = FColor::FromHex(ColorString);

		ColorOut = IntermediateColor;
	}
	else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetFName() == NAME_ColorProperty)
	{
		FColor& ColorOut = *(FColor*)OutValue;
		FString ColorString = JsonValue->AsString();

		ColorOut = FColor::FromHex(ColorString);
	}
	else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetFName() == NAME_DateTime)
	{
		FString DateString = JsonValue->AsString();
		FDateTime& DateTimeOut = *(FDateTime*)OutValue;
		if (DateString == TEXT("min"))
		{
			// min representable value for our date struct. Actual date may vary by platform (this is used for sorting)
			DateTimeOut = FDateTime::MinValue();
		}
		else if (DateString == TEXT("max"))
		{
			// max representable value for our date struct. Actual date may vary by platform (this is used for sorting)
			DateTimeOut = FDateTime::MaxValue();
		}
		else if (DateString == TEXT("now"))
		{
			// this value's not really meaningful from json serialization (since we don't know timezone) but handle it anyway since we're handling the other keywords
			DateTimeOut = FDateTime::UtcNow();
		}
		else if (FDateTime::ParseIso8601(*DateString, DateTimeOut))
		{
			// ok
		}
		else if (FDateTime::Parse(DateString, DateTimeOut))
		{
			// ok
		}
		else
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable to import FDateTime for property %s"), *StructProperty->GetNameCPP());
			return false;
		}
	}
	else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetCppStructOps() && StructProperty->Struct->GetCppStructOps()->HasImportTextItem())
	{
		UScriptStruct::ICppStructOps* TheCppStructOps = StructProperty->Struct->GetCppStructOps();

		FString ImportTextString = JsonValue->AsString();
		const TCHAR* ImportTextPtr = *ImportTextString;
		if (!TheCppStructOps->ImportTextItem(ImportTextPtr, OutValue, PPF_None, nullptr, (FOutputDevice*)GWarn))
		{
			// Fall back to trying the tagged property approach if custom ImportTextItem couldn't get it done
#if CHECK_UE_VERSION_ORUNDER_5_0
			StructProperty->ImportText(ImportTextPtr, OutValue, PPF_None, nullptr);
#else
			StructProperty->ImportText_Direct(ImportTextPtr, OutValue, nullptr, PPF_None);
#endif
		}
	}
	else if (JsonValue->Type == EJson::String)
	{
		FString ImportTextString = JsonValue->AsString();
		const TCHAR* ImportTextPtr = *ImportTextString;
#if CHECK_UE_VERSION_ORUNDER_5_0
		StructProperty->ImportText(ImportTextPtr, OutValue, PPF_None, nullptr);
#else
		StructProperty->ImportText_Direct(ImportTextPtr, OutValue, nullptr, PPF_None);
#endif
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import UStruct from non-object JSON key for property %s"), *StructProperty->GetNameCPP());
		return false;
	}

	return true;
}

bool UODJsonDeserializer::JsonValueToFObjectProperty(const TSharedPtr<FJsonValue>& JsonValue, FObjectProperty* ObjectProperty, void* OutValue)
{
	if (JsonValue->Type == EJson::Object)
	{
		TSharedPtr<FJsonObject> jsonObj = JsonValue->AsObject();

		if (jsonObj.IsValid())
		{
			UObject* createdObj = JsonObjectToUObject(jsonObj, ObjectProperty->PropertyClass);
			if (IsValid(createdObj))
			{
				ObjectProperty->SetObjectPropertyValue(OutValue, createdObj);
			}
		}
	}
	else if (JsonValue->Type == EJson::String)
	{
		// Default to expect a string for everything else
#if CHECK_UE_VERSION_ORUNDER_5_0
		if (ObjectProperty->ImportText(*JsonValue->AsString(), OutValue, 0, NULL) == NULL)
#else
		if (ObjectProperty->ImportText_Direct(*JsonValue->AsString(), OutValue, NULL, 0) == NULL)
#endif
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable import property type %s from string value for property %s"), *ObjectProperty->GetClass()->GetName(), *ObjectProperty->GetNameCPP());
			return false;
		}
	}

	return true;
}