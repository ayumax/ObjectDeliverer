#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "Sockets.h"
#include "CommNetProtocol.h"
#include "CNTcpIpServer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTcpIpServerConnected, class UCNTcpIpSocket*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTcpIpServerDisconnected, class UCNTcpIpSocket*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTcpIpServerReceiveData, class UCNTcpIpSocket*, ClientSocket, const TArray<uint8>&, Buffer, int32, Size);

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

	virtual void BeginDestroy() override;

protected:
	void OnListen();

	UFUNCTION()
	void DisconnectedClient(UCNTcpIpSocket* ClientSocket);

	UFUNCTION()
	void ReceiveDataFromClient(UCNTcpIpSocket* ClientSocket, const TArray<uint8>& Buffer, int32 Size);

public:
	UPROPERTY(BlueprintAssignable, Category = "CommNet")
	FTcpIpServerConnected Connected;
	UPROPERTY(BlueprintAssignable, Category = "CommNet")
	FTcpIpServerDisconnected Disconnected;
	UPROPERTY(BlueprintAssignable, Category = "CommNet")
	FTcpIpServerReceiveData ReceiveData;

	UPROPERTY(EditAnywhere, Category = "CommNet")
	int32 ListenPort;

	UPROPERTY(EditAnywhere, Category = "CommNet")
	int32 MaxBacklog = 10;

protected:
	FSocket* ListenerSocket = nullptr;
	class FWorkerThread ListenInnerThread = nullptr;
	class FRunnableThread* ListenThread = nullptr;

	TArray<UCNTcpIpSocket*> ConnectedSockets;
};
