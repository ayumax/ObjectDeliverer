#pragma once

#include "CoreMinimal.h"
#include "CNUdpSocket.h"

#include "CNUdpSocketReceiver.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUdpSocketReceiveData, class UCNUdpSocketReceiver*, ClientSocket, const TArray<uint8>&, Buffer, int32, Size);

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

public:
	UPROPERTY(BlueprintAssignable, Category = "CommNet")
		FUdpSocketReceiveData ReceiveData;

private:
	FUdpSocketReceiver* Receiver = nullptr;
};
