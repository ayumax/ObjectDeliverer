// Copyright 2019 ayumax. All Rights Reserved.
#include "ODObjectUtil.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "UObject/EnumProperty.h"
#include "UObject/TextProperty.h"
#include "UObject/PropertyPortFlags.h"
#include "UObject/Package.h"
#include "UObject/ConstructorHelpers.h"
#include "LogObjectDeliverer.h"

void UODObjectUtil::EnumProperties(UObject* TargetObject, TFunction<bool(FProperty*)> EnumFunc)
{
	for (TFieldIterator<FProperty> PropIt(TargetObject->GetClass()); PropIt; ++PropIt)
	{
		if (!EnumFunc(*PropIt))
		{
			break;
		}
	}
}

bool UODObjectUtil::FindClass(const FString& ClassName, UClass*& Class)
{
	// Method 1: Current method using FSoftClassPath
	UClass* LoadedClass = FSoftClassPath(ClassName).TryLoadClass<UObject>();
	
	// Method 2: Use FindFirstObjectSafe to directly search for the class
	if (!LoadedClass)
	{
		LoadedClass = FindFirstObjectSafe<UClass>(*ClassName);
	}
	
	// Method 3: Use StaticLoadObject to load the class
	if (!LoadedClass)
	{
		LoadedClass = StaticLoadClass(UObject::StaticClass(), nullptr, *ClassName);
	}

	bool bFound = false;
	if (LoadedClass)
	{
		bFound = true;
		Class = LoadedClass;
	}	
	else
	{
		UE_LOG(LogObjectDeliverer, Error, TEXT("Can't find class: %s"), *ClassName);
	}
	
	return bFound;
}
