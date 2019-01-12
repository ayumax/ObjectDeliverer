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

bool UCNTcpIpServer::Start(int32 Port)
{
	Close();

	auto socket = FTcpSocketBuilder(TEXT("CommNet TcpIpServer"))
		.AsBlocking()
		.BoundToPort(Port)
		.Listening(10)
		.Build();

	if (socket == nullptr) return false;

	ListenerSocket = socket;

	auto worker = new FWorkerThread([this] { OnListen(); });
	ListenThread = FRunnableThread::Create(worker, TEXT("CommNet TcpIpSocket ListenThread"));
	

	return true;
}

void UCNTcpIpServer::Close()
{
	for (auto clientSocket : ConnectedSockets)
	{
		clientSocket->Close();
	}

	ConnectedSockets.Reset();

	if (!ListenerSocket) return;

	ListenThread->Kill(false);
	ListenerSocket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);

	ListenerSocket = nullptr;
}

void UCNTcpIpServer::BeginDestroy()
{
	Super::BeginDestroy();

	Close();
}


void UCNTcpIpServer::Send(const TArray<uint8>& DataBuffer)
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