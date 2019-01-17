#pragma once

#include "CoreMinimal.h"
#include "CNSocketBase.h"
#include "CNUdpSocketReceiver.generated.h"


class FSocket;
class FUdpSocketReceiver;

UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UCNUdpSocketReceiver : public UCNSocketBase
{
	GENERATED_BODY()

public:
	UCNUdpSocketReceiver();
	~UCNUdpSocketReceiver();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void InitializeWithReceiver(int32 BoundPort = 8001);

	virtual void Start_Implementation() override;
	virtual void Close_Implementation() override;

protected:
	void UdpReceivedCallback(const FArrayReaderPtr& data, const FIPv4Endpoint& ip);

private:
	FUdpSocketReceiver* Receiver = nullptr;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	int32 BoundPort = 8001;
};
