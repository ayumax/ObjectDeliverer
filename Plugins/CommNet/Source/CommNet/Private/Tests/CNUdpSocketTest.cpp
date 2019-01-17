#include "CoreMinimal.h"
#include "AutomationTest.h"
#include "AutomationCommon.h"
#include "CNUdpSocketSender.h"
#include "CNUdpSocketReceiver.h"
#include "CNPacketRuleSizeBody.h"
#include "CNPacketRuleFactory.h"
#include "CNProtocolFactory.h"
#include "CommNetManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNUdpTest1, "CommNet.ProtocolTest.CNUdpTest1", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNUdpTest1::RunTest(const FString& Parameters)
{

	// send and receive.
	auto serverHelper = NewObject<UCommNetManagerTestHelper>();
	auto commnetServer = NewObject<UCommNetManager>();
	commnetServer->Connected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetServer->Disconnected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnDisConnect);
	commnetServer->ReceiveData.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnReceive);
	commnetServer->Start(UCNProtocolFactory::CreateCNUdpSocketReceiver(9099), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());

	auto clientHelper = NewObject<UCommNetManagerTestHelper>();

	auto commnetClient = NewObject<UCommNetManager>();
	commnetClient->Connected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetClient->Disconnected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnDisConnect);
	commnetClient->Start(UCNProtocolFactory::CreateUdpSocketSender("localhost", 9099), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, commnetClient]()
	{
		for (int i = 0; i < 1000; ++i)
		{
			uint8 data = i;
			TArray<uint8> sendbuffer = { data };
			commnetClient->Send(sendbuffer);
		}
		return true;
	}));

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, commnetClient, commnetServer]()
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
		commnetClient->Close();
		commnetServer->Close();
		return true;
	}));

	return true;
}