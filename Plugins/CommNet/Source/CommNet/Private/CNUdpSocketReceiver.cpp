#include "CNUdpSocketReceiver.h"

UCNUdpSocketReceiver::UCNUdpSocketReceiver()
{

}

UCNUdpSocketReceiver::~UCNUdpSocketReceiver()
{

}

void UCNUdpSocketReceiver::OnStart()
{
	Receiver = new FUdpSocketReceiver(InnerSocket, FTimespan::FromMicroseconds(1), TEXT("UCNUdpSocketReceiver"));
	Receiver->OnDataReceived().BindUObject(this, &UCNUdpSocketReceiver::UdpReceivedCallback);
}

void UCNUdpSocketReceiver::OnClose()
{
	if (Receiver)
	{
		delete Receiver;
		Receiver = nullptr;
	}
}

void UCNUdpSocketReceiver::BeginDestroy()
{
	Super::BeginDestroy();

	Close();
}

void UCNUdpSocketReceiver::UdpReceivedCallback(const FArrayReaderPtr& data, const FIPv4Endpoint& ip)
{
	int64 receivedSize = data->TotalSize();
	
	DispatchReceiveData(this, *(data.Get()), (int32)receivedSize);
}