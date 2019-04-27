// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ObjectDelivererProtocol.h"
#include "PacketRule/PacketRule.h"

UObjectDelivererProtocol::UObjectDelivererProtocol()
{

}

UObjectDelivererProtocol::~UObjectDelivererProtocol()
{
}

void UObjectDelivererProtocol::Start()
{
}

void UObjectDelivererProtocol::Close()
{

}

void UObjectDelivererProtocol::Send(const TArray<uint8>& DataBuffer) const
{

}

void UObjectDelivererProtocol::DispatchConnected(const UObjectDelivererProtocol* ConnectedObject)
{
	Connected.ExecuteIfBound(ConnectedObject);
}

void UObjectDelivererProtocol::DispatchDisconnected(const UObjectDelivererProtocol* DisconnectedObject)
{
	Disconnected.ExecuteIfBound(DisconnectedObject);
}

void UObjectDelivererProtocol::DispatchReceiveData(const UObjectDelivererProtocol* FromObject, const TArray<uint8>& Buffer)
{
	ReceiveData.ExecuteIfBound(FromObject, Buffer);
}

void UObjectDelivererProtocol::BeginDestroy()
{
	Close();

	Super::BeginDestroy();
}

void UObjectDelivererProtocol::SetPacketRule(UPacketRule* _PacketRule)
{
	PacketRule = _PacketRule;
	PacketRule->Initialize();

	PacketRule->MadeSendBuffer.BindLambda([this](const TArray<uint8>& Buffer)
	{
		RequestSend(Buffer);
	});

	PacketRule->MadeReceiveBuffer.BindLambda([this](const TArray<uint8>& Buffer)
	{
		DispatchReceiveData(this, Buffer);
	});
}

void UObjectDelivererProtocol::RequestSend(const TArray<uint8>& DataBuffer)
{

}