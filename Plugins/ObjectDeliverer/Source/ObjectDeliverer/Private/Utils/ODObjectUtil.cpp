// Copyright 2019 ayumax. All Rights Reserved.
#include "ODObjectUtil.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "UObject/EnumProperty.h"
#include "UObject/TextProperty.h"
#include "UObject/PropertyPortFlags.h"
#include "Package.h"

EUPropertyType UODObjectUtil::GetUPropertyType(UProperty* Property)
{
	UClass* Class = Property->GetClass();

	if (Class->IsChildOf(UBoolProperty::StaticClass()))
	{
		return EUPropertyType::Bool;
	}
	else if (Class->IsChildOf(UIntProperty::StaticClass()))
	{
		return EUPropertyType::Int;
	}
	else if (Class->IsChildOf(UFloatProperty::StaticClass()))
	{
		return EUPropertyType::Float;
	}
	else if (Class->IsChildOf(UDoubleProperty::StaticClass()))
	{
		return EUPropertyType::Double;
	}
	else if (Class->IsChildOf(UByteProperty::StaticClass()))
	{
		UByteProperty* ByteProp = static_cast<UByteProperty*>(Property);
		if (ByteProp->Enum)
		{
			return EUPropertyType::Enum;
		}
		else
		{
			return EUPropertyType::UInt8;
		}
	}
		
	else if (Class->IsChildOf(UEnumProperty::StaticClass()))
	{
		return EUPropertyType::Enum;
	}
	else if (Class->IsChildOf(UStrProperty::StaticClass()))
	{
		return EUPropertyType::String;
	}
	else if (Class->IsChildOf(UArrayProperty::StaticClass()))
	{
		return EUPropertyType::Array;
	}
	else if (Class->IsChildOf(UMapProperty::StaticClass()))
	{
		return EUPropertyType::Map;
	}
	else if (Class->IsChildOf(USetProperty::StaticClass()))
	{
		return EUPropertyType::Set;
	}
	else if (Class->IsChildOf(UStructProperty::StaticClass()))
	{
		return EUPropertyType::Struct;
	}
	else if (Class->IsChildOf(UObjectPropertyBase::StaticClass()))
	{
		return EUPropertyType::Object;
	}

	return EUPropertyType::None;
}

void UODObjectUtil::EnumProperties(UObject* TargetObject, TFunction<bool(UProperty*)> EnumFunc)
{
	for (TFieldIterator<UProperty> PropIt(TargetObject->GetClass()); PropIt; ++PropIt)
	{
		if (!EnumFunc(*PropIt))
		{
			break;
		}
	}
}
bool UODObjectUtil::FindClass(const FString& ClassName, UClass*& Class)
{
	Class = FindObject<UClass>(ANY_PACKAGE, *ClassName);
	return Class != nullptr;
}
