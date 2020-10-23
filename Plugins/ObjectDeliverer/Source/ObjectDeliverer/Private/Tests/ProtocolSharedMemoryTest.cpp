// Copyright 2019 ayumax. All Rights Reserved.
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Protocol/ProtocolSharedMemory.h"
#include "PacketRule/PacketRuleSizeBody.h"
#include "PacketRule/PacketRuleFactory.h"
#include "Protocol/ProtocolFactory.h"
#include "ObjectDelivererManager.h"
#include "Tests/ObjectDelivererManagerTestHelper.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProtocolSharedMemoryTest, "ObjectDeliverer.ProtocolTest.ProtocolSharedMemoryTest1", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FProtocolSharedMemoryTest::RunTest(const FString& Parameters)
{

	// send and receive.
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolSharedMemory("shared_memory_test", 2048), UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolSharedMemory("shared_memory_test", 2048), UPacketRuleFactory::CreatePacketRuleSizeBody());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient]()
	{
		TArray<uint8> sendbuffer;
		sendbuffer.SetNum(1000);
		for (int i = 0; i < 1000; ++i)
		{
			sendbuffer[i] = i;	
		}
		ObjectDelivererClient->Send(sendbuffer);
		return true;
	}));

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
	{
		TestEqual("check received count", serverHelper->ReceiveBuffers.Num(), 1);
		if (serverHelper->ReceiveBuffers.Num() == 1)
		{
			TArray<uint8>& receivebuf = serverHelper->ReceiveBuffers[0];

			for (int i = 0; i < 1000; ++i)
			{
				TestEqual("check received data", receivebuf[i], (uint8)i);
			}
		}
		ObjectDelivererClient->Close();
		ObjectDelivererServer->Close();
		return true;
	}));

	return true;
}
#endif