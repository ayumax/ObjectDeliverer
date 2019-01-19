#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
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

	virtual void Start_Implementation() override;
	virtual void Close_Implementation() override;
	virtual void Send_Implementation(const TArray<uint8>& DataBuffer) override;

protected:
	bool OnListen();

	UFUNCTION()
	void DisconnectedClient(UObjectDelivererProtocol* ClientSocket);

	UFUNCTION()
	void ReceiveDataFromClient(UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 ListenPort = 8000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ObjectDeliverer|Protocol")
	int32 MaxBacklog = 10;

protected:
	FSocket* ListenerSocket = nullptr;
	class FWorkerThread* ListenInnerThread = nullptr;
	class FRunnableThread* ListenThread = nullptr;

	TArray<UProtocolTcpIpSocket*> ConnectedSockets;
};
