#pragma once

#include "CoreMinimal.h"
#include "ObjectDelivererProtocol.generated.h"

DECLARE_DELEGATE_OneParam(FObjectDelivererProtocolConnected, UObjectDelivererProtocol*);
DECLARE_DELEGATE_OneParam(FObjectDelivererProtocolDisconnected, UObjectDelivererProtocol*);
DECLARE_DELEGATE_TwoParams(FObjectDelivererProtocolReceiveData, UObjectDelivererProtocol*, const TArray<uint8>&);

class UPacketRule;

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UObjectDelivererProtocol : public UObject
{
	GENERATED_BODY()

public:
	UObjectDelivererProtocol();
	~UObjectDelivererProtocol();

	/**
	 * start communication protocol.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "ObjectDeliverer")
	void Start();
	virtual void Start_Implementation();

	/**
	 * close communication protocol.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "ObjectDeliverer")
	void Close();
	virtual void Close_Implementation();

	/**
	 * send the data to the connection destination.
	 * @param DataBuffer - databuffer
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "ObjectDeliverer")
	void Send(const TArray<uint8>& DataBuffer);
	virtual void Send_Implementation(const TArray<uint8>& DataBuffer);


	virtual void BeginDestroy() override;

	void SetPacketRule(UPacketRule* PacketRule);

	virtual void RequestSend(const TArray<uint8>& DataBuffer);

protected:
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer")
	void DispatchConnected(UObjectDelivererProtocol* ConnectedObject);
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer")
	void DispatchDisconnected(UObjectDelivererProtocol* DisconnectedObject);
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer")
	void DispatchReceiveData(UObjectDelivererProtocol* FromObject, const TArray<uint8>& Buffer);

	UPROPERTY()
	UPacketRule* PacketRule;

public:

	FObjectDelivererProtocolConnected Connected;
	FObjectDelivererProtocolDisconnected Disconnected;
	FObjectDelivererProtocolReceiveData ReceiveData;
};
