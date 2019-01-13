#include "CommNetManager.h"
#include "CommNetProtocol.h"

UCommNetManager::UCommNetManager()
{

}

UCommNetManager::~UCommNetManager()
{
}

void UCommNetManager::Start(UCommNetProtocol* Protocol)
{
	CurrentProtocol = Protocol;

	CurrentProtocol->Connected.BindLambda([this](UCommNetProtocol* ConnectedObject) 
	{
		Connected.Broadcast(ConnectedObject);
	});

	CurrentProtocol->Disconnected.BindLambda([this](UCommNetProtocol* DisconnectedObject)
	{
		Connected.Broadcast(DisconnectedObject);
	});

	CurrentProtocol->ReceiveData.BindLambda([this](UCommNetProtocol* FromObject, const TArray<uint8>& Buffer, int32 Size)
	{
		ReceiveData.Broadcast(FromObject, Buffer, Size);
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
	
	CurrentProtocol->Close();

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