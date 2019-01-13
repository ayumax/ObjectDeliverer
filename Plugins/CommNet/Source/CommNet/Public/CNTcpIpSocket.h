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

	virtual void BeginDestroy() override;

protected:
	void CloseSocket(bool Wait);
	void StartPollilng();
	void ReceivedData();
	uint32 WantSize();
	void OnReceivedSize();
	void OnReceivedBody();

protected:
	class FWorkerThread* CurrentInnerThread = nullptr;
	class FRunnableThread* CurrentThread = nullptr;

	enum EReceiveMode
	{
		Size,
		Body
	} ReceiveMode;
	
	TArray<uint8> ReceiveBuffer;
	uint32 BodySize;

};
