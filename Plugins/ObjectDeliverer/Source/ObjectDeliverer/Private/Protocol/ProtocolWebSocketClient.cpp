// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolWebSocketClient.h"
#include "WebSocketsModule.h"
#include "Utils/ODWorkerThread.h"
#include "HAL/RunnableThread.h"
#include "PacketRule/PacketRule.h"

UProtocolWebSocketClient::UProtocolWebSocketClient()
{
}

UProtocolWebSocketClient::~UProtocolWebSocketClient()
{
	Close();
}

void UProtocolWebSocketClient::Initialize(const FString& Url)
{
	ServerUrl = Url;
}

void UProtocolWebSocketClient::InitializeWithProtocols(const FString& Url, const TArray<FString>& Protocols)
{
	ServerUrl = Url;
	SubProtocols = Protocols;
}

void UProtocolWebSocketClient::InitializeWithHeaders(const FString& Url, const TArray<FString>& Protocols, const TMap<FString, FString>& Headers)
{
	ServerUrl = Url;
	SubProtocols = Protocols;
	ConnectionHeaders = Headers;
}

void UProtocolWebSocketClient::SetAutoReconnect(bool bInAutoReconnect)
{
	bAutoReconnect = bInAutoReconnect;
}

void UProtocolWebSocketClient::SetReconnectDelay(float Delay)
{
	ReconnectDelay = FMath::Max(0.1f, Delay);
}

void UProtocolWebSocketClient::Start()
{
	bIsManualClose = false;
	CreateWebSocket();
}

void UProtocolWebSocketClient::Close()
{
	bIsManualClose = true;
	bIsReconnecting = false;

	if (ReconnectThread)
	{
		ReconnectThread->Kill(true);
		delete ReconnectThread;
		ReconnectThread = nullptr;
	}

	if (ReconnectInnerThread)
	{
		delete ReconnectInnerThread;
		ReconnectInnerThread = nullptr;
	}

	if (WebSocket.IsValid())
	{
		WebSocket->Close();
		WebSocket.Reset();
	}
}

void UProtocolWebSocketClient::Send(const TArray<uint8>& DataBuffer) const
{
	if (!WebSocket.IsValid() || !WebSocket->IsConnected())
	{
		UE_LOG(LogTemp, Warning, TEXT("WebSocket is not connected"));
		return;
	}

	WebSocket->Send(DataBuffer.GetData(), DataBuffer.Num(), true);
}

void UProtocolWebSocketClient::CreateWebSocket()
{
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServerUrl, SubProtocols, ConnectionHeaders);

	if (!WebSocket.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create WebSocket"));
		return;
	}

	WebSocket->OnConnected().AddUObject(this, &UProtocolWebSocketClient::OnWebSocketConnected);
	WebSocket->OnConnectionError().AddUObject(this, &UProtocolWebSocketClient::OnWebSocketConnectionError);
	WebSocket->OnClosed().AddUObject(this, &UProtocolWebSocketClient::OnWebSocketClosed);
	WebSocket->OnMessage().AddUObject(this, &UProtocolWebSocketClient::OnWebSocketMessage);
	WebSocket->OnBinaryMessage().AddUObject(this, &UProtocolWebSocketClient::OnWebSocketBinaryMessage);

	WebSocket->Connect();
}

void UProtocolWebSocketClient::OnWebSocketConnected()
{
	UE_LOG(LogTemp, Log, TEXT("WebSocket connected"));
	DispatchConnected(this);
}

void UProtocolWebSocketClient::OnWebSocketConnectionError(const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("WebSocket connection error: %s"), *Error);
	
	if (bAutoReconnect && !bIsManualClose && !bIsReconnecting)
	{
		bIsReconnecting = true;
		ReconnectInnerThread = new FODWorkerThread([this] { return TryReconnect(); }, ReconnectDelay);
		ReconnectThread = FRunnableThread::Create(ReconnectInnerThread, TEXT("ObjectDeliverer WebSocket ReconnectThread"));
	}
}

void UProtocolWebSocketClient::OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogTemp, Log, TEXT("WebSocket closed: StatusCode=%d, Reason=%s, WasClean=%d"), StatusCode, *Reason, bWasClean);
	
	DispatchDisconnected(this);
	
	if (bAutoReconnect && !bIsManualClose && !bIsReconnecting)
	{
		bIsReconnecting = true;
		ReconnectInnerThread = new FODWorkerThread([this] { return TryReconnect(); }, ReconnectDelay);
		ReconnectThread = FRunnableThread::Create(ReconnectInnerThread, TEXT("ObjectDeliverer WebSocket ReconnectThread"));
	}
}

void UProtocolWebSocketClient::OnWebSocketMessage(const FString& Message)
{
	TArray<uint8> DataBuffer;
	DataBuffer.Append(reinterpret_cast<const uint8*>(TCHAR_TO_UTF8(*Message)), Message.Len());
	
	if (PacketRule)
	{
		PacketRule->NotifyReceiveData(DataBuffer);
	}
	else
	{
		DispatchReceiveData(this, DataBuffer);
	}
}

void UProtocolWebSocketClient::OnWebSocketBinaryMessage(const void* Data, SIZE_T Size, bool bIsLastFragment)
{
	if (!bIsLastFragment)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fragmented binary messages are not yet supported"));
		return;
	}

	TArray<uint8> DataBuffer;
	DataBuffer.Append(static_cast<const uint8*>(Data), Size);
	
	if (PacketRule)
	{
		PacketRule->NotifyReceiveData(DataBuffer);
	}
	else
	{
		DispatchReceiveData(this, DataBuffer);
	}
}

bool UProtocolWebSocketClient::TryReconnect()
{
	if (bIsManualClose)
	{
		bIsReconnecting = false;
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Attempting to reconnect..."));
	
	CreateWebSocket();
	bIsReconnecting = false;
	
	return false;
}