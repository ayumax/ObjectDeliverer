// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolSocketBase.h"
#include "Misc/OutputDeviceRedirector.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

UProtocolSocketBase::UProtocolSocketBase()
{

}

UProtocolSocketBase::~UProtocolSocketBase()
{

}

void UProtocolSocketBase::CloseInnerSocket()
{
	if (!InnerSocket) return;

	InnerSocket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(InnerSocket);
	InnerSocket = nullptr;
}

void UProtocolSocketBase::SendTo(const TArray<uint8>& DataBuffer, const FIPv4Endpoint& EndPoint) const 
{
	if (!InnerSocket) return;

	int32 BytesSent;
	InnerSocket->SendTo(DataBuffer.GetData(), DataBuffer.Num(), BytesSent, EndPoint.ToInternetAddr().Get());
}

void UProtocolSocketBase::SendToConnected(const TArray<uint8>& DataBuffer) const
{
	if (!InnerSocket) return;

	int32 BytesSent;
	InnerSocket->Send(DataBuffer.GetData(), DataBuffer.Num(), BytesSent);
}


bool UProtocolSocketBase::FormatIP4ToNumber(const FString& IpAddress, uint8(&Out)[4])
{
	auto _ip = IpAddress.ToLower();
	if (_ip == TEXT("localhost"))
	{
		Out[0] = 127;
		Out[1] = 0;
		Out[2] = 0;
		Out[3] = 1;
		return true;
	}

	_ip = _ip.Replace(TEXT(" "), TEXT(""));

	const TCHAR* Delims[] = { TEXT(".") };
	TArray<FString> Parts;
	_ip.ParseIntoArray(Parts, Delims, true);
	if (Parts.Num() != 4)
		return false;

	for (int32 i = 0; i < 4; ++i)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}

	return true;
}

TTuple<bool, FIPv4Endpoint> UProtocolSocketBase::GetIP4EndPoint(const FString& IpAddress, int32 Port)
{
	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(IpAddress, IP4Nums))
	{
		UE_LOG(LogTemp, Error, TEXT("UProtocolTcpIpSocket::ipaddress format violation"));
		return MakeTuple(false, FIPv4Endpoint());
	}

	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), Port);

	return MakeTuple(true, Endpoint);
}