#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "Sockets.h"
#include "CommNetProtocol.h"
#include "CNTcpIpServer.generated.h"


class UCNTcpIpSocket;

UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UCNTcpIpServer : public UCommNetProtocol
{
	GENERATED_BODY()

public:
	UCNTcpIpServer();
	~UCNTcpIpServer();
 
	/**
	 * Initialize TCP/IP server.
	 * @param Port - listen port number.
	 */
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void Initialize(int32 Port);

	virtual void Start_Implementation() override;
	virtual void Close_Implementation() override;
	virtual void Send_Implementation(const TArray<uint8>& DataBuffer) override;

protected:
	void OnListen();

	UFUNCTION()
	void DisconnectedClient(UCommNetProtocol* ClientSocket);

	UFUNCTION()
	void ReceiveDataFromClient(UCommNetProtocol* ClientSocket, const TArray<uint8>& Buffer, int32 Size);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	int32 ListenPort = 8000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CommNet")
	int32 MaxBacklog = 1;

protected:
	FSocket* ListenerSocket = nullptr;
	class FWorkerThread* ListenInnerThread = nullptr;
	class FRunnableThread* ListenThread = nullptr;

	TArray<UCNTcpIpSocket*> ConnectedSockets;
};
