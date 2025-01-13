// Copyright 2019 ayumax. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "ObjectDelivererManagerTestHelper.h"
#include "../Utils/JsonSerializer/ODJsonSerializer.h"
#include "../Utils/JsonSerializer/ODJsonDeserializer.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ODJsonSerializer_Tests, "ObjectDeliverer.JsonSerializer.SerializationTest", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool ODJsonSerializer_Tests::RunTest(const FString& Parameters)
{
	UJsonSerializerTestObject* testObj = NewObject<UJsonSerializerTestObject>();
	testObj->IntProperty = 1;
	testObj->FloatProperty = 3.14f;
	testObj->Uint8Property = 128;
	testObj->BoolProperty = true;
	testObj->StringProperty = TEXT("ABCD");
	testObj->VectorProperty = FVector(1.1f, 2.99f, 0.123f);
	testObj->RotateProperty = FRotator(1.22f, 2.33f, 3.44f);
	testObj->ArrayProperty.SetNum(3);
	for (int i = 0; i < 3; ++i)
	{
		testObj->ArrayProperty[i] = NewObject<UJsonSerializerTestArrayElementObject1>();
		testObj->ArrayProperty[i]->IntProperty = i;
	}

	UODJsonSerializer* serializer = NewObject<UODJsonSerializer>();
	auto jsonObj = serializer->CreateJsonObject(testObj);

	UODJsonDeserializer* deserializr = NewObject<UODJsonDeserializer>();
	UJsonSerializerTestObject* testObj2 = Cast<UJsonSerializerTestObject>(deserializr->JsonObjectToUObject(jsonObj, UJsonSerializerTestObject::StaticClass()));

	TestEqual(TEXT("check int property"), testObj2->IntProperty, 1);
	TestEqual(TEXT("check float property"), testObj2->FloatProperty, 3.14f);
	TestEqual(TEXT("check uint8 property"), testObj2->Uint8Property, (uint8)128);
	TestEqual(TEXT("check bool property"), testObj2->BoolProperty, true);
	TestEqual(TEXT("check string property"), testObj2->StringProperty, FString(TEXT("ABCD")));
	TestEqual(TEXT("check vector property"), testObj2->VectorProperty, FVector(1.1f, 2.99f, 0.123f));
	TestEqual(TEXT("check rotator property"), testObj2->RotateProperty, FRotator(1.22f, 2.33f, 3.44f));
	TestEqual(TEXT("check array property num"), testObj2->ArrayProperty.Num(), 3);

	for (int i = 0; i < 3; ++i)
	{
		TestEqual(TEXT("check array property value"), testObj2->ArrayProperty[i]->IntProperty, i);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ODJsonSerializer_ConvertPropertyNameTest, "ObjectDeliverer.JsonSerializer.ConvertPropertyNameTest", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool ODJsonSerializer_ConvertPropertyNameTest::RunTest(const FString& Parameters)
{
	UJsonSerializeConvertNameTestObject* testObj = NewObject<UJsonSerializeConvertNameTestObject>();
	testObj->IntProperty = 1;
	testObj->FloatProperty = 3.14f;
	testObj->Uint8Property = 128;

	UODJsonSerializer* serializer = NewObject<UODJsonSerializer>();
	auto jsonObj = serializer->CreateJsonObject(testObj);

	UODJsonDeserializer* deserializr = NewObject<UODJsonDeserializer>();
	UJsonSerializeConvertNameTestObject* testObj2 = Cast<UJsonSerializeConvertNameTestObject>(deserializr->JsonObjectToUObject(jsonObj, UJsonSerializeConvertNameTestObject::StaticClass()));

	TestEqual(TEXT("check int property"), testObj2->IntProperty, 1);
	TestEqual(TEXT("check float property"), testObj2->FloatProperty, 3.14f);
	TestEqual(TEXT("check uint8 property"), testObj2->Uint8Property, (uint8)128);

	return true;
}

