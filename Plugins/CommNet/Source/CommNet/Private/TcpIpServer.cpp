#include "TcpIpServer.h"
#include "TcpSocketBuilder.h"
#include "WorkerThread.h"
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


	auto worker = new FWorkerThread([this] { OnListen(); });
	ListenThread = FRunnableThread::Create(worker, TEXT("CommNet TcpIpSocket ListenThread"));
	

	return true;
}

void UTcpIpServer::Close()
{
	
}

void UTcpIpServer::Send(const TArray<uint8>& DataBuffer)
{
	
}

void UTcpIpServer::OnListen()
{

}