// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ProtocolSocketBase.h"
#include "ProtocolUdpSocketReceiver.generated.h"


class FSocket;
class FUdpSocketReceiver;

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolUdpSocketReceiver : public UProtocolSocketBase
{
	GENERATED_BODY()

public:
	UProtocolUdpSocketReceiver();
	~UProtocolUdpSocketReceiver();

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	void InitializeWithReceiver(int32 BoundPort = 8000);

	virtual void Start() override;
	virtual void Close() override;

protected:
	void UdpReceivedCallback(const FArrayReaderPtr& data, const FIPv4Endpoint& ip);

private:
	FUdpSocketReceiver* Receiver = nullptr;
	FCriticalSection ct;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 BoundPort = 8000;
};
