#include "ProtocolTcpIpClient.h"
#include "TcpSocketBuilder.h"
#include "Utils/WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UProtocolTcpIpClient::UProtocolTcpIpClient()
{

}

UProtocolTcpIpClient::~UProtocolTcpIpClient()
{

}

void UProtocolTcpIpClient::Initialize(const FString& IpAddress, int32 Port, bool Retry/* = false*/)
{
	ServerIpAddress = IpAddress;
	ServerPort = Port;
	RetryConnect = Retry;
}

void UProtocolTcpIpClient::Start_Implementation()
{
	CloseSocket(true);

	auto socket = FTcpSocketBuilder(TEXT("ObjectDeliverer TcpIpClient"))
		.AsBlocking()
		.WithReceiveBufferSize(1024 * 1024)
		.Build();

	if (socket == nullptr) return;

	auto endPoint = GetIP4EndPoint(ServerIpAddress, ServerPort);
	if (!endPoint.Get<0>()) return;

	ConnectEndPoint = endPoint.Get<1>();
	InnerSocket = socket;

	ConnectInnerThread = new FWorkerThread([this] { return TryConnect(); }, 1.0f);
	ConnectThread = FRunnableThread::Create(ConnectInnerThread, TEXT("ObjectDeliverer UProtocolTcpIpClient ConnectThread"));
}

bool UProtocolTcpIpClient::TryConnect()
{
	if (InnerSocket->Connect(ConnectEndPoint.ToInternetAddr().Get()))
	{
		DispatchConnected(this);

		OnConnected(InnerSocket);		
	}
	else if (RetryConnect)
	{
		return true;
	}

	return false;
}

void UProtocolTcpIpClient::Close_Implementation()
{
	Super::Close_Implementation();

	if (!ConnectThread) return;
	ConnectThread->Kill(true);

	delete ConnectThread;
	ConnectThread = nullptr;

	if (!ConnectInnerThread) return;
	delete ConnectInnerThread;
	ConnectInnerThread = nullptr;
}