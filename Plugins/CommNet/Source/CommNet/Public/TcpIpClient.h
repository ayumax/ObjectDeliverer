#pragma once

#include "CoreMinimal.h"
#include "TcpIpSocket.h"
#include "TcpIpClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTcpIpClientConnected);


UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UTcpIpClient : public UTcpIpSocket
{
	GENERATED_BODY()
public:

	UTcpIpClient();
	~UTcpIpClient();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		bool Connect(const FString& IpAddress, int32 Port);


private:
	bool FormatIP4ToNumber(const FString& IpAddress, uint8(&Out)[4]);


public:
	FTcpIpClientConnected Connected;

};
