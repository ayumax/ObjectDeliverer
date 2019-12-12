// Copyright 2019 ayumax. All Rights Reserved.
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

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	virtual void Initialize(const FString& FilePath = "log.bin", bool PathIsAbsolute = false);

	virtual void Start() override;
	virtual void Close() override;
	virtual void Send(const TArray<uint8>& DataBuffer) const override;
	virtual void RequestSend(const TArray<uint8>& DataBuffer) override;


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	FString FilePath = "log.bin";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	bool PathIsAbsolute = false;

private:
	class ODFileWriterUtil* Writer;
	FDateTime StartTime;
};
