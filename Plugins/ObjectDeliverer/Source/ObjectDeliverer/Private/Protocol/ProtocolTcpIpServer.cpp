// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolTcpIpServer.h"
#include "Protocol/ProtocolTcpIpSocket.h"
#include "Common/TcpSocketBuilder.h"
#include "Utils/ODWorkerThread.h"
#include "PacketRule/PacketRule.h"
#include "HAL/RunnableThread.h"

UProtocolTcpIpServer::UProtocolTcpIpServer()
{

}

UProtocolTcpIpServer::~UProtocolTcpIpServer()
{

}

void UProtocolTcpIpServer::Initialize(int32 Port)
{
	ListenPort = Port;
}

UProtocolTcpIpServer* UProtocolTcpIpServer::WithReceiveBufferSize(int32 SizeInBytes)
{
	ReceiveBufferSize = SizeInBytes;

	return this;
}

UProtocolTcpIpServer* UProtocolTcpIpServer::WithSendBufferSize(int32 SizeInBytes)
{
	SendBufferSize = SizeInBytes;

	return this;
}

void UProtocolTcpIpServer::Start()
{
	Close();

	auto socket = FTcpSocketBuilder(TEXT("ObjectDeliverer TcpIpServer"))
		.AsBlocking()
		.BoundToPort(ListenPort)
		.Listening(MaxBacklog)
		.Build();

	if (socket == nullptr) return;

	ListenerSocket = socket;

	ListenInnerThread = new FODWorkerThread([this] { return OnListen(); });
	ListenThread = FRunnableThread::Create(ListenInnerThread, TEXT("ObjectDeliverer TcpIpSocket ListenThread"));
}

void UProtocolTcpIpServer::Close()
{
	for (auto clientSocket : ConnectedSockets)
	{
		clientSocket->Disconnected.Unbind();
		clientSocket->ReceiveData.Unbind();
		clientSocket->Close();
	}

	ConnectedSockets.Reset();

	if (!ListenerSocket) return;
	ListenerSocket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);

	if (!ListenThread) return;
	ListenThread->Kill(true);
	delete ListenThread;
	ListenThread = nullptr;

	if (!ListenInnerThread) return;
	delete ListenInnerThread;
	ListenInnerThread = nullptr;

	ListenerSocket = nullptr;

}

void UProtocolTcpIpServer::Send(const TArray<uint8>& DataBuffer) const
{
	for (auto clientSocket : ConnectedSockets)
	{
		clientSocket->Send(DataBuffer);
	}
}

bool UProtocolTcpIpServer::OnListen()
{
	if (!ListenerSocket) return false;

	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending = false;

	if (ListenerSocket->HasPendingConnection(Pending))
	{
		auto _clientSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("ObjectDeliverer Received Socket Connection"));

		if (_clientSocket != nullptr)
		{
			int32 _newReceiveBufferSize;
			_clientSocket->SetReceiveBufferSize(ReceiveBufferSize, _newReceiveBufferSize);
			int32 _newSendBufferSize;
			_clientSocket->SetSendBufferSize(SendBufferSize, _newSendBufferSize);

			auto clientSocket = NewObject<UProtocolTcpIpSocket>();
			clientSocket->Disconnected.BindUObject(this, &UProtocolTcpIpServer::DisconnectedClient);
			clientSocket->ReceiveData.BindUObject(this, &UProtocolTcpIpServer::ReceiveDataFromClient);
			clientSocket->SetPacketRule(PacketRule->Clone());

			clientSocket->OnConnected(_clientSocket);

			ConnectedSockets.Add(clientSocket);

			DispatchConnected(clientSocket);
		}
	}

	return true;
}

void UProtocolTcpIpServer::DisconnectedClient(const UObjectDelivererProtocol* ClientSocket)
{
	auto _clientSocket = (UProtocolTcpIpSocket*)(ClientSocket);
	if (!IsValid(_clientSocket)) return;

	auto foundIndex = ConnectedSockets.Find(_clientSocket);
	if (foundIndex != INDEX_NONE)
	{
		_clientSocket->Disconnected.Unbind();
		_clientSocket->ReceiveData.Unbind();
		
		ConnectedSockets.RemoveAt(foundIndex);

		DispatchDisconnected(ClientSocket);
	}
}

void UProtocolTcpIpServer::ReceiveDataFromClient(const UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
{
	DispatchReceiveData(ClientSocket, Buffer);
}
