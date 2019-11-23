// Copyright 2019 ayumax. All Rights Reserved.
#include "DeliveryBox/ObjectDeliveryBoxUsingJson.h"
#include "Utils/StringUtil.h"
#include "../Utils/ObjectJsonSerializer.h"

UObjectDeliveryBoxUsingJson::UObjectDeliveryBoxUsingJson()
{

}

UObjectDeliveryBoxUsingJson::~UObjectDeliveryBoxUsingJson()
{
}

void UObjectDeliveryBoxUsingJson::Initialize(UClass* _TargetClass)
{
	TargetClass = _TargetClass;
}

void UObjectDeliveryBoxUsingJson::Send(const UObject* message)
{
	SendTo(message, nullptr);
}

void UObjectDeliveryBoxUsingJson::SendTo(const UObject* message, const UObjectDelivererProtocol* Destination)
{
	auto jsonObject = UObjectJsonSerializer::CreateJsonObject(message);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), Writer);

	TArray<uint8> buffer;
	UStringUtil::StringToBuffer(OutputString, buffer);

	RequestSend.ExecuteIfBound(Destination, buffer);
}

void UObjectDeliveryBoxUsingJson::NotifyReceiveBuffer(const UObjectDelivererProtocol* FromObject, const TArray<uint8>& buffer)
{
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(UStringUtil::BufferToString(buffer));
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) return;

	UObject* createdObj = NewObject<UObject>((UObject*)GetTransientPackage(), TargetClass);

	UObjectJsonSerializer::JsonObjectToUObject(JsonObject, createdObj);

  if (!createdObj)
    return;

  // With dynamic object class serialization it is not necessary to specify TargetClass */
  // TODO: think of throwing away TargetClass
  //       this will let user set more flexible object class checks
  if (!createdObj->GetClass()->IsChildOf(TargetClass))
    return

	Received.Broadcast(createdObj, FromObject);
}

