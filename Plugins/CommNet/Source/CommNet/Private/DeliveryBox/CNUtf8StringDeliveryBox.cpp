#include "CNUtf8StringDeliveryBox.h"
#include "Utils/CNStringUtil.h"

UCNUtf8StringDeliveryBox::UCNUtf8StringDeliveryBox()
{

}

UCNUtf8StringDeliveryBox::~UCNUtf8StringDeliveryBox()
{
}

void UCNUtf8StringDeliveryBox::Send(const FString& message)
{
	TArray<uint8> buffer;
	UCNStringUtil::StringToBuffer(message, buffer);

	RequestSend.ExecuteIfBound(buffer);
}

void UCNUtf8StringDeliveryBox::NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer)
{
	Received.Broadcast(UCNStringUtil::BufferToString(buffer));
}
