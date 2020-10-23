// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ProtocolSocketBase.h"
#include "GetIPV4Info.h"
#include "Utils/ODGrowBuffer.h"
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

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 ReceiveBufferSize = 1024 * 1024;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	int32 SendBufferSize = 1024 * 1024;

protected:
	void CloseSocket();
	void StartPollilng();
	bool ReceivedData();

protected:
	class FODWorkerThread* CurrentInnerThread = nullptr;
	class FRunnableThread* CurrentThread = nullptr;

	ODGrowBuffer ReceiveBuffer;
	FCriticalSection ct;
	bool IsSelfClose = false;
};
