#pragma once

#include "CoreMinimal.h"
#include "ProtocolFactory.generated.h"


UCLASS(BlueprintType)
class OBJECTDELIVERER_API UProtocolFactory : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolTcpIpClient* CreateProtocolTcpIpClient(const FString& IpAddress = "localhost", int32 Port = 8000, bool Retry = false);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolTcpIpServer* CreateProtocolTcpIpServer(int32 Port = 8000);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolUdpSocketSender* CreateProtocolUdpSocketSender(const FString& IpAddress = "localhost", int32 Port = 8000);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolUdpSocketReceiver* CreateProtocolUdpSocketReceiver(int32 BoundPort = 8000);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolSharedMemory* CreateProtocolSharedMemory(FString SharedMemoryName = "SharedMemory", int32 SharedMemorySize = 1024);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolLogWriter* CreateProtocolLogWriter(const FString& FilePath = "log.bin", bool PathIsAblolute = false);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolLogReader* CreateProtocolLogReader(const FString& FilePath = "log.bin", bool PathIsAblolute = false);

};
