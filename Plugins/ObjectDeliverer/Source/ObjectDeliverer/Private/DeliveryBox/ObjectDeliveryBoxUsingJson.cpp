#include "ObjectDeliveryBoxUsingJson.h"
#include "Utils/ObjectUtil.h"
#include "Utils/StringUtil.h"
#include "JsonObjectConverter.h"

UObjectDeliveryBoxUsingJson::UObjectDeliveryBoxUsingJson()
{

}

UObjectDeliveryBoxUsingJson::~UObjectDeliveryBoxUsingJson()
{
}

void UObjectDeliveryBoxUsingJson::Send_Implementation(const UObject* message)
{
	auto jsonObject = CreateJsonObject(message);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), Writer);

	TArray<uint8> buffer;
	UStringUtil::StringToBuffer(OutputString, buffer);

	RequestSend.ExecuteIfBound(buffer);
}

void UObjectDeliveryBoxUsingJson::NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer)
{
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(UStringUtil::BufferToString(buffer));
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) return;
	
	for (auto& jsonValue : JsonObject->Values)
	{
		//DeliveryObject->property
		//jsonValue.Key
	}
	
}


TSharedPtr<FJsonObject> UObjectDeliveryBoxUsingJson::CreateJsonObject(const UObject* Obj)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!Obj) return JsonObject;

	for (TFieldIterator<UProperty> PropIt(Obj->GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		UProperty* Property = *PropIt;
		FString PropertyName = Property->GetName();
		uint8* CurrentPropAddr = PropIt->ContainerPtrToValuePtr<uint8>((UObject*)Obj);

		FJsonObjectConverter::CustomExportCallback CustomCB;
		CustomCB.BindStatic(UObjectDeliveryBoxUsingJson::ObjectJsonCallback);
		JsonObject->SetField(PropertyName, FJsonObjectConverter::UPropertyToJsonValue(Property, CurrentPropAddr, 0, 0, &CustomCB));

	}

	return JsonObject;
}

TSharedPtr<FJsonValue> UObjectDeliveryBoxUsingJson::ObjectJsonCallback(UProperty* Property, const void* Value)
{
	if (UObjectProperty* ObjectProperty = Cast<UObjectProperty>(Property))
	{
		if (!ObjectProperty->HasAnyFlags(RF_Transient)) // We are taking Transient to mean we don't want to serialize to Json either (could make a new flag if nessasary)
		{
			return MakeShareable(new FJsonValueObject(CreateJsonObject(ObjectProperty->GetObjectPropertyValue(Value))));
		}
	}

	// invalid
	return TSharedPtr<FJsonValue>();
}