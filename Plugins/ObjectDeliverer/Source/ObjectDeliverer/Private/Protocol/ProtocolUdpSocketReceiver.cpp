// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolUdpSocketReceiver.h"
#include "PacketRule/PacketRule.h"
#include "Protocol/ProtocolUdpSocket.h"

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

void UProtocolUdpSocketReceiver::Start()
{
	InnerSocket = FUdpSocketBuilder(TEXT("ObjectDeliverer UdpSocket"))
		.WithReceiveBufferSize(1024 * 1024)
		.BoundToPort(BoundPort)
		.Build();

	if (InnerSocket)
	{
		Receiver = new FUdpSocketReceiver(InnerSocket, FTimespan::FromMilliseconds(10), TEXT("UProtocolUdpSocketReceiver"));
		Receiver->OnDataReceived().BindUObject(this, &UProtocolUdpSocketReceiver::UdpReceivedCallback);
		Receiver->Start();

		ConnectedSockets.Reset();

		DispatchConnected(this);
	}
}

void UProtocolUdpSocketReceiver::Close()
{
	if (!InnerSocket) return;

	InnerSocket->Close();

	FScopeLock lock(&ct);

	if (Receiver)
	{
		Receiver->Stop();
		delete Receiver;
		Receiver = nullptr;
	}

	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(InnerSocket);
	InnerSocket = nullptr;
}

void UProtocolUdpSocketReceiver::UdpReceivedCallback(const FArrayReaderPtr& data, const FIPv4Endpoint& ip)
{
	if (!ConnectedSockets.Contains(ip))
	{
		auto udpSender = NewObject<UProtocolUdpSocket>(this);
		udpSender->Initialize(ip);
		udpSender->SetPacketRule(PacketRule->Clone());
		udpSender->ReceiveData.BindUObject(this, &UProtocolUdpSocketReceiver::ReceiveDataFromClient);
		ConnectedSockets.Add(ip, udpSender);
	}

	FScopeLock lock(&ct);
	ConnectedSockets[ip]->NotifyReceived(data);

}


void UProtocolUdpSocketReceiver::ReceiveDataFromClient(const UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
{
	DispatchReceiveData(ClientSocket, Buffer);
}

