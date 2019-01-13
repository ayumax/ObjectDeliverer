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
	Close_Implementation();

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
	ListenerSocket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);
	ListenerSocket = nullptr;

	ListenThread->Kill(true);
	delete ListenThread;
	ListenThread = nullptr;

	delete ListenInnerThread;
	ListenInnerThread = nullptr;


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
			clientSocket->Disconnected.BindUObject(this, &UCNTcpIpServer::DisconnectedClient);
			clientSocket->ReceiveData.BindUObject(this, &UCNTcpIpServer::ReceiveDataFromClient);

			clientSocket->OnConnected(_clientSocket);

			ConnectedSockets.Add(clientSocket);

			DispatchConnected(clientSocket);
		}
	}
}

void UCNTcpIpServer::DisconnectedClient(UCommNetProtocol* ClientSocket)
{
	auto _clientSocket = Cast< UCNTcpIpSocket>(ClientSocket);
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

void UCNTcpIpServer::ReceiveDataFromClient(UCommNetProtocol* ClientSocket, const TArray<uint8>& Buffer, int32 Size)
{
	DispatchReceiveData(ClientSocket, Buffer, Size);
}