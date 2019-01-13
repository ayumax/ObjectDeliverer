#pragma once

#include "CoreMinimal.h"
#include "CommNetManager.generated.h"

class UCommNetProtocol;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCommNetManagerConnected, UCommNetProtocol*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCommNetManagerDisconnected, UCommNetProtocol*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCommNetManagerReceiveData, UCommNetProtocol*, ClientSocket, const TArray<uint8>&, Buffer, int32, Size);

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
	void Start(UCommNetProtocol* Protocol);

	/**
	 * close communication protocol.
	 */
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void Close();

	/**
	 * send the data to the connection destination.
	 * @param DataBuffer - databuffer
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
