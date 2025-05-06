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

	CreateSocket();

	
	auto endPoint = GetIP4EndPoint(ServerIpAddress, ServerPort);
	if (!endPoint.Get<0>()) return;

	ConnectEndPoint = endPoint.Get<1>();

	if (!InnerSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("TryConnect failed: InnerSocket is null"));
		return;
	}

	ConnectInnerThread = new FODWorkerThread([this] { return TryConnect(); }, 1.0f);
	ConnectThread = FRunnableThread::Create(ConnectInnerThread, TEXT("ObjectDeliverer UProtocolTcpIpClient ConnectThread"));
}

bool UProtocolTcpIpClient::TryConnect()
{
	UE_LOG(LogTemp, Log, TEXT("Start TryConnect"));
	
	if (!InnerSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("TryConnect failed: InnerSocket is null"));
		return false;
	}

	// Check socket state
	ESocketConnectionState ConnectionState = InnerSocket->GetConnectionState();
	UE_LOG(LogTemp, Log, TEXT("Socket Connection State before connect: %d"), (int32)ConnectionState);

	// Recreate socket if in error or connected state
	if (ConnectionState == ESocketConnectionState::SCS_ConnectionError || 
		ConnectionState == ESocketConnectionState::SCS_Connected)
	{
		UE_LOG(LogTemp, Log, TEXT("Recreating socket due to invalid state"));
		InnerSocket->Shutdown(ESocketShutdownMode::ReadWrite);
		InnerSocket->Close();
		
		CreateSocket();
	}

	// -------------------------------------------------------------------------------------
	// Skip state check on Linux due to bug where GetConnectionState() returns Connected immediately after socket creation
	// -------------------------------------------------------------------------------------
#if PLATFORM_WINDOWS || PLATFORM_MAC
	// Recheck socket state before attempting connection
	ConnectionState = InnerSocket->GetConnectionState();
	if (ConnectionState != ESocketConnectionState::SCS_NotConnected)
	{
		UE_LOG(LogTemp, Log, TEXT("Socket is not in NotConnected state, skipping connect attempt"));
		return RetryConnect;
	}
#endif
	
	if (InnerSocket->Connect(ConnectEndPoint.ToInternetAddr().Get()))
	{
		UE_LOG(LogTemp, Log, TEXT("TryConnect success"));
		DispatchConnected(this);
		OnConnected(InnerSocket);
		return false;
	}
	else
	{
		int32 ErrorCode = FPlatformMisc::GetLastError();
		ConnectionState = InnerSocket->GetConnectionState();
		
		bool PendingConnection = false;
		InnerSocket->HasPendingConnection(PendingConnection);
		
		UE_LOG(LogTemp, Log, TEXT("TryConnect failed - Error: %d, ConnectionState: %d, Pending: %d"), 
			ErrorCode, (int32)ConnectionState, PendingConnection ? 1 : 0);
			
		if (RetryConnect)
		{
			UE_LOG(LogTemp, Log, TEXT("RetryConnect is true, retrying..."));
			return true;
		}
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

void UProtocolTcpIpClient::CreateSocket()
{
	auto socket = FTcpSocketBuilder(TEXT("ObjectDeliverer TcpIpClient"))
			.AsBlocking()
			.WithReceiveBufferSize(ReceiveBufferSize)
			.WithSendBufferSize(SendBufferSize)
			.Build();

	if (socket == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to create socket"));
		return;
	}
		
	InnerSocket = socket;
}