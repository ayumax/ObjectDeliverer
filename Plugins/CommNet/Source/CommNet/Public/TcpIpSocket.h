#pragma once

#include "CoreMinimal.h"

#include "TcpIpSocket.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTcpIpSocketConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTcpIpSocketDisconnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTcpIpSocketReceiveData, const TArray<uint8>&, Buffer, int32, Size);



UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UTcpIpSocket : public UObject
{
	GENERATED_BODY()
public:

	UTcpIpSocket();
	~UTcpIpSocket();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		bool Connect(const FString& IpAddress, int32 Port);

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		void Close();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		void Send(const TArray<uint8>& DataBuffer);

private:
	bool FormatIP4ToNumber(const FString& IpAddress, uint8(&Out)[4]);
	void StartPollilng();
	void ReceivedData();
	uint32 WantSize();
	void OnReceivedSize();
	void OnReceivedBody();

private:
	class FSocket* ConnectedSocket = nullptr;
	class FSocketPollingThread* PollingThread = nullptr;
	class FRunnableThread* CurrentThread = nullptr;

	enum EReceiveMode
	{
		Size,
		Body
	} ReceiveMode;
	
	TArray<uint8> ReceiveBuffer;
	uint32 BodySize;

public:
	FTcpIpSocketConnected Connected;
	FTcpIpSocketDisconnected Disconnected;
	FTcpIpSocketReceiveData ReceiveData;
};
