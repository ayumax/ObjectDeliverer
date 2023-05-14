// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Sockets.h"
#include "ObjectDelivererProtocol.h"
#include "ProtocolSocketBase.generated.h"

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolSocketBase : public UObjectDelivererProtocol
{
	GENERATED_BODY()

public:
	UProtocolSocketBase();
	~UProtocolSocketBase();

	void CloseInnerSocket();

	void SendTo(const TArray<uint8>& DataBuffer, const FIPv4Endpoint& EndPoint) const;

	void SendToConnected(const TArray<uint8>& DataBuffer) const;

protected:
	bool FormatIP4ToNumber(const FString& IpAddress, uint8(&Out)[4]);
	TTuple<bool, FIPv4Endpoint> GetIP4EndPoint(const FString& IpAddress, int32 Port);

protected:
	FSocket* InnerSocket = nullptr;
};
