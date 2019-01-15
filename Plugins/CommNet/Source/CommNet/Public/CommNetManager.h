#pragma once

#include "CoreMinimal.h"
#include "CommNetManager.generated.h"

class UCommNetProtocol;
class UCNPacketRule;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCommNetManagerConnected, UCommNetProtocol*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCommNetManagerDisconnected, UCommNetProtocol*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCommNetManagerReceiveData, UCommNetProtocol*, ClientSocket, const TArray<uint8>&, Buffer);

UCLASS(BlueprintType)
class COMMNET_API UCommNetManager : public UObject
{
	GENERATED_BODY()

public:
	UCommNetManager();
	~UCommNetManager();

	/**
	 * start communication protocol.
	 */
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void Start(UCommNetProtocol* Protocol, UCNPacketRule* PacketRule);

	/**
	 * close communication protocol.
	 */
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void Close();

	/**
	 * send the data to the connection destination.
	 * @param DataBuffer - databuffer
	 * @param DataSize - Size to send in databuffer(0:all bytes in databuffer)
	 */
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void Send(const TArray<uint8>& DataBuffer);


	UPROPERTY(BlueprintAssignable, Category = "CommNet")
	FCommNetManagerConnected Connected;

	UPROPERTY(BlueprintAssignable, Category = "CommNet")
	FCommNetManagerDisconnected Disconnected;

	UPROPERTY(BlueprintAssignable, Category = "CommNet")
	FCommNetManagerReceiveData ReceiveData;

	virtual void BeginDestroy() override;

private:
	UPROPERTY()
	UCommNetProtocol* CurrentProtocol;
};

UCLASS()
class COMMNET_API UCommNetManagerTestHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnConnect(UCommNetProtocol* ClientSocket)
	{
		ConnectedSocket.Add(ClientSocket);
	}
	UFUNCTION()
	void OnDisConnect(UCommNetProtocol* ClientSocket)
	{
		DisconnectedSocket.Add(ClientSocket);
	}
	UFUNCTION()
	void OnReceive(UCommNetProtocol* ClientSocket, const TArray<uint8>& Buffer)
	{
		ReceiveSocket = ClientSocket;
		ReceiveBuffer = Buffer;
	}

	UPROPERTY()
	TArray<UCommNetProtocol*> ConnectedSocket;

	UPROPERTY()
	TArray<UCommNetProtocol*> DisconnectedSocket;

	UPROPERTY()
	UCommNetProtocol* ReceiveSocket;

	UPROPERTY()
	TArray<uint8> ReceiveBuffer;

};