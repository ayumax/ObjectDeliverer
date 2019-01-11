#pragma once

#include "CoreMinimal.h"

#include "TcpIpServer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTcpIpServerConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTcpIpServerDisconnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTcpIpServerReceiveData, const TArray<uint8>&, Buffer, int32, Size);


class UTcpIpSocket;

UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UTcpIpServer : public UObject
{
	GENERATED_BODY()
public:

	UTcpIpServer();
	~UTcpIpServer();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		bool Start(int32 Port);

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		void Close();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		void Send(const TArray<uint8>& DataBuffer);


public:
	FTcpIpServerConnected Connected;
	FTcpIpServerDisconnected Disconnected;
	FTcpIpServerReceiveData ReceiveData;

private:
	TArray<UTcpIpSocket*> ConnectedSockets;
};
