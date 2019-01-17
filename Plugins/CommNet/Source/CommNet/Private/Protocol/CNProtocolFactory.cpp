#include "CNProtocolFactory.h"
#include "CNTcpIpClient.h"
#include "CNTcpIpServer.h"
#include "CNUdpSocketSender.h"
#include "CNUdpSocketReceiver.h"

UCNTcpIpClient* UCNProtocolFactory::CreateTcpIpClient(const FString& IpAddress, int32 Port, bool Retry/* = false*/)
{
	auto protocol = NewObject<UCNTcpIpClient>();
	protocol->Initialize(IpAddress, Port, Retry);
	return protocol;
}

UCNTcpIpServer* UCNProtocolFactory::CreateTcpIpServer(int32 Port)
{
	auto protocol = NewObject<UCNTcpIpServer>();
	protocol->Initialize(Port);
	return protocol;
}

UCNUdpSocketSender* UCNProtocolFactory::CreateUdpSocketSender(const FString& IpAddress, int32 Port)
{
	auto protocol = NewObject<UCNUdpSocketSender>();
	protocol->Initialize(IpAddress, Port);
	return protocol;
}

UCNUdpSocketReceiver* UCNProtocolFactory::CreateCNUdpSocketReceiver(int32 BoundPort)
{
	auto protocol = NewObject<UCNUdpSocketReceiver>();
	protocol->InitializeWithReceiver(BoundPort);
	return protocol;
}