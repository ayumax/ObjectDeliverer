#include "CommNetProtocol.h"

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