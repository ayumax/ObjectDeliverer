#pragma once

#include "CoreMinimal.h"
#include "CNTcpIpSocket.h"
#include "TimerManager.h"
#include "CNTcpIpClient.generated.h"


UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UCNTcpIpClient : public UCNTcpIpSocket
{
	GENERATED_BODY()

public:
	UCNTcpIpClient();
	~UCNTcpIpClient();

	/**
	 * Initialize TCP/IP server.
	 * @param IpAddress - The ip address of the connection destination.
	 * @param Port - The port number of the connection destination.
	 * @param Retry - If connection fails, try connection again
	 */
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void Initialize(const FString& IpAddress = "localhost", int32 Port = 8000, bool Retry = false);

	virtual void Start_Implementation() override;
	virtual void Close_Implementation() override;

private:
	UFUNCTION()
	void TryConnect();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	FString ServerIpAddress = "localhost";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	int32 ServerPort = 8000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	bool RetryConnect = false;

private:
	FTimerHandle ConnectTimerHandle;
	FIPv4Endpoint ConnectEndPoint;
};
