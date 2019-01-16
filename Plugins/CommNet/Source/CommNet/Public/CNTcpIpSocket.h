#pragma once

#include "CoreMinimal.h"
#include "CNSocketBase.h"
#include "CNTcpIpSocket.generated.h"

UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UCNTcpIpSocket : public UCNSocketBase
{
	GENERATED_BODY()

public:
	UCNTcpIpSocket();
	virtual ~UCNTcpIpSocket();

	virtual void Close_Implementation() override;
	virtual void Send_Implementation(const TArray<uint8>& DataBuffer) override;

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
