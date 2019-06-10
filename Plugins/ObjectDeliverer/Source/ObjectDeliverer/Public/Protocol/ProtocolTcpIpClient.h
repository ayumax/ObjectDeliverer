// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ProtocolTcpIpSocket.h"
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
	void Initialize(const FString& IpAddress = "localhost", int32 Port = 8000, bool Retry = false, bool AutoConnectAfterDisconnect = false);

	virtual void Start() override;
	virtual void Close() override;

private:
	bool TryConnect();

protected:
	virtual void DispatchDisconnected(const UObjectDelivererProtocol* DisconnectedObject) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	FString ServerIpAddress = "localhost";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 ServerPort = 8000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	bool RetryConnect = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	bool AutoConnectAfterDisconnect = false;

protected:
	class FWorkerThread* ConnectInnerThread = nullptr;
	class FRunnableThread* ConnectThread = nullptr;

	FIPv4Endpoint ConnectEndPoint;
};
