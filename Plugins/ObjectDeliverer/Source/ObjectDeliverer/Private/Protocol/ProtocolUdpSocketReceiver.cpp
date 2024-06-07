// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolUdpSocketReceiver.h"
#include "PacketRule/PacketRule.h"
#include "Protocol/ProtocolUdpSocket.h"
#include "Utils/ODWorkerThread.h"
#include "HAL/RunnableThread.h"
#include "Common/UdpSocketBuilder.h"

UProtocolUdpSocketReceiver::UProtocolUdpSocketReceiver()
{

}

UProtocolUdpSocketReceiver::~UProtocolUdpSocketReceiver()
{

}	

void UProtocolUdpSocketReceiver::InitializeWithReceiver(int32 _BoundPort)
{
	BoundPort = _BoundPort;
}

UProtocolUdpSocketReceiver* UProtocolUdpSocketReceiver::WithReceiveBufferSize(int32 SizeInBytes)
{
	ReceiveBufferSize = SizeInBytes;

	return this;
}


void UProtocolUdpSocketReceiver::Start()
{
	ReceiveBuffer.SetLength(0);

	InnerSocket = FUdpSocketBuilder(TEXT("ObjectDeliverer UdpSocket"))
		.WithReceiveBufferSize(ReceiveBufferSize)
		.BoundToPort(BoundPort)
		.Build();

	if (InnerSocket)
	{
		SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

		CurrentInnerThread = new FODWorkerThread([this]
			{
				FScopeLock lock(&ct);
				return ReceivedData();
			});
		CurrentThread = FRunnableThread::Create(CurrentInnerThread, TEXT("ObjectDeliverer UDPSocket PollingThread"));

		ConnectedSockets.Reset();

		DispatchConnected(this);
	}
}

void UProtocolUdpSocketReceiver::Close()
{
	if (!InnerSocket) return;

	IsSelfClose = true;

	InnerSocket->Close();

	FScopeLock lock(&ct);

	if (!CurrentThread) return;
	CurrentThread->Kill(true);

	delete CurrentThread;
	CurrentThread = nullptr;

	if (!CurrentInnerThread) return;
	delete CurrentInnerThread;
	CurrentInnerThread = nullptr;

	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(InnerSocket);
	InnerSocket = nullptr;
}


bool UProtocolUdpSocketReceiver::ReceivedData()
{
	if (!InnerSocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromMilliseconds(10)))
	{
		return true;
	}

	uint32 Size = 0;
	while (InnerSocket->HasPendingData(Size))
	{
		ReceiveBuffer.SetLength(Size);

		TSharedRef<FInternetAddr> Sender = SocketSubsystem->CreateInternetAddr();
		int32 Read = 0;

		if (!InnerSocket->RecvFrom(ReceiveBuffer.AsSpan().Buffer, ReceiveBuffer.GetLength(), Read, *Sender))
		{
			if (!IsSelfClose)
			{
				CloseInnerSocket();
				DispatchDisconnected(this);
			}
			return false;
		}

		ReceiveBuffer.SetLength(Read);

		if (ReceiveBuffer.GetLength() > 0)
		{
			auto ip = FIPv4Endpoint(Sender);

			if (!ConnectedSockets.Contains(ip))
			{
				auto udpSender = NewObject<UProtocolUdpSocket>(this);
				udpSender->Initialize(ip);
				udpSender->SetPacketRule(PacketRule->Clone());
				udpSender->ReceiveData.BindUObject(this, &UProtocolUdpSocketReceiver::ReceiveDataFromClient);
				ConnectedSockets.Add(ip, udpSender);
			}

			FScopeLock lock(&ct);
			ConnectedSockets[ip]->NotifyReceived(ReceiveBuffer.AsSpan());

			ReceiveBuffer.Clear();
		}
	}

	return true;
}


void UProtocolUdpSocketReceiver::ReceiveDataFromClient(const UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
{
	DispatchReceiveData(ClientSocket, Buffer);
}

