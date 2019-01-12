#pragma once

#include "CoreMinimal.h"
#include "CNTcpIpSocket.h"
#include "CNTcpIpClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTcpIpClientConnected, class UCNTcpIpSocket*, ClientSocket);


UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UCNTcpIpClient : public UCNTcpIpSocket
{
	GENERATED_BODY()

public:
	UCNTcpIpClient();
	~UCNTcpIpClient();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		bool Connect(const FString& IpAddress, int32 Port);

public:
	UPROPERTY(BlueprintAssignable, Category = "CommNet")
		FTcpIpClientConnected Connected;

};
