#pragma once

#include "CoreMinimal.h"
#include "ProtocolTcpIpSocket.h"
#include "TimerManager.h"
#include "ProtocolTcpIpClient.generated.h"


UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolTcpIpClient : public UProtocolTcpIpSocket
{
	GENERATED_BODY()

public:
	UProtocolTcpIpClient();
	~UProtocolTcpIpClient();

	/**
	 * Initialize TCP/IP server.
	 * @param IpAddress - The ip address of the connection destination.
	 * @param Port - The port number of the connection destination.
	 * @param Retry - If connection fails, try connection again
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	void Initialize(const FString& IpAddress = "localhost", int32 Port = 8000, bool Retry = false);

	virtual void Start_Implementation() override;
	virtual void Close_Implementation() override;

private:
	UFUNCTION()
	void TryConnect();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	FString ServerIpAddress = "localhost";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 ServerPort = 8000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	bool RetryConnect = false;

private:
	FTimerHandle ConnectTimerHandle;
	FIPv4Endpoint ConnectEndPoint;
};
