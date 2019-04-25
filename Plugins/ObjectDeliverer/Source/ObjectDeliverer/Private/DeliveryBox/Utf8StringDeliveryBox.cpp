// Copyright 2019 ayumax. All Rights Reserved.
#include "Utf8StringDeliveryBox.h"
#include "Utils/StringUtil.h"

UUtf8StringDeliveryBox::UUtf8StringDeliveryBox()
{

}

UUtf8StringDeliveryBox::~UUtf8StringDeliveryBox()
{
}

void UUtf8StringDeliveryBox::Send(const FString& message)
{
	SendTo(message, nullptr);
}

void UUtf8StringDeliveryBox::SendTo(const FString& message, const UObjectDelivererProtocol* Destination)
{
	TArray<uint8> buffer;
	UStringUtil::StringToBuffer(message, buffer);

	RequestSend.ExecuteIfBound(Destination, buffer);
}

void UUtf8StringDeliveryBox::NotifyReceiveBuffer(const UObjectDelivererProtocol* FromObject, const TArray<uint8>& buffer)
{
	Received.Broadcast(UStringUtil::BufferToString(buffer), FromObject);
}
