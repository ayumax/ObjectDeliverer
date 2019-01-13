#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "Sockets.h"
#include "CommNetProtocol.h"
#include "CNSocketBase.generated.h"

UCLASS(BlueprintType)
class COMMNET_API UCNSocketBase : public UCommNetProtocol
{
	GENERATED_BODY()

public:
	UCNSocketBase();
	~UCNSocketBase();

	void CloseInnerSocket();

	void SendTo(const TArray<uint8>& DataBuffer, const FIPv4Endpoint& EndPoint);

	void SendToConnected(const TArray<uint8>& DataBuffer);

protected:
	bool FormatIP4ToNumber(const FString& IpAddress, uint8(&Out)[4]);
	TTuple<bool, FIPv4Endpoint> GetIP4EndPoint(const FString& IpAddress, int32 Port);

protected:
	FSocket* InnerSocket = nullptr;
};
