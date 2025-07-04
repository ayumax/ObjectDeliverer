// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolFactory.h"
#include "Protocol/ProtocolTcpIpClient.h"
#include "Protocol/ProtocolTcpIpServer.h"
#include "Protocol/ProtocolUdpSocketSender.h"
#include "Protocol/ProtocolUdpSocketReceiver.h"
#include "Protocol/ProtocolSharedMemory.h"
#include "Protocol/ProtocolLogReader.h"
#include "Protocol/ProtocolLogWriter.h"
#include "Protocol/ProtocolReflection.h"
#include "Protocol/ProtocolWebSocketClient.h"

UProtocolTcpIpClient *UProtocolFactory::CreateProtocolTcpIpClient(const FString &IpAddress /*="localhost"*/, int32 Port /*= 8000*/, bool Retry /*= false*/, bool AutoConnectAfterDisconnect /*= false*/)
{
	auto protocol = NewObject<UProtocolTcpIpClient>();
	protocol->Initialize(IpAddress, Port, Retry, AutoConnectAfterDisconnect);
	return protocol;
}

UProtocolTcpIpServer *UProtocolFactory::CreateProtocolTcpIpServer(int32 Port /*= 8000*/)
{
	auto protocol = NewObject<UProtocolTcpIpServer>();
	protocol->Initialize(Port);
	return protocol;
}

UProtocolUdpSocketSender *UProtocolFactory::CreateProtocolUdpSocketSender(const FString &IpAddress /*="localhost"*/, int32 Port /*= 8000*/)
{
	auto protocol = NewObject<UProtocolUdpSocketSender>();
	protocol->Initialize(IpAddress, Port);
	return protocol;
}

UProtocolUdpSocketSender *UProtocolFactory::CreateProtocolUdpSocketSenderWithBroadcast(const FString &IpAddress /*="localhost"*/, int32 Port /*= 8000*/, bool EnableBroadcast /*= true*/)
{
	auto protocol = NewObject<UProtocolUdpSocketSender>();
	protocol->Initialize(IpAddress, Port);
	protocol->WithBroadcast(EnableBroadcast);
	return protocol;
}

UProtocolUdpSocketReceiver *UProtocolFactory::CreateProtocolUdpSocketReceiver(int32 BoundPort /*= 8000*/)
{
	auto protocol = NewObject<UProtocolUdpSocketReceiver>();
	protocol->InitializeWithReceiver(BoundPort);
	return protocol;
}

UProtocolSharedMemory *UProtocolFactory::CreateProtocolSharedMemory(FString SharedMemoryName /* = "SharedMemory"*/, int32 SharedMemorySize /* = 1024*/)
{
	auto protocol = NewObject<UProtocolSharedMemory>();
	protocol->Initialize(SharedMemoryName, SharedMemorySize);
	return protocol;
}

UProtocolLogReader *UProtocolFactory::CreateProtocolLogReader(const FString &FilePath /*= "log.bin"*/, bool PathIsAbsolute /*= false*/, bool CutFirstInterval /*= true*/)
{
	auto protocol = NewObject<UProtocolLogReader>();
	protocol->Initialize(FilePath, PathIsAbsolute, CutFirstInterval);
	return protocol;
}

UProtocolLogWriter *UProtocolFactory::CreateProtocolLogWriter(const FString &FilePath /*= "log.bin"*/, bool PathIsAbsolutePath /* = false*/)
{
	auto protocol = NewObject<UProtocolLogWriter>();
	protocol->Initialize(FilePath, PathIsAbsolutePath);
	return protocol;
}

UProtocolReflection *UProtocolFactory::CreateProtocolReflection()
{
	return NewObject<UProtocolReflection>();
}

UProtocolWebSocketClient *UProtocolFactory::CreateProtocolWebSocketClient(const FString &Url)
{
	auto protocol = NewObject<UProtocolWebSocketClient>();
	protocol->Initialize(Url);
	return protocol;
}

UProtocolWebSocketClient *UProtocolFactory::CreateProtocolWebSocketClientWithProtocols(const FString &Url, const TArray<FString> &Protocols)
{
	auto protocol = NewObject<UProtocolWebSocketClient>();
	protocol->InitializeWithProtocols(Url, Protocols);
	return protocol;
}

UProtocolWebSocketClient *UProtocolFactory::CreateProtocolWebSocketClientWithHeaders(const FString &Url, const TArray<FString> &Protocols, const TMap<FString, FString> &Headers)
{
	auto protocol = NewObject<UProtocolWebSocketClient>();
	protocol->InitializeWithHeaders(Url, Protocols, Headers);
	return protocol;
}

