#include "ObjectDeliveryBox.h"
#include "Utils/StringUtil.h"

UObjectDeliveryBox::UObjectDeliveryBox()
{

}

UObjectDeliveryBox::~UObjectDeliveryBox()
{
}

void UObjectDeliveryBox::Initialize(UClass* TargetClass)
{
	DeliveryObject = NewObject<UObject>((UObject*)GetTransientPackage(), TargetClass);

	for (TFieldIterator<UProperty> PropIt(TargetClass, EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		UProperty* Property = *PropIt;
		FString PropertyName = Property->GetName();
		if (!PropertyMap.Contains(PropertyName))
		{
			PropertyMap.Add(PropertyName, Property);
		}

		
	}
}

void UObjectDeliveryBox::Send_Implementation(const UObject* message)
{
	
}

void UObjectDeliveryBox::NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer)
{
	
}

//uint8* CurrentPropAddr = It->ContainerPtrToValuePtr<uint8>(Buffer);
// ((UBoolProperty*)*It)->SetPropertyValue( CurrentPropAddr, true );
