#pragma once

#include "CoreMinimal.h"
#include "CommNetProtocol.generated.h"

DECLARE_DELEGATE_OneParam(FCommNetProtocolConnected, UCommNetProtocol*);
DECLARE_DELEGATE_OneParam(FCommNetProtocolDisconnected, UCommNetProtocol*);
DECLARE_DELEGATE_ThreeParams(FCommNetProtocolReceiveData, UCommNetProtocol*, const TArray<uint8>&, int32);


UCLASS(BlueprintType)
class COMMNET_API UCommNetProtocol : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * start communication protocol.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	void Start();
	virtual void Start_Implementation();

	/**
	 * close communication protocol.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	void Close();
	virtual void Close_Implementation();

	/**
	 * send the data to the connection destination.
	 * @param DataBuffer - databuffer
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "CommNet")
	void Send(const TArray<uint8>& DataBuffer);
	virtual void Send_Implementation(const TArray<uint8>& DataBuffer);

protected:
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void DispatchConnected(UCommNetProtocol* ConnectedObject);
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void DispatchDisconnected(UCommNetProtocol* DisconnectedObject);
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void DispatchReceiveData(UCommNetProtocol* FromObject, const TArray<uint8>& Buffer, int32 Size);

public:
	FCommNetProtocolConnected Connected;
	FCommNetProtocolDisconnected Disconnected;
	FCommNetProtocolReceiveData ReceiveData;
};
