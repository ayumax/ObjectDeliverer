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

	for (TFieldIterator<UProperty> PropIt(Obj->GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		UProperty* Property = *PropIt;
		FString PropertyName = Property->GetName();
		uint8* CurrentPropAddr = PropIt->ContainerPtrToValuePtr<uint8>((UObject*)Obj);

		switch (UObjectUtil::GetUPropertyType(Property))
		{
		case EUPropertyType::Bool:
			JsonObject->SetBoolField(PropertyName, ((UBoolProperty*)Property)->GetPropertyValue(CurrentPropAddr));
			break;
		case EUPropertyType::Int32:
			JsonObject->SetNumberField(PropertyName, (int32)((UUInt32Property*)Property)->GetSignedIntPropertyValue(CurrentPropAddr));
			break;
		case EUPropertyType::Float:
			JsonObject->SetNumberField(PropertyName, (float)((UFloatProperty*)Property)->GetFloatingPointPropertyValue(CurrentPropAddr));
			break;
		case EUPropertyType::UInt8:
			JsonObject->SetNumberField(PropertyName, (uint8)((UByteProperty*)Property)->GetUnsignedIntPropertyValue(CurrentPropAddr));
			break;
		case EUPropertyType::Enum:
			JsonObject->SetNumberField(PropertyName, (int32)((UEnumProperty*)Property)->GetUnderlyingProperty()->GetUnsignedIntPropertyValue(CurrentPropAddr));
			break;
		case EUPropertyType::Array:
		{
			TArray<TSharedPtr<FJsonValue>> arrayValue;
			UArrayProperty* ArrayProperty = (UArrayProperty*)Property;
			FScriptArrayHelper ArrayHelper(ArrayProperty, CurrentPropAddr);
			for (int32 ArrayIndex = 0; ArrayIndex < ArrayHelper.Num(); ++ArrayIndex)
			{
				void* ArrayData = ArrayHelper.GetRawPtr(ArrayIndex);
				TSharedPtr<FJsonValue> arrayItem = FJsonObjectConverter::UPropertyToJsonValue(ArrayProperty->Inner, ArrayData, 0, 0);
				arrayValue.Add(arrayItem);
			}
			JsonObject->SetArrayField(PropertyName, arrayValue);
		}
			break;
		case EUPropertyType::Struct:
		{
			TSharedRef<FJsonObject> OutJsonObject = MakeShareable(new FJsonObject());
			UStructProperty* StructProperty = (UStructProperty*)Property;
			FJsonObjectConverter::UStructToJsonObject(StructProperty->Struct, CurrentPropAddr, OutJsonObject, 0, 0);


			JsonObject->SetObjectField(PropertyName, OutJsonObject);
		}
			
			break;
		case EUPropertyType::Object:
			JsonObject->SetObjectField(PropertyName, CreateJsonObject(((UObjectProperty*)Property)->GetObjectPropertyValue(CurrentPropAddr)));
			break;
		}
	}

	return JsonObject;
}