#pragma once

#include "CoreMinimal.h"
#include "CommNetProtocol.generated.h"

/**
* 
*/
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

};
