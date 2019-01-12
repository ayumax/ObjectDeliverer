#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "Sockets.h"

#include "CNSocketBase.generated.h"

UCLASS()
class COMMNET_API UCNSocketBase : public UObject
{
	GENERATED_BODY()

public:
	UCNSocketBase();
	~UCNSocketBase();

	void CloseInnerSocket();

	void SendTo(const TArray<uint8>& DataBuffer, const FString& IpAddress, int32 Port);

	void SendToConnected(const TArray<uint8>& DataBuffer);

protected:
	bool FormatIP4ToNumber(const FString& IpAddress, uint8(&Out)[4]);

protected:
	FSocket* InnerSocket = nullptr;
};
