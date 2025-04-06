// Copyright 2019 ayumax. All Rights Reserved.
#include "ODJsonSerializer.h"
#include "Utils/ODObjectUtil.h"
#include "JsonObjectConverter.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "UObject/EnumProperty.h"
#include "UObject/TextProperty.h"
#include "UObject/PropertyPortFlags.h"
#include "DeliveryBox/IODConvertPropertyName.h"

UODJsonSerializer::UODJsonSerializer()
{
}

TSharedPtr<FJsonObject> UODJsonSerializer::CreateJsonObject(const UObject* Obj, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/)
{
    // Initialize the object tracking array for circular reference detection
    VisitedObjects.Empty();
    return UObjectToJsonObject(Obj->GetClass(), Obj, CheckFlags, SkipFlags);
}

TSharedPtr<FJsonValue> UODJsonSerializer::ObjectJsonCallback(FProperty* Property, const void* Value)
{
    if (const FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
    {
        if (UObject* ObjectValue = ObjectProperty->GetObjectPropertyValue(Value))
        {                        
            // Generate JSON object
            const TSharedPtr<FJsonObject> JsonObj = UObjectToJsonObject(ObjectProperty->PropertyClass, ObjectValue);
            
            return MakeShareable(new FJsonValueObject(JsonObj));
        }
        else
        {
            return MakeShareable(new FJsonValueNull());
        }
    }
    else if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
    {
        if (StructProperty->Struct == TBaseStructure<FDateTime>::Get())
        {
            const FDateTime* DateTimeValue = static_cast<const FDateTime*>(Value);
                    
            FString DateTimeStr = DateTimeValue->ToIso8601();
            
            return MakeShareable(new FJsonValueString(DateTimeStr));
        }
                
        TSharedPtr<FJsonValue> JsonObject = FJsonObjectConverter::UPropertyToJsonValue(StructProperty, Value);
        if (JsonObject.IsValid())
        {
            return JsonObject;
        }
        else
        {
            return MakeShareable(new FJsonValueNull());
        }
    }
    else if (const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
    {
        FScriptArrayHelper ArrayHelper(ArrayProperty, Value);
        TArray<TSharedPtr<FJsonValue>> JsonArray;
        
        for (int32 i = 0; i < ArrayHelper.Num(); i++)
        {
            const void* ElementPtr = ArrayHelper.GetRawPtr(i);
            FProperty* ElementProperty = ArrayProperty->Inner;
            
            if (const FObjectProperty* ElementObjectProperty = CastField<FObjectProperty>(ElementProperty))
            {
                if (const UObject* ElementObject = ElementObjectProperty->GetObjectPropertyValue(ElementPtr))
                {                    
                    // Generate JSON object
                    const TSharedPtr<FJsonObject> JsonObj = UObjectToJsonObject(ElementObjectProperty->PropertyClass, ElementObject);
                    
                    JsonArray.Add(MakeShareable(new FJsonValueObject(JsonObj)));
                }
                else
                {
                    JsonArray.Add(MakeShareable(new FJsonValueNull()));
                }
            }
            else if (const FStructProperty* ElementStructProperty = CastField<FStructProperty>(ElementProperty))
            {
                if (ElementStructProperty->Struct == TBaseStructure<FDateTime>::Get())
                {
                    const FDateTime* DateTimeValue = static_cast<const FDateTime*>(Value);

                    const FString DateTimeStr = DateTimeValue->ToIso8601();
            
                    return MakeShareable(new FJsonValueString(DateTimeStr));
                }

                if (TSharedPtr<FJsonValue> JsonObject = FJsonObjectConverter::UPropertyToJsonValue(ElementProperty, ElementPtr); JsonObject.IsValid())
                {
                    JsonArray.Add(JsonObject);
                }
                else
                {
                    JsonArray.Add(MakeShareable(new FJsonValueNull()));
                }
            }
            else if (const FNumericProperty* ElementNumericProperty = CastField<FNumericProperty>(ElementProperty))
            {
                if (ElementNumericProperty->IsFloatingPoint())
                {
                    JsonArray.Add(MakeShareable(new FJsonValueNumber(ElementNumericProperty->GetFloatingPointPropertyValue(ElementPtr))));
                }
                else if (ElementNumericProperty->IsInteger())
                {
                    JsonArray.Add(MakeShareable(new FJsonValueNumber(ElementNumericProperty->GetSignedIntPropertyValue(ElementPtr))));
                }
            }
            else if (const FStrProperty* ElementStrProperty = CastField<FStrProperty>(ElementProperty))
            {
                JsonArray.Add(MakeShareable(new FJsonValueString(ElementStrProperty->GetPropertyValue(ElementPtr))));
            }
            else if (const FTextProperty* ElementTextProperty = CastField<FTextProperty>(ElementProperty))
            {
                JsonArray.Add(MakeShareable(new FJsonValueString(ElementTextProperty->GetPropertyValue(ElementPtr).ToString())));
            }
            else if (const FNameProperty* ElementNameProperty = CastField<FNameProperty>(ElementProperty))
            {
                JsonArray.Add(MakeShareable(new FJsonValueString(ElementNameProperty->GetPropertyValue(ElementPtr).ToString())));
            }
            else if (const FBoolProperty* ElementBoolProperty = CastField<FBoolProperty>(ElementProperty))
            {
                JsonArray.Add(MakeShareable(new FJsonValueBoolean(ElementBoolProperty->GetPropertyValue(ElementPtr))));
            }
            else if (const FEnumProperty* ElementEnumProperty = CastField<FEnumProperty>(ElementProperty))
            {
                const UEnum* Enum = ElementEnumProperty->GetEnum();
                const FNumericProperty* UnderlyingProperty = ElementEnumProperty->GetUnderlyingProperty();
                const int64 InValue = UnderlyingProperty->GetSignedIntPropertyValue(ElementPtr);
                
                FString EnumName = Enum->GetNameStringByValue(InValue);
                JsonArray.Add(MakeShareable(new FJsonValueString(EnumName)));
            }
            else
            {
                JsonArray.Add(MakeShareable(new FJsonValueNull()));
            }
        }
        
        return MakeShareable(new FJsonValueArray(JsonArray));
    }

    // invalid
    return TSharedPtr<FJsonValue>();
}

TSharedPtr<FJsonObject> UODJsonSerializer::UObjectToJsonObject(UClass* ObjectClass, const UObject* Obj, int64 CheckFlags, int64 SkipFlags)
{
    if (!Obj) return MakeShareable(new FJsonObject());

    // Check for circular references
    if (VisitedObjects.Contains(const_cast<UObject*>(Obj)))
    {
        UE_LOG(LogTemp, Warning, TEXT("Circular reference detected in object %s"), *Obj->GetName());
        return MakeShareable(new FJsonObject());
    }

    // Add the object to the visited list
    VisitedObjects.Add(const_cast<UObject*>(Obj));

    auto objectSerializer = DefaultSerializer;

    if (ObjectSerializeres.Contains(ObjectClass))
    {
        objectSerializer = ObjectSerializeres[ObjectClass];
    }

    TSharedPtr<FJsonObject> Result = objectSerializer->UObjectToJsonObject(this, Obj, CheckFlags, SkipFlags);

    // Remove from the list when processing is complete
    VisitedObjects.Remove(const_cast<UObject*>(Obj));

    return Result;
}

void UODJsonSerializer::AddJsonValue(TSharedPtr<FJsonObject> JsonObject, const UObject* Obj, FProperty* Property, int64 CheckFlags, int64 SkipFlags)
{
    FString PropertyName = Property->GetName();
    if (Obj->GetClass()->ImplementsInterface(UODConvertPropertyName::StaticClass()))
    {
        if (FString ConvertedPropertyName = IODConvertPropertyName::Execute_ConvertFPropertyName(Obj, Property->GetFName()); !ConvertedPropertyName.IsEmpty())
        {
            PropertyName = ConvertedPropertyName;
        }
    }

    const uint8* CurrentPropAddr = Property->ContainerPtrToValuePtr<uint8>(const_cast<UObject*>(Obj));

    FJsonObjectConverter::CustomExportCallback CustomCB;
    CustomCB.BindLambda([this](FProperty* Property, const void* Value)
        {
            return ObjectJsonCallback(Property, Value);
        });

    JsonObject->SetField(PropertyName, FJsonObjectConverter::UPropertyToJsonValue(Property, CurrentPropAddr, CheckFlags, SkipFlags, &CustomCB));
}
