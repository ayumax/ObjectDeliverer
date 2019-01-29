#pragma once

#include "CoreMinimal.h"
#include "ProtocolFactory.generated.h"


UCLASS(BlueprintType)
class OBJECTDELIVERER_API UProtocolFactory : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolTcpIpClient* CreateProtocolTcpIpClient(const FString& IpAddress, int32 Port, bool Retry = false);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolTcpIpServer* CreateProtocolTcpIpServer(int32 Port);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolUdpSocketSender* CreateProtocolUdpSocketSender(const FString& IpAddress, int32 Port);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolUdpSocketReceiver* CreateProtocolUdpSocketReceiver(int32 BoundPort);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolSharedMemory* CreateProtocolSharedMemory(FString SharedMemoryName = "SharedMemory", int32 SharedMemorySize = 1024);
};
