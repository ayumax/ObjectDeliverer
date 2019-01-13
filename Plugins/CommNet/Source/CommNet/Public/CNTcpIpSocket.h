#pragma once

#include "CoreMinimal.h"
#include "CNSocketBase.h"
#include "CNTcpIpSocket.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTcpIpSocketDisconnected, class UCNTcpIpSocket*, ClientSocket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTcpIpSocketReceiveData, class UCNTcpIpSocket*, ClientSocket, const TArray<uint8>&, Buffer, int32, Size);

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

public:
	UPROPERTY(BlueprintAssignable, Category = "CommNet")
		FTcpIpSocketDisconnected Disconnected;
	UPROPERTY(BlueprintAssignable, Category = "CommNet")
		FTcpIpSocketReceiveData ReceiveData;
};
