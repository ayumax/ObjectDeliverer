// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ProtocolFactory.generated.h"


UCLASS(BlueprintType)
class OBJECTDELIVERER_API UProtocolFactory : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * create protocol (TCP/IP client)
	 * @param IpAddress - ip address of server
	 * @param Port - Connected port number
	 * @param Retry - true:If connection fails, try retry until connection
	 * @param AutoConnectAfterDisconnect - true: Automatic connection attempt after disconnection
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolTcpIpClient* CreateProtocolTcpIpClient(const FString& IpAddress = "localhost", int32 Port = 8000, bool Retry = false, bool AutoConnectAfterDisconnect = false);

	/**
	 * create protocol (TCP/IP server)
	 * @param Port - Port number to listen to
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolTcpIpServer* CreateProtocolTcpIpServer(int32 Port = 8000);

	/**
	 * create protocol (UDP send only)
	 * @param IpAddress - ip address of the destination
	 * @param Port - port number of the destination
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolUdpSocketSender* CreateProtocolUdpSocketSender(const FString& IpAddress = "localhost", int32 Port = 8000);

	/**
	 * create protocol (UDP receive only)
	 * @param Port - Port number to bind to
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolUdpSocketReceiver* CreateProtocolUdpSocketReceiver(int32 BoundPort = 8000);

	/**
	 * create protocol (shared memory)
	 * @param SharedMemoryName - shared memory name
	 * @param SharedMemorySize - shared memory size (byte)
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolSharedMemory* CreateProtocolSharedMemory(FString SharedMemoryName = "SharedMemory", int32 SharedMemorySize = 1024);

	/**
	 * create protocol (Log file write only)
	 * @param FilePath - log file path
	 * @param PathIsAbsolute - true:FilePath is absolute path, false:Relative path from "Logs"
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolLogWriter* CreateProtocolLogWriter(const FString& FilePath = "log.bin", bool PathIsAbsolute = false);

	/**
	 * create protocol (Log file read only)
	 * @param FilePath - log file path
	 * @param PathIsAbsolute - true:FilePath is absolute path, false:Relative path from "Logs"
	 */
	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolLogReader* CreateProtocolLogReader(const FString& FilePath = "log.bin", bool PathIsAbsolute = false, bool CutFirstInterval = true);

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer|Protocol")
	static class UProtocolReflection* CreateProtocolReflection();

};
