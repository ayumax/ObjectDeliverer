#include "ProtocolFactory.h"
#include "ProtocolTcpIpClient.h"
#include "ProtocolTcpIpServer.h"
#include "ProtocolUdpSocketSender.h"
#include "ProtocolUdpSocketReceiver.h"
#include "ProtocolSharedMemory.h"

UProtocolTcpIpClient* UProtocolFactory::CreateProtocolTcpIpClient(const FString& IpAddress, int32 Port, bool Retry/* = false*/)
{
	auto protocol = NewObject<UProtocolTcpIpClient>();
	protocol->Initialize(IpAddress, Port, Retry);
	return protocol;
}

UProtocolTcpIpServer* UProtocolFactory::CreateProtocolTcpIpServer(int32 Port)
{
	auto protocol = NewObject<UProtocolTcpIpServer>();
	protocol->Initialize(Port);
	return protocol;
}

UProtocolUdpSocketSender* UProtocolFactory::CreateProtocolUdpSocketSender(const FString& IpAddress, int32 Port)
{
	auto protocol = NewObject<UProtocolUdpSocketSender>();
	protocol->Initialize(IpAddress, Port);
	return protocol;
}

UProtocolUdpSocketReceiver* UProtocolFactory::CreateProtocolUdpSocketReceiver(int32 BoundPort)
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