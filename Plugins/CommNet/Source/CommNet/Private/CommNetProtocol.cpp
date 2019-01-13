#include "CommNetProtocol.h"
#include "CNPacketRule.h"

UCommNetProtocol::UCommNetProtocol()
{

}

UCommNetProtocol::~UCommNetProtocol()
{
}

void UCommNetProtocol::Start_Implementation()
{
}

void UCommNetProtocol::Close_Implementation()
{

}

void UCommNetProtocol::Send_Implementation(const TArray<uint8>& DataBuffer)
{

}

void UCommNetProtocol::DispatchConnected(UCommNetProtocol* ConnectedObject)
{
	Connected.ExecuteIfBound(ConnectedObject);
}

void UCommNetProtocol::DispatchDisconnected(UCommNetProtocol* DisconnectedObject)
{
	Disconnected.ExecuteIfBound(DisconnectedObject);
}

void UCommNetProtocol::DispatchReceiveData(UCommNetProtocol* FromObject, const TArray<uint8>& Buffer)
{
	ReceiveData.ExecuteIfBound(FromObject, Buffer);
}

void UCommNetProtocol::BeginDestroy()
{
	Super::BeginDestroy();

	Close_Implementation();
}

void UCommNetProtocol::SetPacketRule(UCNPacketRule* _PacketRule)
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

void UCommNetProtocol::RequestSend(const TArray<uint8>& DataBuffer)
{

}