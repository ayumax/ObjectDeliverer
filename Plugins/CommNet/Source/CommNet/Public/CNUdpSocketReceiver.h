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

	virtual void BeginDestroy() override;

protected:
	virtual void OnStart() override;
	virtual void OnClose() override;
	
	void UdpReceivedCallback(const FArrayReaderPtr& data, const FIPv4Endpoint& ip);

private:
	FUdpSocketReceiver* Receiver = nullptr;
};
