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
}

void UProtocolUdpSocket::NotifyReceived(const FArrayReaderPtr& data)
{
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
