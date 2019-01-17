#include "CommNetManager.h"
#include "CommNetProtocol.h"
#include "CNPacketRule.h"
#include "CNDeliveryBox.h"

UCommNetManager::UCommNetManager()
{

}

UCommNetManager::~UCommNetManager()
{
}

void UCommNetManager::Start(UCommNetProtocol* Protocol, UCNPacketRule* PacketRule, UCNDeliveryBox* _DeliveryBox)
{
	CurrentProtocol = Protocol;
	CurrentProtocol->SetPacketRule(PacketRule);

	DeliveryBox = _DeliveryBox;
	if (DeliveryBox)
	{
		DeliveryBox->RequestSend.BindLambda([this](const TArray<uint8>& Buffer)
		{
			Send(Buffer);
		});
	}
	

	CurrentProtocol->Connected.BindLambda([this](UCommNetProtocol* ConnectedObject) 
	{
		Connected.Broadcast(ConnectedObject);
	});

	CurrentProtocol->Disconnected.BindLambda([this](UCommNetProtocol* DisconnectedObject)
	{
		Disconnected.Broadcast(DisconnectedObject);
	});

	CurrentProtocol->ReceiveData.BindLambda([this](UCommNetProtocol* FromObject, const TArray<uint8>& Buffer)
	{
		ReceiveData.Broadcast(FromObject, Buffer);

		if (DeliveryBox)
		{
			DeliveryBox->NotifyReceiveBuffer(Buffer);
		}
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