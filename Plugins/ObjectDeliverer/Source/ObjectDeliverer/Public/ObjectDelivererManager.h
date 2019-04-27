// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "PacketRule/PacketRuleFactory.h"
#include "Protocol/ProtocolFactory.h"
#include "DeliveryBox/DeliveryBoxFactory.h"
#include "UObject/GCObject.h"
#include "Delegates/DelegateCombinations.h"
#include "ObjectDelivererManager.generated.h"

class UObjectDelivererProtocol;
class UPacketRule;
class UDeliveryBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectDelivererManagerConnected, const UObjectDelivererProtocol*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectDelivererManagerDisconnected, const UObjectDelivererProtocol*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FObjectDelivererManagerReceiveData, const UObjectDelivererProtocol*, ClientSocket, const TArray<uint8>&, Buffer);

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UObjectDelivererManager : public UObject
{
	GENERATED_BODY()

public:
	UObjectDelivererManager();
	~UObjectDelivererManager();

	/**
	 * create ObjectDelivererManager
	 * @param IsEventWithGameThread - Process events with GameThread
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer")
	static UObjectDelivererManager* CreateObjectDelivererManager(bool IsEventWithGameThread = true);

	/**
	 * start communication protocol.
	 * @param Protocol - Communication protocol
	 * @param PacketRule - Data division rule
	 * @param DeliveryBox - Serialization method(optional)
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
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer")
	void Send(const TArray<uint8>& DataBuffer);

	/**
	 * send the data to the connection destination.
	 * @param DataBuffer - databuffer
	 * @param Target - send destination
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer")
	void SendTo(const TArray<uint8>& DataBuffer, const UObjectDelivererProtocol* Target);

	/**
	* Whether connected
	* @return true:connected, false:disconnected
	*/
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer")
	bool IsConnected();

	/**
	* Preparation for transmission / reception is completed
	*/
	UPROPERTY(BlueprintAssignable, Category = "ObjectDeliverer")
	FObjectDelivererManagerConnected Connected;

	/**
	* lost my connection
	*/
	UPROPERTY(BlueprintAssignable, Category = "ObjectDeliverer")
	FObjectDelivererManagerDisconnected Disconnected;

	/**
	* Received data
	*/
	UPROPERTY(BlueprintAssignable, Category = "ObjectDeliverer")
	FObjectDelivererManagerReceiveData ReceiveData;

	virtual void BeginDestroy() override;

	/**
	* Process events with GameThread
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer")
	bool IsEventWithGameThread;

	/**
	* Connected list
	*/
	UPROPERTY(BlueprintReadOnly, Category = "ObjectDeliverer")
	TArray<const UObjectDelivererProtocol*> ConnectedList;

private:
	void DispatchEvent(TFunction<void()> EventAction);

private:
	UPROPERTY(Transient)
	UObjectDelivererProtocol* CurrentProtocol;
	UPROPERTY(Transient)
	UDeliveryBox* DeliveryBox;

	bool IsDestorying;
};

