#include "ProtocolTcpIpServer.h"
#include "TcpSocketBuilder.h"
#include "ProtocolTcpIpSocket.h"
#include "Utils/WorkerThread.h"
#include "PacketRule.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

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

void UProtocolTcpIpServer::Start_Implementation()
{
	Close_Implementation();

	auto socket = FTcpSocketBuilder(TEXT("ObjectDeliverer TcpIpServer"))
		.AsBlocking()
		.BoundToPort(ListenPort)
		.Listening(MaxBacklog)
		.Build();

	if (socket == nullptr) return;

	ListenerSocket = socket;

	ListenInnerThread = new FWorkerThread([this] { return OnListen(); });
	ListenThread = FRunnableThread::Create(ListenInnerThread, TEXT("ObjectDeliverer TcpIpSocket ListenThread"));
}

void UProtocolTcpIpServer::Close_Implementation()
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

void UProtocolTcpIpServer::Send_Implementation(const TArray<uint8>& DataBuffer)
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

void UProtocolTcpIpServer::DisconnectedClient(UObjectDelivererProtocol* ClientSocket)
{
	auto _clientSocket = Cast< UProtocolTcpIpSocket>(ClientSocket);
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

void UProtocolTcpIpServer::ReceiveDataFromClient(UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
{
	DispatchReceiveData(ClientSocket, Buffer);
}
