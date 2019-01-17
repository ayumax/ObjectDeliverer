#pragma once

#include "CoreMinimal.h"
#include "CNProtocolFactory.generated.h"


UCLASS(BlueprintType)
class COMMNET_API UCNProtocolFactory : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "CommNet")
	static class UCNTcpIpClient* CreateTcpIpClient(const FString& IpAddress, int32 Port, bool Retry = false);

	UFUNCTION(BlueprintPure, Category = "CommNet")
	static class UCNTcpIpServer* CreateTcpIpServer(int32 Port);

	UFUNCTION(BlueprintPure, Category = "CommNet")
	static class UCNUdpSocketSender* CreateUdpSocketSender(const FString& IpAddress, int32 Port);

	UFUNCTION(BlueprintPure, Category = "CommNet")
	static class UCNUdpSocketReceiver* CreateCNUdpSocketReceiver(int32 BoundPort);
};
