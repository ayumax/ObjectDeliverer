// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "DeliveryBox.generated.h"

class UObjectDelivererProtocol;

DECLARE_DELEGATE_TwoParams(FUDeliveryBoxRequestSend, const UObjectDelivererProtocol*, const TArray<uint8>&);

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UDeliveryBox : public UObject
{
	GENERATED_BODY()

public:
	UDeliveryBox();
	~UDeliveryBox();

	virtual void NotifyReceiveBuffer(const UObjectDelivererProtocol* FromObject, const TArray<uint8>& buffer);

	FUDeliveryBoxRequestSend RequestSend;

};
