#include "CNUdpSocketReceiver.h"
#include "CNPacketRule.h"

UCNUdpSocketReceiver::UCNUdpSocketReceiver()
{

}

UCNUdpSocketReceiver::~UCNUdpSocketReceiver()
{

}	

void UCNUdpSocketReceiver::InitializeWithReceiver(int32 _BoundPort)
{
	BoundPort = _BoundPort;
}

void UCNUdpSocketReceiver::Start_Implementation()
{
	InnerSocket = FUdpSocketBuilder(TEXT("CommNet UdpSocket"))
		.WithReceiveBufferSize(1024 * 1024)
		.BoundToPort(BoundPort)
		.Build();

	Receiver = new FUdpSocketReceiver(InnerSocket, FTimespan::FromMilliseconds(10), TEXT("UCNUdpSocketReceiver"));
	Receiver->OnDataReceived().BindUObject(this, &UCNUdpSocketReceiver::UdpReceivedCallback);
	Receiver->Start();
}

void UCNUdpSocketReceiver::Close_Implementation()
{
	if (Receiver)
	{
		Receiver->Stop();
		delete Receiver;
		Receiver = nullptr;
	}

	CloseInnerSocket();	
}

void UCNUdpSocketReceiver::UdpReceivedCallback(const FArrayReaderPtr& data, const FIPv4Endpoint& ip)
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