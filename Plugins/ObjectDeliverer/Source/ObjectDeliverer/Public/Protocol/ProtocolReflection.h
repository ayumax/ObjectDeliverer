// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ObjectDelivererProtocol.h"
#include "ProtocolReflection.generated.h"


UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolReflection : public UObjectDelivererProtocol
{
	GENERATED_BODY()

public:
	UProtocolReflection();
	~UProtocolReflection();

	virtual void Send(const TArray<uint8>& DataBuffer) const override;

	virtual void RequestSend(const TArray<uint8>& DataBuffer) override;
};
