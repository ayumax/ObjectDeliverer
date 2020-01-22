// Copyright 2019 ayumax. All Rights Reserved.
#include "ODJsonSerializerBase.h"

UODJsonSerializerBase::UODJsonSerializerBase()
{
	DefaultSerializer = CreateDefaultSubobject<UODNoWriteTypeJsonSerializer>(TEXT("DefaultObjectSerializer"));
}

void UODJsonSerializerBase::AddOverrideJsonSerializers(EODJsonSerializeType DefaultSerializerType, const TMap<UClass*, EODJsonSerializeType>& ObjectSerializerTypes)
{
	DefaultSerializer = CreateOverrideJsonSerializer(DefaultSerializerType);

	TArray<UClass*> classKeyes;
	ObjectSerializerTypes.GetKeys(classKeyes);

	for (auto classKey : classKeyes)
	{
		ObjectSerializeres.Add(classKey, CreateOverrideJsonSerializer(ObjectSerializerTypes[classKey]));
	}
	
}

UODOverrideJsonSerializer* UODJsonSerializerBase::CreateOverrideJsonSerializer(EODJsonSerializeType jsonSerializeType)
{
	UClass* serializeClass = nullptr;

	switch (jsonSerializeType)
	{
	case EODJsonSerializeType::NoWriteType:
		serializeClass = UODNoWriteTypeJsonSerializer::StaticClass();
		break;
	case EODJsonSerializeType::WriteType:
		serializeClass = UODWriteTypeJsonSerializer::StaticClass();
		break;
	default:
		serializeClass = UODNoWriteTypeJsonSerializer::StaticClass();
		break;
	}


	return NewObject<UODOverrideJsonSerializer>(this, serializeClass);
}