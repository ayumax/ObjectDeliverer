// Copyright 2019 ayumax. All Rights Reserved.
#include "ObjectDelivererManagerTestHelper.h"
#include "Protocol/ObjectDelivererProtocol.h"

void UObjectDelivererManagerTestHelper::OnConnect(const UObjectDelivererProtocol* ClientSocket)
{
	ConnectedSocket.Add(ClientSocket);
}

void UObjectDelivererManagerTestHelper::OnDisConnect(const UObjectDelivererProtocol* ClientSocket)
{
	DisconnectedSocket.Add(ClientSocket);
}

void UObjectDelivererManagerTestHelper::OnReceive(const UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
{
	ReceiveSocket = ClientSocket;
	ReceiveBuffers.Add(Buffer);
}

void UObjectDelivererManagerTestHelper::OnReceiveString(const FString& StringValue, const UObjectDelivererProtocol* FromObject)
{
	ReceiveStrings.Add(StringValue);
}