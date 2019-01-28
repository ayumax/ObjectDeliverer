#pragma once

#include "CoreMinimal.h"
#include "ObjectDelivererProtocol.h"
#include "ProtocolSharedMemory.generated.h"


UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolSharedMemory : public UObjectDelivererProtocol
{
	GENERATED_BODY()

public:
	UProtocolSharedMemory();
	~UProtocolSharedMemory();

	/**
	 * Initialize TCP/IP server.
	 * @param SharedMemoryName - SharedMemory name.
	 * @param SharedMemorySize - SharedMemory size.
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	void Initialize(const FString& SharedMemoryName = "SharedMemory", int32 SharedMemorySize = 1024);

	virtual void Start_Implementation() override;
	virtual void Close_Implementation() override;
	
	virtual void RequestSend(const TArray<uint8>& DataBuffer) override;
	
protected:
	bool ReceivedData();
	void CloseSharedMemory();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	FString SharedMemoryName = "SharedMemory";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 SharedMemorySize = 1024;

protected:
	class FWorkerThread* CurrentInnerThread = nullptr;
	class FRunnableThread* CurrentThread = nullptr;

	TArray<uint8> ReceiveBuffer;
	TArray<uint8> TempBuffer;

	void* SharedMemoryHandle;           ///<  Mapped memory handle.
	unsigned char* SharedMemoryData;	///<  Pointer to memory data.
	void* SharedMemoryMutex;            ///<  Mutex handle.

	uint8 NowCounter;
};
