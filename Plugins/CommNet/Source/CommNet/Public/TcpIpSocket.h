#pragma once

#include "CoreMinimal.h"
#include "TcpIpSocket.generated.h"

UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UTcpIpSocket : public UObject
{
	GENERATED_BODY()
public:

	UTcpIpSocket();
	~UTcpIpSocket();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		bool Connect(const FString& IpAddress, int32 Port);
};