#include "ObjectDelivererProtocol.h"
#include "PacketRule.h"

UObjectDelivererProtocol::UObjectDelivererProtocol()
{

}

UObjectDelivererProtocol::~UObjectDelivererProtocol()
{
}

void UObjectDelivererProtocol::Start_Implementation()
{
}

void UObjectDelivererProtocol::Close_Implementation()
{

}

void UObjectDelivererProtocol::Send_Implementation(const TArray<uint8>& DataBuffer)
{

}

void UObjectDelivererProtocol::DispatchConnected(UObjectDelivererProtocol* ConnectedObject)
{
	Connected.ExecuteIfBound(ConnectedObject);
}

void UObjectDelivererProtocol::DispatchDisconnected(UObjectDelivererProtocol* DisconnectedObject)
{
	Disconnected.ExecuteIfBound(DisconnectedObject);
}

void UObjectDelivererProtocol::DispatchReceiveData(UObjectDelivererProtocol* FromObject, const TArray<uint8>& Buffer)
{
	ReceiveData.ExecuteIfBound(FromObject, Buffer);
}

void UObjectDelivererProtocol::BeginDestroy()
{
	Super::BeginDestroy();

	Close();
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