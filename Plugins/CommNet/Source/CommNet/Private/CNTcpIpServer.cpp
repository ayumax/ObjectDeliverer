#include "CNTcpIpServer.h"
#include "TcpSocketBuilder.h"
#include "CNTcpIpSocket.h"
#include "WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

UCNTcpIpServer::UCNTcpIpServer()
{

}

UCNTcpIpServer::~UCNTcpIpServer()
{

}

void UCNTcpIpServer::Initialize(int32 Port)
{
	ListenPort = Port;
}

void UCNTcpIpServer::Start_Implementation()
{
	Close();

	auto socket = FTcpSocketBuilder(TEXT("CommNet TcpIpServer"))
		.AsBlocking()
		.BoundToPort(ListenPort)
		.Listening(MaxBacklog)
		.Build();

	if (socket == nullptr) return;

	ListenerSocket = socket;

	ListenInnerThread = new FWorkerThread([this] { OnListen(); });
	ListenThread = FRunnableThread::Create(ListenInnerThread, TEXT("CommNet TcpIpSocket ListenThread"));
}

void UCNTcpIpServer::Close_Implementation()
{
	for (auto clientSocket : ConnectedSockets)
	{
		clientSocket->Close();
	}

	ConnectedSockets.Reset();

	if (!ListenerSocket) return;

	ListenThread->Kill(false);
	delete ListenThread;
	ListenThread = nullptr;

	delete ListenInnerThread;
	ListenInnerThread = nullptr;

	ListenerSocket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);

	ListenerSocket = nullptr;
}

void UCNTcpIpServer::BeginDestroy()
{
	Super::BeginDestroy();

	Close();
}


void UCNTcpIpServer::Send_Implementation(const TArray<uint8>& DataBuffer)
{
	for (auto clientSocket : ConnectedSockets)
	{
		clientSocket->Send(DataBuffer);
	}
}

void UCNTcpIpServer::OnListen()
{
	if (!ListenerSocket) return;

	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending = false;

	if (ListenerSocket->HasPendingConnection(Pending))
	{
		auto _clientSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("CommNet Received Socket Connection"));

		if (_clientSocket != nullptr)
		{
			auto clientSocket = NewObject<UCNTcpIpSocket>();
			clientSocket->Disconnected.AddDynamic(this, &UCNTcpIpServer::DisconnectedClient);
			clientSocket->ReceiveData.AddDynamic(this, &UCNTcpIpServer::ReceiveDataFromClient);

			clientSocket->OnConnected(_clientSocket);

			ConnectedSockets.Add(clientSocket);

			Connected.Broadcast(clientSocket);
		}
	}
}

void UCNTcpIpServer::DisconnectedClient(UCNTcpIpSocket* ClientSocket)
{
	auto foundIndex = ConnectedSockets.Find(ClientSocket);
	if (foundIndex != INDEX_NONE)
	{
		ClientSocket->Disconnected.RemoveDynamic(this, &UCNTcpIpServer::DisconnectedClient);
		ClientSocket->ReceiveData.RemoveDynamic(this, &UCNTcpIpServer::ReceiveDataFromClient);
		
		ConnectedSockets.RemoveAt(foundIndex);

		Disconnected.Broadcast(ClientSocket);
	}
}

void UCNTcpIpServer::ReceiveDataFromClient(UCNTcpIpSocket* ClientSocket, const TArray<uint8>& Buffer, int32 Size)
{
	ReceiveData.Broadcast(ClientSocket, Buffer, Size);
}