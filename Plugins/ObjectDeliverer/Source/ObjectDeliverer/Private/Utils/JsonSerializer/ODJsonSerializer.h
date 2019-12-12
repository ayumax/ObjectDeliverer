// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

class ODJsonSerializer
{
public:

	TSharedPtr<FJsonObject> CreateJsonObject(const UObject* Obj);
	bool JsonObjectToUObject(const TSharedPtr<FJsonObject>& JsonObject, UObject* OutObject);

private:
	TSharedPtr<FJsonValue> ObjectJsonCallback(UProperty* Property, const void* Value);
	
};
