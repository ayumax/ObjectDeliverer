#pragma once

#include "CoreMinimal.h"
#include "ObjectDelivererProtocol.h"
#include "ProtocolLogWriter.generated.h"

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolLogWriter : public UObjectDelivererProtocol
{
	GENERATED_BODY()

public:
	UProtocolLogWriter();
	~UProtocolLogWriter();

	/**
	 * Initialize UDP.
	 * @param IpAddress - The ip address of the destination.
	 * @param Port - The port number of the destination.
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	virtual void Initialize(const FString& FilePath = "log.bin", bool PathIsAblolute = false);

	virtual void Start() override;
	virtual void Close() override;
	virtual void Send(const TArray<uint8>& DataBuffer) override;

	virtual void RequestSend(const TArray<uint8>& DataBuffer) override;


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	FString FilePath = "log.bin";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	bool PathIsAblolute = false;

private:
	class FileWriterUtil* Writer;
	FDateTime StartTime;
};
