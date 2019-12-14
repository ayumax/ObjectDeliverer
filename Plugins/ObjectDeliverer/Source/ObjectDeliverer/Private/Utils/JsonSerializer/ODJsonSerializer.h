// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

class OBJECTDELIVERER_API ODJsonSerializer
{
public:

	TSharedPtr<FJsonObject> CreateJsonObject(const UObject* Obj, int64 CheckFlags = 0, int64 SkipFlags = 0);

	DECLARE_DELEGATE_RetVal_TwoParams(TSharedPtr<FJsonValue>, CustomExportCallback, UProperty* /* Property */, const void* /* Value */);

private:

	TSharedPtr<FJsonValue> ObjectJsonCallback(UProperty* Property, const void* Value);
	
	TSharedPtr<FJsonValue> UPropertyToJsonValue(UProperty* Property, const void* Value, int64 CheckFlags, int64 SkipFlags, const CustomExportCallback* ExportCb);
	TSharedPtr<FJsonValue> ConvertScalarUPropertyToJsonValue(UProperty* Property, const void* Value, int64 CheckFlags, int64 SkipFlags, const CustomExportCallback* ExportCb);

};
