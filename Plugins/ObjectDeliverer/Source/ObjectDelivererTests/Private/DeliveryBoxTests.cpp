// Copyright 2019 ayumax. All Rights Reserved.
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "DeliveryBox/Utf8StringDeliveryBox.h"
#include "Protocol/ProtocolTcpIpServer.h"
#include "ObjectDelivererManagerTestHelper.h"
#include "DeliveryBox/ObjectDeliveryBoxUsingJson.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"

#if WITH_DEV_AUTOMATION_TESTS

// Test for Utf8StringDeliveryBox
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUtf8StringDeliveryBoxTest, "ObjectDeliverer.DeliveryBox.Utf8StringDeliveryBoxTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUtf8StringDeliveryBoxTest::RunTest(const FString &Parameters)
{
    TArray<const TArray<uint8>> stringBuffer;

    UUtf8StringDeliveryBox *sut = NewObject<UUtf8StringDeliveryBox>();

    sut->RequestSend.BindLambda([&stringBuffer](const UObjectDelivererProtocol *protocol, const TArray<uint8> &Buffer)
                                { stringBuffer.Add(Buffer); });

    auto TestHelper = NewObject<UUtf8StringDeliveryBoxTestHelper>();

    sut->Received.AddDynamic(TestHelper, &UUtf8StringDeliveryBoxTestHelper::OnReceiveString);

    // Send multiple string data
    const TArray<FString>
        TestStrings = {
            TEXT("Hello World"),
            TEXT("こんにちは世界"),
            TEXT("你好世界"),
            TEXT("¡Hola Mundo!"),
            TEXT("1234567890!@#$%^&*()_+")};

    for (const FString &TestString : TestStrings)
    {
        sut->Send(TestString);
    }

    for (const TArray<uint8> &Buffer : stringBuffer)
    {
        sut->NotifyReceiveBuffer(nullptr, Buffer);
    }

    TestEqual("Received buffer count should match sent strings count", TestHelper->ReceivedStrings.Num(),
              TestStrings.Num());

    for (int32 i = 0; i < TestHelper->ReceivedStrings.Num(); i++)
    {
        TestEqual("Received string should match sent string", TestHelper->ReceivedStrings[i], TestStrings[i]);
    }

    return true;
}

// Test for object serialization using ObjectDeliveryBox
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FObjectDeliveryBoxTest, "ObjectDeliverer.DeliveryBox.ObjectDeliveryBoxTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FObjectDeliveryBoxTest::RunTest(const FString &Parameters)
{
    TArray<UObjectDeliveryBoxUsingJson *> deliveryBoxes;

    auto sut = NewObject<UObjectDeliveryBoxUsingJson>();
    sut->Initialize(UJsonSerializerTestObject::StaticClass());
    deliveryBoxes.Add(sut);

    sut = NewObject<UObjectDeliveryBoxUsingJson>();
    sut->InitializeCustom(EODJsonSerializeType::WriteType, TMap<UClass *, EODJsonSerializeType>(),
                          UJsonSerializerTestObject::StaticClass());
    deliveryBoxes.Add(sut);

    for (UObjectDeliveryBoxUsingJson *deliveryBoxe : deliveryBoxes)
    {
        // Buffer for sending and objects for receiving
        TArray<TArray<uint8>> sendBuffers;

        // Set up the event that will be executed when sending
        deliveryBoxe->RequestSend.BindLambda(
            [&sendBuffers](const UObjectDelivererProtocol *protocol, const TArray<uint8> &Buffer)
            {
                sendBuffers.Add(Buffer);
            });

        auto TestHelper = NewObject<UObjectDeliveryBoxUsingJsonTestHelper>();

        deliveryBoxe->Received.AddDynamic(TestHelper, &UObjectDeliveryBoxUsingJsonTestHelper::OnReceiveObject);

        // Create and send objects
        TArray<UJsonSerializerTestObject *> TestObjects;
        for (int32 i = 0; i < 10; i++)
        {
            auto testObject = NewObject<UJsonSerializerTestObject>();
            testObject->IntProperty = 42 + i;
            testObject->FloatProperty = 3.14f * i;
            testObject->Uint8Property = 255 - i;
            testObject->BoolProperty = i % 2 ? true : false;
            testObject->StringProperty = FString::Printf(TEXT("テスト文字列%d"), i);
            testObject->VectorProperty = FVector(i, 2, 3);
            testObject->RotateProperty = FRotator(10 + i, 20, 30);

            FString jsonString;
            deliveryBoxe->Send(testObject, jsonString);

            TestObjects.Add(testObject);
        }

        for (const TArray<uint8> &Buffer : sendBuffers)
        {
            deliveryBoxe->NotifyReceiveBuffer(nullptr, Buffer);
        }

        TestEqual("Should have received objects", TestHelper->ReceivedObjects.Num(), TestObjects.Num());

        for (int32 i = 0; i < 10; i++)
        {
            auto ReceivedObject = Cast<UJsonSerializerTestObject>(TestHelper->ReceivedObjects[i]);
            TestNotNull("Received object should be valid", ReceivedObject);

            if (ReceivedObject)
            {
                TestEqual("IntProperty should match", ReceivedObject->IntProperty, TestObjects[i]->IntProperty);
                TestEqual("FloatProperty should match", ReceivedObject->FloatProperty, TestObjects[i]->FloatProperty);
                TestEqual("Uint8Property should match", ReceivedObject->Uint8Property, TestObjects[i]->Uint8Property);
                TestEqual("BoolProperty should match", ReceivedObject->BoolProperty, TestObjects[i]->BoolProperty);
                TestEqual("StringProperty should match", ReceivedObject->StringProperty,
                          TestObjects[i]->StringProperty);
                TestEqual("VectorProperty should match", ReceivedObject->VectorProperty,
                          TestObjects[i]->VectorProperty);
                TestEqual("RotatorProperty should match", ReceivedObject->RotateProperty,
                          TestObjects[i]->RotateProperty);
            }
        }
    }

    return true;
}

// Test for property name conversion functionality
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPropertyNameConversionTest, "ObjectDeliverer.DeliveryBox.PropertyNameConversionTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FPropertyNameConversionTest::RunTest(const FString &Parameters)
{
    // Buffer for sending
    TArray<TArray<uint8>> sendBuffers;

    // Create a delivery box using JSON serializer
    auto sut = NewObject<UObjectDeliveryBoxUsingJson>();
    sut->Initialize(UJsonSerializeConvertNameTestObject::StaticClass());

    // Set up the event that will be executed when sending
    sut->RequestSend.BindLambda([&sendBuffers](const UObjectDelivererProtocol *protocol, const TArray<uint8> &Buffer)
                                { sendBuffers.Add(Buffer); });

    // Set up the event that will be executed when receiving
    auto TestHelper = NewObject<UObjectDeliveryBoxUsingJsonTestHelper>();
    sut->Received.AddDynamic(TestHelper, &UObjectDeliveryBoxUsingJsonTestHelper::OnReceiveObject);

    // Test property name conversion
    auto TestObject = NewObject<UJsonSerializeConvertNameTestObject>();
    TestObject->IntProperty = 123;
    TestObject->FloatProperty = 45.67f;
    TestObject->Uint8Property = 89;

    // Serialize the object
    FString jsonString;
    sut->Send(TestObject, jsonString);

    // Create FJsonObject from JSON string
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(jsonString);
    bool bSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);
    TestTrue("JSON deserialization should succeed", bSuccess);
    TestTrue("JSON object should be valid", JsonObject.IsValid());

    if (JsonObject.IsValid())
    {
        // Verify property values
        int32 IntValue = 0;
        TestTrue("Should get IntProperty value", JsonObject->TryGetNumberField(TEXT("IntProperty_Converted"), IntValue));
        TestEqual("IntProperty value should match", IntValue, 123);

        double FloatValue = 0.0;
        TestTrue("Should get FloatProperty value", JsonObject->TryGetNumberField(TEXT("FloatProperty_Converted"), FloatValue));
        TestEqual("FloatProperty value should match", static_cast<float>(FloatValue), 45.67f);

        int32 Uint8Value = 0;
        TestTrue("Should get Uint8Property value", JsonObject->TryGetNumberField(TEXT("Uint8Property_Converted"), Uint8Value));
        TestEqual("Uint8Property value should match", static_cast<uint8>(Uint8Value), 89);
    }

    for (const TArray<uint8> &Buffer : sendBuffers)
    {
        // Simulate receiving the sent data
        sut->NotifyReceiveBuffer(nullptr, Buffer);
    }

    // Verify the received data
    TestEqual("Should have received objects", TestHelper->ReceivedObjects.Num(), 1);

    // Verify properties of the received object
    if (TestHelper->ReceivedObjects.Num() > 0)
    {
        auto ReceivedObject = Cast<UJsonSerializeConvertNameTestObject>(TestHelper->ReceivedObjects[0]);
        TestNotNull("Received object should be valid", ReceivedObject);

        if (ReceivedObject)
        {
            TestEqual("IntProperty should match", ReceivedObject->IntProperty, 123);
            TestEqual("FloatProperty should match", ReceivedObject->FloatProperty, 45.67f);
            TestEqual("Uint8Property should match", ReceivedObject->Uint8Property, (uint8)89);
        }
    }

    return true;
}

#endif
