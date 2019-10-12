// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ProtocolSocketBase.h"
#include "GetIPV4Info.h"
#include "ProtocolTcpIpSocket.generated.h"

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolTcpIpSocket : public UProtocolSocketBase, public IGetIPV4Info
{
	GENERATED_BODY()

public:
	UProtocolTcpIpSocket();
	virtual ~UProtocolTcpIpSocket();

	virtual void Close() override;
	virtual void Send(const TArray<uint8>& DataBuffer) const override;

	void OnConnected(FSocket* ConnectionSocket);

	virtual void RequestSend(const TArray<uint8>& DataBuffer) override;

	bool GetIPAddress(TArray<uint8>& IPAddress) override;
	bool GetIPAddressInString(FString& IPAddress) override;

protected:
	void CloseSocket();
	void StartPollilng();
	bool ReceivedData();

protected:
	class FWorkerThread* CurrentInnerThread = nullptr;
	class FRunnableThread* CurrentThread = nullptr;

	TArray<uint8> ReceiveBuffer;
	FCriticalSection ct;
	bool IsSelfClose = false;
};
