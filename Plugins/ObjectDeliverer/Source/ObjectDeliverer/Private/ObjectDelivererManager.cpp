// Copyright 2019 ayumax. All Rights Reserved.
#include "ObjectDelivererManager.h"
#include "Protocol/ObjectDelivererProtocol.h"
#include "PacketRule/PacketRule.h"
#include "DeliveryBox/DeliveryBox.h"
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
	if (!Protocol || !PacketRule) return;

	CurrentProtocol = Protocol;
	CurrentProtocol->SetPacketRule(PacketRule);

	DeliveryBox = _DeliveryBox;
	if (DeliveryBox)
	{
		DeliveryBox->RequestSend.BindLambda([this](const UObjectDelivererProtocol* Destination, const TArray<uint8>& Buffer)
		{
			if (Destination)
			{
				SendTo(Buffer, Destination);
			}
			else
			{
				Send(Buffer);
			}
			
		});
	}

	CurrentProtocol->Connected.BindLambda([this](const UObjectDelivererProtocol* ConnectedObject) 
	{
		ConnectedList.Add(ConnectedObject);

		DispatchEvent([this, ConnectedObject]()
		{
			Connected.Broadcast(ConnectedObject);
		});
	});

	CurrentProtocol->Disconnected.BindLambda([this](const UObjectDelivererProtocol* DisconnectedObject)
	{
		if (ConnectedList.Contains(DisconnectedObject))
		{
			ConnectedList.Remove(DisconnectedObject);
		}

		DispatchEvent([this, DisconnectedObject]()
		{
			Disconnected.Broadcast(DisconnectedObject);
		});
	});

	CurrentProtocol->ReceiveData.BindLambda([this](const UObjectDelivererProtocol* FromObject, const TArray<uint8>& Buffer)
	{
		DispatchEvent([this, FromObject, Buffer]() {
			ReceiveData.Broadcast(FromObject, Buffer);

			if (DeliveryBox)
			{
				DeliveryBox->NotifyReceiveBuffer(FromObject, Buffer);
			}
		});
	});

	ConnectedList.SetNum(0);

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

void UObjectDelivererManager::SendTo(const TArray<uint8>& DataBuffer, const UObjectDelivererProtocol* Target)
{
	if (!CurrentProtocol) return;
	if (IsDestorying) return;

	Target->Send(DataBuffer);
}

bool UObjectDelivererManager::IsConnected()
{
	return ConnectedList.Num() > 0;
}

void UObjectDelivererManager::BeginDestroy()
{
	IsDestorying = true;

	Close();

	Super::BeginDestroy();

}