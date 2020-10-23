// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ProtocolSocketBase.h"
#include "GetIPV4Info.h"
#include "Utils/ODGrowBuffer.h"
#include "ProtocolUdpSocket.generated.h"


class FSocket;

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolUdpSocket : public UProtocolSocketBase, public IGetIPV4Info
{
	GENERATED_BODY()

public:
	UProtocolUdpSocket();
	~UProtocolUdpSocket();

	void Initialize(FIPv4Endpoint IP);
	void NotifyReceived(const ODByteSpan& data);

	bool GetIPAddress(TArray<uint8>& IPAddress) override;
	bool GetIPAddressInString(FString& IPAddress) override;


private:
	FIPv4Endpoint IPEndPoint;
	ODGrowBuffer ReceiveBuffer;
};
