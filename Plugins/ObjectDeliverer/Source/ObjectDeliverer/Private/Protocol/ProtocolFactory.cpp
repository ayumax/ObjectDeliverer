// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolFactory.h"
#include "Protocol/ProtocolTcpIpClient.h"
#include "Protocol/ProtocolTcpIpServer.h"
#include "Protocol/ProtocolUdpSocketSender.h"
#include "Protocol/ProtocolUdpSocketReceiver.h"
#include "Protocol/ProtocolSharedMemory.h"
#include "Protocol/ProtocolLogWriter.h"
#include "Protocol/ProtocolLogReader.h"
#include "Protocol/ProtocolReflection.h"

UProtocolTcpIpClient* UProtocolFactory::CreateProtocolTcpIpClient(const FString& IpAddress /*= "localhost"*/, int32 Port /*= 8000*/, bool Retry/* = false*/, bool AutoConnectAfterDisconnect/* = false*/)
{
	auto protocol = NewObject<UProtocolTcpIpClient>();
	protocol->Initialize(IpAddress, Port, Retry, AutoConnectAfterDisconnect);
	return protocol;
}

UProtocolTcpIpServer* UProtocolFactory::CreateProtocolTcpIpServer(int32 Port /*= 8000*/)
{
	auto protocol = NewObject<UProtocolTcpIpServer>();
	protocol->Initialize(Port);
	return protocol;
}

UProtocolUdpSocketSender* UProtocolFactory::CreateProtocolUdpSocketSender(const FString& IpAddress /*= "localhost"*/, int32 Port /*= 8000*/)
{
	auto protocol = NewObject<UProtocolUdpSocketSender>();
	protocol->Initialize(IpAddress, Port);
	return protocol;
}

UProtocolUdpSocketReceiver* UProtocolFactory::CreateProtocolUdpSocketReceiver(int32 BoundPort /*= 8000*/)
{
	auto protocol = NewObject<UProtocolUdpSocketReceiver>();
	protocol->InitializeWithReceiver(BoundPort);
	return protocol;
}

UProtocolSharedMemory* UProtocolFactory::CreateProtocolSharedMemory(FString SharedMemoryName/* = "SharedMemory"*/, int32 SharedMemorySize/* = 1024*/)
{
	auto protocol = NewObject<UProtocolSharedMemory>();
	protocol->Initialize(SharedMemoryName, SharedMemorySize);
	return protocol;
}

UProtocolLogWriter* UProtocolFactory::CreateProtocolLogWriter(const FString& FilePath /*= "log.bin"*/, bool PathIsAblolute /*= false*/)
{
	auto protocol = NewObject<UProtocolLogWriter>();
	protocol->Initialize(FilePath, PathIsAblolute);
	return protocol;
}

UProtocolLogReader* UProtocolFactory::CreateProtocolLogReader(const FString& FilePath /*= "log.bin"*/, bool PathIsAblolute /*= false*/, bool CutFirstInterval /*= true*/)
{
	auto protocol = NewObject<UProtocolLogReader>();
	protocol->Initialize(FilePath, PathIsAblolute, CutFirstInterval);
	return protocol;
}

UProtocolReflection* UProtocolFactory::CreateProtocolReflection()
{
	return NewObject<UProtocolReflection>();
}