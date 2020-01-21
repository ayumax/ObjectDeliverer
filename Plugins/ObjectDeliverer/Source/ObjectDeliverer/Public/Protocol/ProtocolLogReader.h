// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ObjectDelivererProtocol.h"
#include "ProtocolLogReader.generated.h"

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolLogReader : public UObjectDelivererProtocol
{
	GENERATED_BODY()

public:
	UProtocolLogReader();
	~UProtocolLogReader();

	/**
	 * Initialize UDP.
	 * @param IpAddress - The ip address of the destination.
	 * @param Port - The port number of the destination.
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	virtual void Initialize(const FString& FilePath = "log.bin", bool PathIsAbsolute = false, bool CutFirstInterval = true);

	virtual void Start() override;
	virtual void Close() override;
	virtual void Send(const TArray<uint8>& DataBuffer) const override;

	virtual void RequestSend(const TArray<uint8>& DataBuffer) override;

private:
	bool ReadData();
	void ReadEnd();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	FString FilePath = "log.bin";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	bool PathIsAbsolute = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	bool CutFirstInterval = false;

private:
	class ODFileReaderUtil* Reader = nullptr;

	class FODWorkerThread* CurrentInnerThread = nullptr;
	class FRunnableThread* CurrentThread = nullptr;

	double CurrentLogTime;
	TArray<uint8> ReadBuffer;
	TArray<uint8> ReceiveBuffer;

	FDateTime StartTime;

	int64 FilePosition;

	bool IsFirst;
};
