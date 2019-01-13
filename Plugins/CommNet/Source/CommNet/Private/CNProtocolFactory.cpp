#include "CNProtocolFactory.h"
#include "CNTcpIpClient.h"
#include "CNTcpIpServer.h"
#include "CNUdpSocket.h"
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

UCNUdpSocket* UCNProtocolFactory::CreateUdpSocket(const FString& IpAddress, int32 Port)
{
	auto protocol = NewObject<UCNUdpSocket>();
	protocol->Initialize(IpAddress, Port);
	return protocol;
}

UCNUdpSocketReceiver* UCNProtocolFactory::CreateCNUdpSocketReceiver(const FString& IpAddress, int32 Port, int32 BoundPort)
{
	auto protocol = NewObject<UCNUdpSocketReceiver>();
	protocol->InitializeWithReceiver(IpAddress, Port, BoundPort);
	return protocol;
}