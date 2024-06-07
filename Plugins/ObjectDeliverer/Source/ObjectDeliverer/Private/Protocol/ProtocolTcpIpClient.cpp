// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolTcpIpClient.h"
#include "Common/TcpSocketBuilder.h"
#include "Utils/ODWorkerThread.h"
#include "HAL/RunnableThread.h"

UProtocolTcpIpClient::UProtocolTcpIpClient()
{

}

UProtocolTcpIpClient::~UProtocolTcpIpClient()
{
	
}

void UProtocolTcpIpClient::Initialize(const FString& IpAddress, int32 Port, bool Retry/* = false*/, bool _AutoConnectAfterDisconnect/* = false*/)
{
	ISocketSubsystem* SocketSubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	auto result = SocketSubSystem->GetAddressInfo(*IpAddress, nullptr, EAddressInfoFlags::Default, NAME_None);
	if (result.Results.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("GetAddressInfo failed"));
		return;
	}
	auto ip = result.Results[0].Address->ToString(false);

	ServerIpAddress = ip;
	ServerPort = Port;
	RetryConnect = Retry;
	AutoConnectAfterDisconnect = _AutoConnectAfterDisconnect;
}

UProtocolTcpIpClient* UProtocolTcpIpClient::WithReceiveBufferSize(int32 SizeInBytes)
{
	ReceiveBufferSize = SizeInBytes;

	return this;
}

UProtocolTcpIpClient* UProtocolTcpIpClient::WithSendBufferSize(int32 SizeInBytes)
{
	SendBufferSize = SizeInBytes;

	return this;
}

void UProtocolTcpIpClient::Start()
{
	CloseSocket();

	auto socket = FTcpSocketBuilder(TEXT("ObjectDeliverer TcpIpClient"))
		.AsBlocking()
		.WithReceiveBufferSize(ReceiveBufferSize)
		.WithSendBufferSize(SendBufferSize)
		.Build();

	if (socket == nullptr) return;

	auto endPoint = GetIP4EndPoint(ServerIpAddress, ServerPort);
	if (!endPoint.Get<0>()) return;

	ConnectEndPoint = endPoint.Get<1>();
	InnerSocket = socket;

	ConnectInnerThread = new FODWorkerThread([this] { return TryConnect(); }, 1.0f);
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

void UProtocolTcpIpClient::Close()
{
	Super::Close();

	if (!ConnectThread) return;
	ConnectThread->Kill(true);

	delete ConnectThread;
	ConnectThread = nullptr;

	if (!ConnectInnerThread) return;
	delete ConnectInnerThread;
	ConnectInnerThread = nullptr;
}


void UProtocolTcpIpClient::DispatchDisconnected(const UObjectDelivererProtocol* DisconnectedObject)
{
	Super::DispatchDisconnected(DisconnectedObject);

	if (AutoConnectAfterDisconnect)
	{
		Start();
	}
}