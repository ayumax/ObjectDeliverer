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
	UClass* LoadedClass = FSoftClassPath(ClassName).TryLoadClass<UObject>();

	bool bFound = false;
	if (LoadedClass)
	{
		LoadedClass->AddToRoot();
		bFound = true;
		Class = LoadedClass;
	}	
	if (!bFound)
	{
		UE_LOG(LogObjectDeliverer, Error, TEXT("Can't find class."));
	}
	
	return bFound;
}
