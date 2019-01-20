#pragma once

#include "CoreMinimal.h"
#include "ObjectDeliveryBox.h"
#include "JsonReader.h"
#include "JsonSerializer.h"
#include "ObjectDeliveryBoxUsingJson.generated.h"


UCLASS(BlueprintType)
class OBJECTDELIVERER_API UObjectDeliveryBoxUsingJson : public UObjectDeliveryBox
{
	GENERATED_BODY()

public:
	UObjectDeliveryBoxUsingJson();
	~UObjectDeliveryBoxUsingJson();

	virtual void Send_Implementation(const UObject* message);

	virtual void NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer) override;

	static TSharedPtr<FJsonValue> ObjectJsonCallback(UProperty* Property, const void* Value);

private:
	static TSharedPtr<FJsonObject> CreateJsonObject(const UObject* Obj);
};
