#pragma once

#include "CoreMinimal.h"
#include "ProtocolSocketBase.h"
#include "ProtocolTcpIpSocket.generated.h"

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolTcpIpSocket : public UProtocolSocketBase
{
	GENERATED_BODY()

public:
	UProtocolTcpIpSocket();
	virtual ~UProtocolTcpIpSocket();

	virtual void Close() override;
	virtual void Send(const TArray<uint8>& DataBuffer) override;

	void OnConnected(FSocket* ConnectionSocket);

	virtual void RequestSend(const TArray<uint8>& DataBuffer) override;

protected:
	void CloseSocket(bool Wait);
	void StartPollilng();
	bool ReceivedData();

protected:
	class FWorkerThread* CurrentInnerThread = nullptr;
	class FRunnableThread* CurrentThread = nullptr;

	TArray<uint8> ReceiveBuffer;
};
