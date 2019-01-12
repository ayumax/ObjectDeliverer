#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "Sockets.h"

#include "CNTcpIpServer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTcpIpServerConnected, class UCNTcpIpSocket*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTcpIpServerDisconnected, class UCNTcpIpSocket*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTcpIpServerReceiveData, class UCNTcpIpSocket*, ClientSocket, const TArray<uint8>&, Buffer, int32, Size);

class UCNTcpIpSocket;

UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UCNTcpIpServer : public UObject
{
	GENERATED_BODY()

public:
	UCNTcpIpServer();
	~UCNTcpIpServer();
 
	UFUNCTION(BlueprintCallable, Category = "CommNet")
		bool Start(int32 Port);

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		void Close();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		void Send(const TArray<uint8>& DataBuffer);

	virtual void BeginDestroy() override;

private:
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

private:
	FSocket* ListenerSocket = nullptr;
	class FRunnableThread* ListenThread = nullptr;

	TArray<UCNTcpIpSocket*> ConnectedSockets;
};
