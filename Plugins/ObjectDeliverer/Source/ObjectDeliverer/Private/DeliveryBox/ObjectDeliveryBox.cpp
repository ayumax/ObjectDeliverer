#include "ObjectDeliveryBox.h"
#include "Utils/StringUtil.h"

UObjectDeliveryBox::UObjectDeliveryBox()
{

}

UObjectDeliveryBox::~UObjectDeliveryBox()
{
}

void UObjectDeliveryBox::Initialize(UClass* _TargetClass)
{
	TargetClass = _TargetClass;
}

void UObjectDeliveryBox::Send_Implementation(const UObject* message)
{
	
}

void UObjectDeliveryBox::NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer)
{
	
}

