// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ODJsonSerializerBase.generated.h"

class UODOverrideJsonSerializer;

UCLASS()
class OBJECTDELIVERER_API UODJsonSerializerBase : public UObject
{
	GENERATED_BODY()

public:
	UODJsonSerializerBase();

	void AddOverrideJsonSerializers(TSubclassOf<UODOverrideJsonSerializer> DefaultObjectSerializerClass, const TMap<UClass*, TSubclassOf<UODOverrideJsonSerializer>>& OverrideObjectSerializerClasses);


protected:
	UPROPERTY(Transient)
	UODOverrideJsonSerializer* DefaultSerializer;

	UPROPERTY(Transient)
	TMap<UClass*, UODOverrideJsonSerializer*> ObjectSerializeres;
};
