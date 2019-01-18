#include "CNObjectDeliveryBox.h"
#include "Utils/CNStringUtil.h"

UCNObjectDeliveryBox::UCNObjectDeliveryBox()
{

}

UCNObjectDeliveryBox::~UCNObjectDeliveryBox()
{
}

void UCNObjectDeliveryBox::Initialize(UClass* TargetClass)
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

void UCNObjectDeliveryBox::Send_Implementation(const UObject* message)
{
	
}

void UCNObjectDeliveryBox::NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer)
{
	
}

//uint8* CurrentPropAddr = It->ContainerPtrToValuePtr<uint8>(Buffer);
// ((UBoolProperty*)*It)->SetPropertyValue( CurrentPropAddr, true );
