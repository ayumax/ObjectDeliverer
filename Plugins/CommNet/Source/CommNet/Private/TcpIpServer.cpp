#include "TcpIpServer.h"
#include "TcpSocketBuilder.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UTcpIpServer::UTcpIpServer()
{

}

UTcpIpServer::~UTcpIpServer()
{

}

bool UTcpIpServer::Start(int32 Port)
{
	Close();

	auto socket = FTcpSocketBuilder(TEXT("CommNet TcpIpServer"))
		.AsBlocking()
		.BoundToPort(Port)
		.Listening(10)
		.Build();

	if (socket == nullptr) return false;



	

	return true;
}

void UTcpIpServer::Close()
{
	
}

void UTcpIpServer::Send(const TArray<uint8>& DataBuffer)
{
	
}
