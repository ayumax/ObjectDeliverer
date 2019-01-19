#include "ObjectDelivererManager.h"
#include "ObjectDelivererProtocol.h"
#include "PacketRule.h"
#include "DeliveryBox.h"

UObjectDelivererManager::UObjectDelivererManager()
{

}

UObjectDelivererManager::~UObjectDelivererManager()
{
}

void UObjectDelivererManager::Start(UObjectDelivererProtocol* Protocol, UPacketRule* PacketRule, UDeliveryBox* _DeliveryBox)
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
	

	CurrentProtocol->Connected.BindLambda([this](UObjectDelivererProtocol* ConnectedObject) 
	{
		Connected.Broadcast(ConnectedObject);
	});

	CurrentProtocol->Disconnected.BindLambda([this](UObjectDelivererProtocol* DisconnectedObject)
	{
		Disconnected.Broadcast(DisconnectedObject);
	});

	CurrentProtocol->ReceiveData.BindLambda([this](UObjectDelivererProtocol* FromObject, const TArray<uint8>& Buffer)
	{
		ReceiveData.Broadcast(FromObject, Buffer);

		if (DeliveryBox)
		{
			DeliveryBox->NotifyReceiveBuffer(Buffer);
		}
	});


	CurrentProtocol->Start();
}

void UObjectDelivererManager::Close()
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

void UObjectDelivererManager::Send(const TArray<uint8>& DataBuffer)
{
	if (!CurrentProtocol) return;

	CurrentProtocol->Send(DataBuffer);
}


void UObjectDelivererManager::BeginDestroy()
{
	Super::BeginDestroy();

	Close();
}