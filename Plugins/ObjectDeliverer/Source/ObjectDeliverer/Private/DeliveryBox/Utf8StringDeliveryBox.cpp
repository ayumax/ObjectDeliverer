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
	TArray<uint8> buffer;
	UStringUtil::StringToBuffer(message, buffer);

	RequestSend.ExecuteIfBound(buffer);
}

void UUtf8StringDeliveryBox::NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer)
{
	Received.Broadcast(UStringUtil::BufferToString(buffer));
}
