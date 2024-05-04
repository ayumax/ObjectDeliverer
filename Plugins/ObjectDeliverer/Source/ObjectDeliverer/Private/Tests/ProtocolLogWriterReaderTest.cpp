// Copyright 2019 ayumax. All Rights Reserved.
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Protocol/ProtocolLogReader.h"
#include "Protocol/ProtocolLogWriter.h"
#include "PacketRule/PacketRuleSizeBody.h"
#include "PacketRule/PacketRuleFactory.h"
#include "Protocol/ProtocolFactory.h"
#include "ObjectDelivererManager.h"
#include "DeliveryBox/Utf8StringDeliveryBox.h"
#include "Tests/ObjectDelivererManagerTestHelper.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProtocolLogWrierReaderTest, "ObjectDeliverer.ProtocolTest.ProtocolLogWrierReaderTest1", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FProtocolLogWrierReaderTest::RunTest(const FString& Parameters)
{
	auto deliveryBox = NewObject<UUtf8StringDeliveryBox>();
	auto ObjectDelivererWriter = NewObject<UObjectDelivererManager>();
	ObjectDelivererWriter->Start(UProtocolFactory::CreateProtocolLogWriter("log.bin", false), UPacketRuleFactory::CreatePacketRuleSizeBody(), deliveryBox);


	auto deliveryBoxReceive = NewObject<UUtf8StringDeliveryBox>();
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererReader = NewObject<UObjectDelivererManager>();
	deliveryBoxReceive->Received.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceiveString);

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([=]()
	{
		deliveryBox->Send("AAA");
		return true;
	}));

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([=]()
	{
		deliveryBox->Send("BBB");
		return true;
	}));

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.7f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([=]()
	{
		deliveryBox->Send("CCC");
		ObjectDelivererWriter->Close();

		ObjectDelivererReader->Start(UProtocolFactory::CreateProtocolLogReader("log.bin", false, true), UPacketRuleFactory::CreatePacketRuleSizeBody(), deliveryBoxReceive);

		return true;
	}));

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.5f));
	
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([=, this]()
	{
		ObjectDelivererReader->Close();

		TestEqual("check received count", serverHelper->ReceiveStrings.Num(), 3);

		if (serverHelper->ReceiveStrings.Num() == 3)
		{
			TestEqual("check received data", serverHelper->ReceiveStrings[0], TEXT("AAA"));
			TestEqual("check received data", serverHelper->ReceiveStrings[1], TEXT("BBB"));
			TestEqual("check received data", serverHelper->ReceiveStrings[2], TEXT("CCC"));
		}
		

		return true;
	}));

	return true;
}
#endif