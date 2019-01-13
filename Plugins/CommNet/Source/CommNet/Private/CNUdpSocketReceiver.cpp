#include "CNUdpSocketReceiver.h"
#include "CNPacketRule.h"

UCNUdpSocketReceiver::UCNUdpSocketReceiver()
{

}

UCNUdpSocketReceiver::~UCNUdpSocketReceiver()
{

}	

void UCNUdpSocketReceiver::InitializeWithReceiver(const FString& IpAddress, int32 Port, int32 _BoundPort)
{
	Super::Initialize(IpAddress, Port);

	BoundPort = _BoundPort;
}

void UCNUdpSocketReceiver::Start_Implementation()
{
	auto endPoint = GetIP4EndPoint(DestinationIpAddress, DestinationPort);
	if (!endPoint.Get<0>()) return;

	DestinationEndpoint = endPoint.Get<1>();

	InnerSocket = FUdpSocketBuilder(TEXT("CommNet UdpSocket"))
		.AsBlocking()
		.WithReceiveBufferSize(1024 * 1024)
		.BoundToPort(BoundPort)
		.Build();

	Receiver = new FUdpSocketReceiver(InnerSocket, FTimespan::FromMicroseconds(1), TEXT("UCNUdpSocketReceiver"));
	Receiver->OnDataReceived().BindUObject(this, &UCNUdpSocketReceiver::UdpReceivedCallback);
}

void UCNUdpSocketReceiver::Close_Implementation()
{
	Super::Close_Implementation();

	if (Receiver)
	{
		delete Receiver;
		Receiver = nullptr;
	}
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

		while (nowSize > PacketRule->GetWantSize())
		{
			SplitBuffer.SetNum(PacketRule->GetWantSize(), false);
			FMemory::Memcpy(SplitBuffer.GetData() + (receivedSize - nowSize), receiveBuffer.GetData(), SplitBuffer.Num());

			nowSize -= PacketRule->GetWantSize();

			if (PacketRule->NotifyReceiveData(SplitBuffer, BodyBuffer))
			{
				DispatchReceiveData(this, BodyBuffer);
			}
		}

	}
}