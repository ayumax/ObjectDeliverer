// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolUdpSocketReceiver.h"
#include "PacketRule/PacketRule.h"

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

		DispatchConnected(this);
	}
}

void UProtocolUdpSocketReceiver::Close()
{
	FScopeLock lock(&ct);

	if (Receiver)
	{
		Receiver->Stop();
		delete Receiver;
		Receiver = nullptr;
	}

	CloseInnerSocket();	
}

void UProtocolUdpSocketReceiver::UdpReceivedCallback(const FArrayReaderPtr& data, const FIPv4Endpoint& ip)
{
	FScopeLock lock(&ct);

	int64 receivedSize = data->TotalSize();
	TArray<uint8>& receiveBuffer = *(data.Get());

	auto wantSize = PacketRule->GetWantSize();
	if (wantSize == 0)
	{
		wantSize = receivedSize;
	}

	TArray<uint8> BodyBuffer;

	if (wantSize <= receivedSize)
	{
		TArray<uint8> SplitBuffer;
		int64 nowSize = receivedSize;

		while (nowSize >= wantSize)
		{
			SplitBuffer.SetNum(wantSize, false);
			FMemory::Memcpy(SplitBuffer.GetData(), receiveBuffer.GetData() + (receivedSize - nowSize), SplitBuffer.Num());

			nowSize -= wantSize;

			PacketRule->NotifyReceiveData(SplitBuffer);
		}

	}
}