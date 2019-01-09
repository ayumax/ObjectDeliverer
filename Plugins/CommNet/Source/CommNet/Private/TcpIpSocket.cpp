#include "TcpIpSocket.h"
#include "TcpSocketBuilder.h"

UTcpIpSocket::UTcpIpSocket()
{

}

UTcpIpSocket::~UTcpIpSocket()
{

}

bool UTcpIpSocket::Connect(const FString& IpAddress, int32 Port)
{
	auto socket = FTcpSocketBuilder(TEXT("TcpIpClient"))
		.AsNonBlocking()
		.Build();

	if (socket == nullptr) return false;

	

	return true;
}