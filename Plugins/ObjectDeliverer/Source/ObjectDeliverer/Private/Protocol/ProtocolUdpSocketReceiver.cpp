#include "ProtocolUdpSocketReceiver.h"
#include "PacketRule.h"

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

void UProtocolUdpSocketReceiver::Start_Implementation()
{
	InnerSocket = FUdpSocketBuilder(TEXT("ObjectDeliverer UdpSocket"))
		.WithReceiveBufferSize(1024 * 1024)
		.BoundToPort(BoundPort)
		.Build();

	Receiver = new FUdpSocketReceiver(InnerSocket, FTimespan::FromMilliseconds(10), TEXT("UProtocolUdpSocketReceiver"));
	Receiver->OnDataReceived().BindUObject(this, &UProtocolUdpSocketReceiver::UdpReceivedCallback);
	Receiver->Start();

	if (InnerSocket)
	{
		DispatchConnected(this);
	}
}

void UProtocolUdpSocketReceiver::Close_Implementation()
{
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
	int64 receivedSize = data->TotalSize();
	TArray<uint8>& receiveBuffer = *(data.Get());

	auto wantSize = PacketRule->GetWantSize();

	TArray<uint8> BodyBuffer;

	if (wantSize < receivedSize)
	{
		TArray<uint8> SplitBuffer;
		int64 nowSize = receivedSize;

		while (nowSize >= PacketRule->GetWantSize())
		{
			SplitBuffer.SetNum(PacketRule->GetWantSize(), false);
			FMemory::Memcpy(SplitBuffer.GetData(), receiveBuffer.GetData() + (receivedSize - nowSize), SplitBuffer.Num());

			nowSize -= PacketRule->GetWantSize();

			PacketRule->NotifyReceiveData(SplitBuffer);
		}

	}
}