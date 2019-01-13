#include "CommNetManager.h"
#include "CommNetProtocol.h"


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

	CurrentProtocol->Close();

	CurrentProtocol->Connected.Unbind();
	CurrentProtocol->Disconnected.Unbind();
	CurrentProtocol->ReceiveData.Unbind();

	CurrentProtocol = nullptr;
}

void UCommNetManager::Send(const TArray<uint8>& DataBuffer)
{
	if (!CurrentProtocol) return;

	CurrentProtocol->Send(DataBuffer);
}

