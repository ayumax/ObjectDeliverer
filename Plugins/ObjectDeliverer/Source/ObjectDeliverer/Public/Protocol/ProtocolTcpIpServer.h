// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "ObjectDelivererProtocol.h"
#include "ProtocolTcpIpServer.generated.h"


class UProtocolTcpIpSocket;

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolTcpIpServer : public UObjectDelivererProtocol
{
	GENERATED_BODY()

public:
	UProtocolTcpIpServer();
	~UProtocolTcpIpServer();
 
	/**
	 * Initialize TCP/IP server.
	 * @param Port - listen port number.
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	void Initialize(int32 Port);

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	UProtocolTcpIpServer* WithReceiveBufferSize(int32 SizeInBytes);

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	UProtocolTcpIpServer* WithSendBufferSize(int32 SizeInBytes);

	virtual void Start() override;
	virtual void Close() override;
	virtual void Send(const TArray<uint8>& DataBuffer) const override;

protected:
	bool OnListen();

	UFUNCTION()
	void DisconnectedClient(const UObjectDelivererProtocol* ClientSocket);

	UFUNCTION()
	void ReceiveDataFromClient(const UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 ListenPort = 8000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ObjectDeliverer|Protocol")
	int32 MaxBacklog = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 ReceiveBufferSize = 1024 * 1024;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 SendBufferSize = 1024 * 1024;

protected:
	FSocket* ListenerSocket = nullptr;
	class FODWorkerThread* ListenInnerThread = nullptr;
	class FRunnableThread* ListenThread = nullptr;

	TArray<UProtocolTcpIpSocket*> ConnectedSockets;
};
