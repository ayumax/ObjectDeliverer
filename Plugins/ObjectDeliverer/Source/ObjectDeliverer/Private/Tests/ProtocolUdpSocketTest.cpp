// Copyright 2019 ayumax. All Rights Reserved.
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Protocol/ProtocolUdpSocketSender.h"
#include "Protocol/ProtocolUdpSocketReceiver.h"
#include "PacketRule/PacketRuleSizeBody.h"
#include "PacketRule/PacketRuleNodivision.h"
#include "PacketRule/PacketRuleFactory.h"
#include "Protocol/ProtocolFactory.h"
#include "ObjectDelivererManager.h"
#include "Tests/ObjectDelivererManagerTestHelper.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProtocolUdpTest1, "ObjectDeliverer.ProtocolTest.ProtocolUdpTest1", EAutomationTestFlags::ClientContext | EAutomationTestFlags::StressFilter)

bool FProtocolUdpTest1::RunTest(const FString& Parameters)
{
	{
		// send and receive.
		auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
		auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
		ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
		ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
		ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
		ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

		auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

		auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
		ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
		ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
		ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", 9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient]()
			{
				for (int i = 0; i < 1000; ++i)
				{
					uint8 data = i;
					TArray<uint8> sendbuffer = { data };
					ObjectDelivererClient->Send(sendbuffer);
				}
				return true;
			}));

		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
			{
				TestEqual("check received count", serverHelper->ReceiveBuffers.Num(), 1000);
				if (serverHelper->ReceiveBuffers.Num() == 1000)
				{
					for (int i = 0; i < 1000; ++i)
					{
						TArray<uint8>& receivebuf = serverHelper->ReceiveBuffers[i];
						TestEqual("check received data", receivebuf[0], (uint8)i);
					}
				}
				ObjectDelivererClient->Close();
				ObjectDelivererServer->Close();
				return true;
			}));
	}

	{
		// send and receive.
		auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
		auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
		ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
		ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
		ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
		ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleNodivision());

		auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

		auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
		ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
		ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
		ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", 9099), UPacketRuleFactory::CreatePacketRuleNodivision());

		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient]()
			{
				for (int i = 0; i < 1000; ++i)
				{
					uint8 data = i;
					TArray<uint8> sendbuffer = { data };
					ObjectDelivererClient->Send(sendbuffer);
				}
				return true;
			}));

		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
			{
				TestEqual("check received count", serverHelper->ReceiveBuffers.Num(), 1000);
				if (serverHelper->ReceiveBuffers.Num() == 1000)
				{
					for (int i = 0; i < 1000; ++i)
					{
						TArray<uint8>& receivebuf = serverHelper->ReceiveBuffers[i];
						TestEqual("check received data", receivebuf[0], (uint8)i);
					}
				}
				ObjectDelivererClient->Close();
				ObjectDelivererServer->Close();
				return true;
			}));
	}

	return true;
}
#endif