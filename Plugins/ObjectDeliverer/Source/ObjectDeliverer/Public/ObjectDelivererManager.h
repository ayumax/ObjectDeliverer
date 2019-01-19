#pragma once

#include "CoreMinimal.h"
#include "ObjectDelivererManager.generated.h"

class UObjectDelivererProtocol;
class UPacketRule;
class UDeliveryBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectDelivererManagerConnected, UObjectDelivererProtocol*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectDelivererManagerDisconnected, UObjectDelivererProtocol*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FObjectDelivererManagerReceiveData, UObjectDelivererProtocol*, ClientSocket, const TArray<uint8>&, Buffer);

UCLASS(BlueprintType)
class OBJECTDELIVERER_API UObjectDelivererManager : public UObject
{
	GENERATED_BODY()

public:
	UObjectDelivererManager();
	~UObjectDelivererManager();

	/**
	 * start communication protocol.
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer")
	void Start(UObjectDelivererProtocol* Protocol, UPacketRule* PacketRule, UDeliveryBox* DeliveryBox = nullptr);

	/**
	 * close communication protocol.
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer")
	void Close();

	/**
	 * send the data to the connection destination.
	 * @param DataBuffer - databuffer
	 * @param DataSize - Size to send in databuffer(0:all bytes in databuffer)
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer")
	void Send(const TArray<uint8>& DataBuffer);


	UPROPERTY(BlueprintAssignable, Category = "ObjectDeliverer")
	FObjectDelivererManagerConnected Connected;

	UPROPERTY(BlueprintAssignable, Category = "ObjectDeliverer")
	FObjectDelivererManagerDisconnected Disconnected;

	UPROPERTY(BlueprintAssignable, Category = "ObjectDeliverer")
	FObjectDelivererManagerReceiveData ReceiveData;

	virtual void BeginDestroy() override;

private:
	UPROPERTY()
	UObjectDelivererProtocol* CurrentProtocol;
	UPROPERTY()
	UDeliveryBox* DeliveryBox;
};

UCLASS()
class OBJECTDELIVERER_API UObjectDelivererManagerTestHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnConnect(UObjectDelivererProtocol* ClientSocket)
	{
		ConnectedSocket.Add(ClientSocket);
	}
	UFUNCTION()
	void OnDisConnect(UObjectDelivererProtocol* ClientSocket)
	{
		DisconnectedSocket.Add(ClientSocket);
	}
	UFUNCTION()
	void OnReceive(UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
	{
		ReceiveSocket = ClientSocket;
		ReceiveBuffers.Add(Buffer);
	}

	UPROPERTY()
	TArray<UObjectDelivererProtocol*> ConnectedSocket;

	UPROPERTY()
	TArray<UObjectDelivererProtocol*> DisconnectedSocket;

	UPROPERTY()
	UObjectDelivererProtocol* ReceiveSocket;

	TArray<TArray<uint8>> ReceiveBuffers;

};