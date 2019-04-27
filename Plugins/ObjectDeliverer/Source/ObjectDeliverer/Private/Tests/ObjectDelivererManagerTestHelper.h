// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "PacketRule/PacketRuleFactory.h"
#include "Protocol/ProtocolFactory.h"
#include "DeliveryBoxFactory.h"
#include "UObject/GCObject.h"
#include "ObjectDelivererManagerTestHelper.generated.h"

class UObjectDelivererProtocol;

UCLASS()
class OBJECTDELIVERER_API UObjectDelivererManagerTestHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnConnect(const UObjectDelivererProtocol* ClientSocket);
	UFUNCTION()
	void OnDisConnect(const UObjectDelivererProtocol* ClientSocket);
	UFUNCTION()
	void OnReceive(const UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer);
	UFUNCTION()
	void OnReceiveString(const FString& StringValue, const UObjectDelivererProtocol* FromObject);

	UPROPERTY()
	TArray<const UObjectDelivererProtocol*> ConnectedSocket;

	UPROPERTY()
	TArray<const UObjectDelivererProtocol*> DisconnectedSocket;

	UPROPERTY()
	const UObjectDelivererProtocol* ReceiveSocket;

	TArray<TArray<uint8>> ReceiveBuffers;
	TArray<FString> ReceiveStrings;
};