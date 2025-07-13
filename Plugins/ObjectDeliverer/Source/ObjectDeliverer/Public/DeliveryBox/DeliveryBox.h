// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "DeliveryDataType.h"
#include "DeliveryBox.generated.h"

class UObjectDelivererProtocol;

/**
 * const UObjectDelivererProtocol* : Destination protocol
 * const TArray<uint8>& : Data
 * const FDeliveryDataType& : Data type
 */
DECLARE_DELEGATE_ThreeParams(FUDeliveryBoxRequestSend, const UObjectDelivererProtocol*, const TArray<uint8>&, const FDeliveryDataType&);

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
