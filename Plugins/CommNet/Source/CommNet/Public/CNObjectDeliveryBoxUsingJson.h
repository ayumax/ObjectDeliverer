#pragma once

#include "CoreMinimal.h"
#include "CNObjectDeliveryBox.h"
#include "JsonReader.h"
#include "JsonSerializer.h"
#include "CNObjectDeliveryBoxUsingJson.generated.h"


UCLASS(BlueprintType)
class COMMNET_API UCNObjectDeliveryBoxUsingJson : public UCNObjectDeliveryBox
{
	GENERATED_BODY()

public:
	UCNObjectDeliveryBoxUsingJson();
	~UCNObjectDeliveryBoxUsingJson();

	virtual void Send_Implementation(const UObject* message);

	virtual void NotifyReceiveBuffer_Implementation(const TArray<uint8>& buffer) override;

private:
	TSharedPtr<FJsonObject> CreateJsonObject(const UObject* Obj);
};
