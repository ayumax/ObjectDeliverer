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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProtocolUdpTest1, "ObjectDeliverer.ProtocolTest.ProtocolUdpTest1", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FProtocolUdpTest1::RunTest(const FString& Parameters)
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

	const int udpSendMaxSize = 65507 - 4;

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, udpSendMaxSize]()
		{
			for (int count = 0; count < 3; ++count)
			{
				TArray<uint8> sendbuffer;

				for (int i = 0; i < udpSendMaxSize; ++i)
				{
					sendbuffer.Add((uint8)i);
				}

				ObjectDelivererClient->Send(sendbuffer);
			}
				
			return true;
		}));

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer, udpSendMaxSize]()
		{
			TestEqual("check received count", serverHelper->ReceiveBuffers.Num() > 0, true);

			if (serverHelper->ReceiveBuffers.Num() > 0)
			{
				TArray<uint8>& receiveBuffer = serverHelper->ReceiveBuffers[0];
				for (int i = 0; i < udpSendMaxSize; ++i)
				{
					TestEqual(*FString::Printf(TEXT("check received data %d, %d"), receiveBuffer[i], i), receiveBuffer[i], (uint8)i);
				}
			}

			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true;
		}));
	
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProtocolUdpTest2, "ObjectDeliverer.ProtocolTest.ProtocolUdpTest2", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FProtocolUdpTest2::RunTest(const FString& Parameters)
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

	const int udpSendMaxSize = 65507;


	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, udpSendMaxSize]()
		{
			for (int count = 0; count < 3; ++count)
			{
				TArray<uint8> sendbuffer;

				for (int i = 0; i < udpSendMaxSize; ++i)
				{
					sendbuffer.Add((uint8)i);
				}

				ObjectDelivererClient->Send(sendbuffer);
			}
			return true;
		}));

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer, udpSendMaxSize]()
		{
			TestEqual("check received count", serverHelper->ReceiveBuffers.Num() > 0, true);

			if (serverHelper->ReceiveBuffers.Num() > 0)
			{
				TArray<uint8>& receiveBuffer = serverHelper->ReceiveBuffers[0];
				for (int i = 0; i < udpSendMaxSize; ++i)
				{
					TestEqual("check received data", receiveBuffer[i], (uint8)i);
				}
			}

			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true;
		}));

	return true;
}
#endif