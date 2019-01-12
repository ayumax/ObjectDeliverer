#include "CNUdpSocket.h"
#include "UdpSocketBuilder.h"
#include "WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UCNUdpSocket::UCNUdpSocket()
{

}

UCNUdpSocket::~UCNUdpSocket()
{

}

void UCNUdpSocket::Start()
{
	InnerSocket = FUdpSocketBuilder(TEXT("CommNet UdpSocket"))
		.AsBlocking()
		.WithReceiveBufferSize(1024 * 1024)
		.Build();

	OnStart();
}

void UCNUdpSocket::Close()
{
	CloseInnerSocket();

	OnClose();
}
