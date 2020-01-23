// Copyright 2019 ayumax. All Rights Reserved.
#include "DeliveryBox/ObjectDeliveryBoxUsingJson.h"
#include "Utils/ODStringUtil.h"
#include "../Utils/JsonSerializer/ODJsonDeserializer.h"
#include "../Utils/JsonSerializer/ODJsonSerializer.h"

UObjectDeliveryBoxUsingJson::UObjectDeliveryBoxUsingJson()
{
	Serializer = CreateDefaultSubobject<UODJsonSerializer>(TEXT("UODJsonSerializer"));
	Deserializer = CreateDefaultSubobject<UODJsonDeserializer>(TEXT("UODJsonDeserializer"));
}

UObjectDeliveryBoxUsingJson::~UObjectDeliveryBoxUsingJson()
{
}

void UObjectDeliveryBoxUsingJson::Initialize(UClass* _TargetClass)
{
	TargetClass = _TargetClass;
}

void UObjectDeliveryBoxUsingJson::InitializeCustom(EODJsonSerializeType DefaultSerializerType, const TMap<UClass*, EODJsonSerializeType>& ObjectSerializerTypes, UClass* _TargetClass)
{
	TargetClass = _TargetClass;
	Serializer->AddOverrideJsonSerializers(DefaultSerializerType, ObjectSerializerTypes);
	Deserializer->AddOverrideJsonSerializers(DefaultSerializerType, ObjectSerializerTypes);
}

void UObjectDeliveryBoxUsingJson::Send(const UObject* message, FString& makedJson)
{
	SendTo(message, nullptr, makedJson);
}

void UObjectDeliveryBoxUsingJson::SendTo(const UObject* message, const UObjectDelivererProtocol* Destination, FString& makedJson)
{
	auto jsonObject = Serializer->CreateJsonObject(message);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), Writer);

	makedJson = OutputString;

	TArray<uint8> buffer;
	UODStringUtil::StringToBuffer(OutputString, buffer);

	RequestSend.ExecuteIfBound(Destination, buffer);
}

void UObjectDeliveryBoxUsingJson::NotifyReceiveBuffer(const UObjectDelivererProtocol* FromObject, const TArray<uint8>& buffer)
{
	auto jsonString = UODStringUtil::BufferToString(buffer);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) return;

	UObject* createdObj = Deserializer->JsonObjectToUObject(JsonObject, TargetClass);

	Received.Broadcast(createdObj, jsonString, FromObject);
}

