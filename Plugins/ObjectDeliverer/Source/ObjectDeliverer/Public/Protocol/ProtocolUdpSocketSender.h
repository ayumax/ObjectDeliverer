// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ProtocolSocketBase.h"
#include "ProtocolUdpSocketSender.generated.h"

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolUdpSocketSender : public UProtocolSocketBase
{
	GENERATED_BODY()

public:
	UProtocolUdpSocketSender();
	~UProtocolUdpSocketSender();

	/**
	 * Initialize UDP.
	 * @param IpAddress - The ip address of the destination.
	 * @param Port - The port number of the destination.
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	virtual void Initialize(const FString &IpAddress = "localhost", int32 Port = 8000);

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	UProtocolUdpSocketSender *WithSendBufferSize(int32 SizeInBytes);

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	UProtocolUdpSocketSender *WithBroadcast(bool InEnableBroadcast);

	virtual void Start() override;
	virtual void Close() override;
	virtual void Send(const TArray<uint8> &DataBuffer, const FDeliveryDataType& KindOfData) const override;

	virtual void RequestSend(const TArray<uint8> &DataBuffer, const FDeliveryDataType& DataType) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	FString DestinationIpAddress = "localhost";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 DestinationPort = 8000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 SendBufferSize = 1024 * 1024;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	bool bEnableBroadcast = false;

protected:
	FIPv4Endpoint DestinationEndpoint;
};
