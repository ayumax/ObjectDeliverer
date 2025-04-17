// Copyright 2019 ayumax. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "ObjectDelivererManagerTestHelper.h"
#include "Utils/JsonSerializer/ODJsonSerializer.h"
#include "Utils/JsonSerializer/ODJsonDeserializer.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonSerializerBasicTest, "ObjectDeliverer.JsonSerializer.BasicTest", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

// Helper method to serialize and deserialize an object
UObject* SerializeAndDeserializeObject(const UObject* SourceObject, const EODJsonSerializeType SerializeType)
{
    // Serialize
    UODJsonSerializer* Serializer = NewObject<UODJsonSerializer>();
    Serializer->AddOverrideJsonSerializers(SerializeType, TMap<UClass*, EODJsonSerializeType>());
    const auto JSONObj = Serializer->CreateJsonObject(SourceObject);
        
    // Deserialize
    UODJsonDeserializer* Deserializer = NewObject<UODJsonDeserializer>();
    Deserializer->AddOverrideJsonSerializers(SerializeType, TMap<UClass*, EODJsonSerializeType>());
    return Deserializer->JsonObjectToUObject(JSONObj, SourceObject->GetClass());
}

bool FJsonSerializerBasicTest::RunTest(const FString& Parameters)
{
	for (const EODJsonSerializeType SerializeType : {EODJsonSerializeType::NoWriteType, EODJsonSerializeType::WriteType})
	{
		UJsonSerializerTestObject* TestObj = NewObject<UJsonSerializerTestObject>();
		
		TestObj->IntProperty = 1;
		TestObj->FloatProperty = 3.14f;
		TestObj->Uint8Property = 128;
		TestObj->BoolProperty = true;
		TestObj->StringProperty = TEXT("string");
		TestObj->VectorProperty = FVector(1.1f, 2.99f, 0.123f);
		TestObj->RotateProperty = FRotator(1.22f, 2.33f, 3.44f);
		TestObj->ArrayProperty.SetNum(3);
		
		for (int i = 0; i < 3; ++i)
		{
			TestObj->ArrayProperty[i] = NewObject<UJsonSerializerTestArrayElementObject1>();
			TestObj->ArrayProperty[i]->IntProperty = i;
		}
			
		// Use the helper method for serialization and deserialization
		UJsonSerializerTestObject* TestObj2 = Cast<UJsonSerializerTestObject>(SerializeAndDeserializeObject(TestObj, SerializeType));

		TestNotNull("Deserialized object should not be null", TestObj2);
		
		// Verification
		TestEqual("IntProperty", TestObj2->IntProperty, TestObj->IntProperty);
		TestEqual("FloatProperty", TestObj2->FloatProperty, TestObj->FloatProperty);
		TestEqual("Uint8Property", TestObj2->Uint8Property, TestObj->Uint8Property);
		TestEqual("BoolProperty", TestObj2->BoolProperty, TestObj->BoolProperty);
		TestEqual("StringProperty", TestObj2->StringProperty, TestObj->StringProperty);
		TestEqual("VectorProperty", TestObj2->VectorProperty, TestObj->VectorProperty);
		TestEqual("RotateProperty", TestObj2->RotateProperty, TestObj->RotateProperty);
		TestEqual("ArrayProperty.Num", TestObj2->ArrayProperty.Num(), TestObj->ArrayProperty.Num());
			
		for (int i = 0; i < TestObj2->ArrayProperty.Num(); ++i)
		{
			TestEqual(FString::Printf(TEXT("ArrayProperty[%d].IntProperty"), i), TestObj2->ArrayProperty[i]->IntProperty, TestObj->ArrayProperty[i]->IntProperty);
		}
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonSerializerPropertyNameTest, "ObjectDeliverer.JsonSerializer.PropertyNameTest", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FJsonSerializerPropertyNameTest::RunTest(const FString& Parameters)
{
	for (const EODJsonSerializeType SerializeType : {EODJsonSerializeType::NoWriteType, EODJsonSerializeType::WriteType})
	{
		UJsonSerializeConvertNameTestObject* TestObj = NewObject<UJsonSerializeConvertNameTestObject>();
		TestObj->IntProperty = 1;
		TestObj->FloatProperty = 3.14f;
		TestObj->Uint8Property = 128;

		const UJsonSerializeConvertNameTestObject* TestObj2 = Cast<UJsonSerializeConvertNameTestObject>(SerializeAndDeserializeObject(TestObj, SerializeType));
		
		TestNotNull("Deserialized object should not be null", TestObj2);
		TestEqual("IntProperty", TestObj2->IntProperty, TestObj->IntProperty);
		TestEqual("FloatProperty", TestObj2->FloatProperty, TestObj->FloatProperty);
		TestEqual("Uint8Property", TestObj2->Uint8Property, TestObj->Uint8Property);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonSerializerComplexTest, "ObjectDeliverer.JsonSerializer.ComplexTest", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FJsonSerializerComplexTest::RunTest(const FString& Parameters)
{
	for (const EODJsonSerializeType SerializeType : {EODJsonSerializeType::NoWriteType, EODJsonSerializeType::WriteType})
	{
		UJsonSerializerComplexObject* TestObj = NewObject<UJsonSerializerComplexObject>();
		TestObj->MapProperty.Add(TEXT("Key1"), 100);
		TestObj->MapProperty.Add(TEXT("Key2"), 200);
		TestObj->MapProperty.Add(TEXT("Key3"), 300);
		
		TestObj->SetProperty.Add(TEXT("Value1"));
		TestObj->SetProperty.Add(TEXT("Value2"));
		TestObj->SetProperty.Add(TEXT("Value3"));
		
		TestObj->NestedObject = NewObject<UJsonSerializerNestedObject>();
		TestObj->NestedObject->Name = TEXT("NestedObject");
		TestObj->NestedObject->Value = 999;
		
		TestObj->NestedObjectArray.SetNum(3);
		for (int i = 0; i < 3; ++i)
		{
			TestObj->NestedObjectArray[i] = NewObject<UJsonSerializerNestedObject>();
			TestObj->NestedObjectArray[i]->Name = FString::Printf(TEXT("ArrayObject%d"), i);
			TestObj->NestedObjectArray[i]->Value = i * 10;
		}
		
		// Use the helper method for serialization and deserialization
		UJsonSerializerComplexObject* TestObj2 = Cast<UJsonSerializerComplexObject>(
			SerializeAndDeserializeObject(TestObj, SerializeType));
		
		TestNotNull("Deserialized object should not be null", TestObj2);
		
		TestEqual("MapProperty.Num", TestObj2->MapProperty.Num(), TestObj->MapProperty.Num());
		TestEqual("MapProperty[Key1]", TestObj2->MapProperty[TEXT("Key1")], TestObj->MapProperty[TEXT("Key1")]);
		TestEqual("MapProperty[Key2]", TestObj2->MapProperty[TEXT("Key2")], TestObj->MapProperty[TEXT("Key2")]);
		TestEqual("MapProperty[Key3]", TestObj2->MapProperty[TEXT("Key3")], TestObj->MapProperty[TEXT("Key3")]);
		
		TestEqual("SetProperty.Num", TestObj2->SetProperty.Num(), TestObj->SetProperty.Num());
		TestTrue("SetProperty contains Value1", TestObj2->SetProperty.Contains(TEXT("Value1")));
		TestTrue("SetProperty contains Value2", TestObj2->SetProperty.Contains(TEXT("Value2")));
		TestTrue("SetProperty contains Value3", TestObj2->SetProperty.Contains(TEXT("Value3")));
		
		TestNotNull("NestedObject exists", TestObj2->NestedObject);
		if (TestObj2->NestedObject)
		{
			TestEqual("NestedObject.Name", TestObj2->NestedObject->Name, TestObj->NestedObject->Name);
			TestEqual("NestedObject.Value", TestObj2->NestedObject->Value, TestObj->NestedObject->Value);
		}
		
		TestEqual("NestedObjectArray.Num", TestObj2->NestedObjectArray.Num(), TestObj->NestedObjectArray.Num());
		
		for (int i = 0; i < FMath::Min(TestObj2->NestedObjectArray.Num(), TestObj->NestedObjectArray.Num()); i++)
		{
			TestNotNull(FString::Printf(TEXT("NestedObjectArray[%d] exists"), i), TestObj2->NestedObjectArray[i]);
			if (TestObj2->NestedObjectArray[i])
			{
				TestEqual(FString::Printf(TEXT("NestedObjectArray[%d].Name"), i), 
					TestObj2->NestedObjectArray[i]->Name, TestObj->NestedObjectArray[i]->Name);
				TestEqual(FString::Printf(TEXT("NestedObjectArray[%d].Value"), i), 
					TestObj2->NestedObjectArray[i]->Value, TestObj->NestedObjectArray[i]->Value);
			}
		}
	}
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonSerializerInheritanceTest, "ObjectDeliverer.JsonSerializer.InheritanceTest", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FJsonSerializerInheritanceTest::RunTest(const FString& Parameters)
{
	for (const EODJsonSerializeType SerializeType : {EODJsonSerializeType::NoWriteType, EODJsonSerializeType::WriteType})
	{
		UJsonSerializerDerivedClass* TestObj = NewObject<UJsonSerializerDerivedClass>();
		TestObj->BaseValue = 100;
		TestObj->BaseName = TEXT("BaseClassName");
		TestObj->DerivedValue = 3.14159f;
		TestObj->DerivedFlag = true;

		// Use the helper method for serialization and deserialization
		UJsonSerializerDerivedClass* TestObj2 = Cast<UJsonSerializerDerivedClass>(
			SerializeAndDeserializeObject(TestObj, SerializeType));
		
		TestNotNull("Deserialized object should not be null", TestObj2);
		
		TestEqual("BaseValue", TestObj2->BaseValue, TestObj->BaseValue);
		TestEqual("BaseName", TestObj2->BaseName, TestObj->BaseName);
		TestEqual("DerivedValue", TestObj2->DerivedValue, TestObj->DerivedValue);
		TestEqual("DerivedFlag", TestObj2->DerivedFlag, TestObj->DerivedFlag);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonSerializerUE4TypesTest, "ObjectDeliverer.JsonSerializer.UE4TypesTest", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FJsonSerializerUE4TypesTest::RunTest(const FString& Parameters)
{
    for (const EODJsonSerializeType SerializeType : {EODJsonSerializeType::NoWriteType, EODJsonSerializeType::WriteType})
    {
        UJsonSerializerUE4TypesObject* TestObj = NewObject<UJsonSerializerUE4TypesObject>();
        
        // Transform
        FTransform OriginalTransform = FTransform();
        OriginalTransform.SetLocation(FVector(1.0f, 2.0f, 3.0f));
        OriginalTransform.SetRotation(FQuat(0.1f, 0.2f, 0.3f, 0.4f).GetNormalized());
        OriginalTransform.SetScale3D(FVector(2.0f, 2.0f, 2.0f));
        TestObj->TransformProperty = OriginalTransform;
        
        // Color
        FColor OriginalColor = FColor(255, 128, 64, 32);
        TestObj->ColorProperty = OriginalColor;
        
        // DateTime
        FDateTime OriginalDateTime = FDateTime::Now();
        TestObj->DateTimeProperty = OriginalDateTime;
        
        // GUID
        FGuid OriginalGuid = FGuid::NewGuid();
        TestObj->GuidProperty = OriginalGuid;

        // Use the helper method for serialization and deserialization
        UJsonSerializerUE4TypesObject* TestObj2 = Cast<UJsonSerializerUE4TypesObject>(
            SerializeAndDeserializeObject(TestObj, SerializeType));

        // 検証
        TestNotNull("Deserialized object should not be null", TestObj2);
        
        TestEqual("TransformProperty.Location", TestObj2->TransformProperty.GetLocation(), TestObj->TransformProperty.GetLocation());
        TestEqual("TransformProperty.Rotation", TestObj2->TransformProperty.GetRotation(), TestObj->TransformProperty.GetRotation());
        TestEqual("TransformProperty.Scale3D", TestObj2->TransformProperty.GetScale3D(), TestObj->TransformProperty.GetScale3D());
        
        TestEqual("ColorProperty.R", TestObj2->ColorProperty.R, TestObj->ColorProperty.R);
        TestEqual("ColorProperty.G", TestObj2->ColorProperty.G, TestObj->ColorProperty.G);
        TestEqual("ColorProperty.B", TestObj2->ColorProperty.B, TestObj->ColorProperty.B);
        TestEqual("ColorProperty.A", TestObj2->ColorProperty.A, TestObj->ColorProperty.A);
        
        TestEqual("DateTimeProperty", TestObj2->DateTimeProperty, TestObj->DateTimeProperty);
        TestEqual("GuidProperty", TestObj2->GuidProperty, TestObj->GuidProperty);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonSerializerCircularReferenceTest, "ObjectDeliverer.JsonSerializer.CircularReferenceTest", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FJsonSerializerCircularReferenceTest::RunTest(const FString& Parameters)
{
    for (EODJsonSerializeType SerializeType : {EODJsonSerializeType::NoWriteType, EODJsonSerializeType::WriteType})
    {
        UJsonSerializerCircularObject* TestObj = NewObject<UJsonSerializerCircularObject>();
        TestObj->Name = TEXT("Parent");
    
        // Create child object and set reference to parent
        UJsonSerializerCircularObject* ChildObj = NewObject<UJsonSerializerCircularObject>();
        ChildObj->Name = TEXT("Child");
        ChildObj->Reference = TestObj;  // Reference to parent
    
        // Set reference from parent to child
        TestObj->Reference = ChildObj;  // Reference to child - creates circular reference
        
        // Use the helper method for serialization and deserialization
        UJsonSerializerCircularObject* TestObj2 = Cast<UJsonSerializerCircularObject>(
            SerializeAndDeserializeObject(TestObj, SerializeType));

        // Verification
        TestNotNull("Deserialized object should not be null", TestObj2);
        TestEqual("Name", TestObj2->Name, TestObj->Name);
        
        // The child object should exist
        TestNotNull("Reference exists", TestObj2->Reference);
        
        if (TestObj2->Reference)
        {
            TestEqual("Reference.Name", TestObj2->Reference->Name, TEXT("Child"));

        	TestTrue("Reference.Reference.Name",
            	TestObj2->Reference->Reference == nullptr || TestObj2->Reference->Reference->Name.IsEmpty());
        }
    }

    return true;
}
