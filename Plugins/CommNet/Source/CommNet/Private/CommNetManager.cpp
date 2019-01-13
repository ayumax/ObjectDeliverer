#include "CommNetManager.h"
#include "CommNetProtocol.h"
#include "CNPacketRule.h"

UCommNetManager::UCommNetManager()
{

}

UCommNetManager::~UCommNetManager()
{
}

void UCommNetManager::Start(UCommNetProtocol* Protocol, UCNPacketRule* PacketRule)
{
	CurrentProtocol = Protocol;
	CurrentProtocol->PacketRule = PacketRule;

	CurrentProtocol->Connected.BindLambda([this](UCommNetProtocol* ConnectedObject) 
	{
		Connected.Broadcast(ConnectedObject);
	});

	CurrentProtocol->Disconnected.BindLambda([this](UCommNetProtocol* DisconnectedObject)
	{
		Connected.Broadcast(DisconnectedObject);
	});

	CurrentProtocol->ReceiveData.BindLambda([this](UCommNetProtocol* FromObject, const TArray<uint8>& Buffer)
	{
		ReceiveData.Broadcast(FromObject, Buffer);
	});


	CurrentProtocol->Start();
}

void UCommNetManager::Close()
{
	if (!CurrentProtocol) return;
	if (!IsValid(CurrentProtocol)) return;

	if (CurrentProtocol->Connected.IsBound())
	{
		CurrentProtocol->Connected.Unbind();
	}
	
	if (CurrentProtocol->Disconnected.IsBound())
	{
		CurrentProtocol->Disconnected.Unbind();
	}
	
	if (CurrentProtocol->ReceiveData.IsBound())
	{
		CurrentProtocol->ReceiveData.Unbind();
	}
	
	CurrentProtocol->Close_Implementation();

	CurrentProtocol = nullptr;
}

void UCommNetManager::Send(const TArray<uint8>& DataBuffer)
{
	if (!CurrentProtocol) return;

	CurrentProtocol->Send(DataBuffer);
}


void UCommNetManager::BeginDestroy()
{
	Super::BeginDestroy();

	Close();
}