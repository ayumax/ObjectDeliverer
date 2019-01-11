#pragma once

#include "CoreMinimal.h"

#include "TcpIpSocket.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTcpIpSocketDisconnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTcpIpSocketReceiveData, const TArray<uint8>&, Buffer, int32, Size);


class FSocket;

UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UTcpIpSocket : public UObject
{
	GENERATED_BODY()
public:

	UTcpIpSocket();
	~UTcpIpSocket();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		void Close(bool Wait = true);

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		void Send(const TArray<uint8>& DataBuffer);

protected:
	void OnConnected(FSocket* ConnectionSocket);
	void StartPollilng();
	void ReceivedData();
	uint32 WantSize();
	void OnReceivedSize();
	void OnReceivedBody();

protected:
	FSocket* ConnectedSocket = nullptr;
	class FWorkerThread* PollingThread = nullptr;
	class FRunnableThread* CurrentThread = nullptr;

	enum EReceiveMode
	{
		Size,
		Body
	} ReceiveMode;
	
	TArray<uint8> ReceiveBuffer;
	uint32 BodySize;

public:
	FTcpIpSocketDisconnected Disconnected;
	FTcpIpSocketReceiveData ReceiveData;
};
