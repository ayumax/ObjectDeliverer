// Copyright 2019 ayumax. All Rights Reserved.
#include "ObjectDelivererManager.h"
#include "ObjectDelivererProtocol.h"
#include "PacketRule.h"
#include "DeliveryBox.h"
#include "Async/Async.h"
#include "Misc/ScopeLock.h"

static FCriticalSection LockObj;

UObjectDelivererManager* UObjectDelivererManager::CreateObjectDelivererManager(bool _IsEventWithGameThread /* = true*/)
{
	auto manager = NewObject<UObjectDelivererManager>();
	manager->IsEventWithGameThread = _IsEventWithGameThread;
	return manager;
}

UObjectDelivererManager::UObjectDelivererManager()
	: IsEventWithGameThread(true)
	, IsDestorying(false)
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
		DispatchEvent([this, ConnectedObject]()
		{
			Connected.Broadcast(ConnectedObject);
		});
	});

	CurrentProtocol->Disconnected.BindLambda([this](UObjectDelivererProtocol* DisconnectedObject)
	{
		DispatchEvent([this, DisconnectedObject]()
		{
			Disconnected.Broadcast(DisconnectedObject);
		});
		
	});

	CurrentProtocol->ReceiveData.BindLambda([this](UObjectDelivererProtocol* FromObject, const TArray<uint8>& Buffer)
	{
		DispatchEvent([this, FromObject, Buffer]() {
			ReceiveData.Broadcast(FromObject, Buffer);

			if (DeliveryBox)
			{
				DeliveryBox->NotifyReceiveBuffer(Buffer);
			}
		});
	});

	CurrentProtocol->Start();
}

void UObjectDelivererManager::DispatchEvent(TFunction<void()> EventAction)
{
	if (IsDestorying) return;
	if (!IsValid(this)) return;

	if (IsEventWithGameThread)
	{
		AsyncTask(ENamedThreads::GameThread, [this, EventAction]() {
			if (IsDestorying) return;
			EventAction();
		});
	}
	else
	{
		EventAction();
	}
}

void UObjectDelivererManager::Close()
{
	if (!IsValid(CurrentProtocol)) return;

	if (DeliveryBox)
	{
		if (DeliveryBox->RequestSend.IsBound())
		{
			DeliveryBox->RequestSend.Unbind();
		}
	}

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
	
	CurrentProtocol->Close();

	CurrentProtocol = nullptr;
}

void UObjectDelivererManager::Send(const TArray<uint8>& DataBuffer)
{
	if (!CurrentProtocol) return;
	if (IsDestorying) return;

	CurrentProtocol->Send(DataBuffer);
}


void UObjectDelivererManager::BeginDestroy()
{
	IsDestorying = true;

	Close();

	Super::BeginDestroy();

}