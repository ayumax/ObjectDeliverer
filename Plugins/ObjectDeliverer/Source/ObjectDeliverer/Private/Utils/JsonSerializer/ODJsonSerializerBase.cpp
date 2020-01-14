// Copyright 2019 ayumax. All Rights Reserved.
#include "ODJsonSerializerBase.h"
#include "ODOverrideJsonSerializer.h"

UODJsonSerializerBase::UODJsonSerializerBase()
{
	DefaultSerializer = CreateDefaultSubobject<UODNoWriteTypeJsonSerializer>(TEXT("DefaultObjectSerializer"));
}

void UODJsonSerializerBase::AddOverrideJsonSerializers(TSubclassOf<UODOverrideJsonSerializer> DefaultObjectSerializerClass, const TMap<UClass*, TSubclassOf<UODOverrideJsonSerializer>>& OverrideObjectSerializerClasses)
{
	DefaultSerializer = NewObject<UODOverrideJsonSerializer>(this, DefaultObjectSerializerClass);

	TArray<UClass*> classKeyes;
	OverrideObjectSerializerClasses.GetKeys(classKeyes);

	for (auto classKey : classKeyes)
	{
		ObjectSerializeres.Add(classKey, NewObject<UODOverrideJsonSerializer>(this, OverrideObjectSerializerClasses[classKey]));
	}
	
}
