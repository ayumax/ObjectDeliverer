// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolUdpSocket.h"
#include "PacketRule/PacketRule.h"

UProtocolUdpSocket::UProtocolUdpSocket()
{

}

UProtocolUdpSocket::~UProtocolUdpSocket()
{

}	

void UProtocolUdpSocket::Initialize(FIPv4Endpoint IP)
{
	IPEndPoint = IP;
	ReceiveBuffer.SetBufferSize(1024);
}

void UProtocolUdpSocket::NotifyReceived(const FArrayReaderPtr& data)
{
	TArray<uint8>& receiveBuffer = *(data.Get());

	ReceiveBuffer.Add(ODByteSpan(receiveBuffer));

	while (ReceiveBuffer.GetLength() > 0)
	{
		const int32 wantSize = PacketRule->GetWantSize();

		if (wantSize > 0)
		{
			if (ReceiveBuffer.GetLength() < wantSize) return;
		}

		const auto receiveSize = wantSize == 0 ? ReceiveBuffer.GetLength() : wantSize;

		PacketRule->NotifyReceiveData(ReceiveBuffer.AsSpan(0, receiveSize).ToArray());

		ReceiveBuffer.RemoveRangeFromStart(0, receiveSize);
	}
	
}


bool UProtocolUdpSocket::GetIPAddress(TArray<uint8>& IPAddress)
{
	IPAddress.SetNum(0);
	IPAddress.Add(IPEndPoint.Address.A);
	IPAddress.Add(IPEndPoint.Address.B);
	IPAddress.Add(IPEndPoint.Address.C);
	IPAddress.Add(IPEndPoint.Address.D);

	return true;
}

bool UProtocolUdpSocket::GetIPAddressInString(FString& IPAddress)
{
	IPAddress = IPEndPoint.Address.ToString();

	return true;
}
