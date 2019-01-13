#pragma once

#include "CoreMinimal.h"
#include "CNSocketBase.h"
#include "CNUdpSocket.generated.h"

UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UCNUdpSocket : public UCNSocketBase
{
	GENERATED_BODY()

public:
	UCNUdpSocket();
	~UCNUdpSocket();

	/**
	 * Initialize UDP.
	 * @param IpAddress - The ip address of the destination.
	 * @param Port - The port number of the destination.
	 */
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	virtual void Initialize(const FString& IpAddress = "localhost", int32 Port = 8000);

	virtual void Start_Implementation() override;
	virtual void Close_Implementation() override;
	virtual void Send_Implementation(const TArray<uint8>& DataBuffer) override;


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	FString DestinationIpAddress = "localhost";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	int32 DestinationPort = 8000;

protected:
	FIPv4Endpoint DestinationEndpoint;
};
