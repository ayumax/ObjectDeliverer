#pragma once

#include "CoreMinimal.h"
#include "CNUdpSocket.h"

#include "CNUdpSocketReceiver.generated.h"


class FSocket;
class FUdpSocketReceiver;

UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UCNUdpSocketReceiver : public UCNUdpSocket
{
	GENERATED_BODY()

public:
	UCNUdpSocketReceiver();
	~UCNUdpSocketReceiver();

	virtual void Start_Implementation() override;
	virtual void Close_Implementation() override;

protected:
	void UdpReceivedCallback(const FArrayReaderPtr& data, const FIPv4Endpoint& ip);

private:
	FUdpSocketReceiver* Receiver = nullptr;

public:
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "CommNet")
	int32 BoundPort;
};
