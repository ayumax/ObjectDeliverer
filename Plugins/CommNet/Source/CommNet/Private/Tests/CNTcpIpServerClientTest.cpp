#include "CoreMinimal.h"
#include "AutomationTest.h"
#include "AutomationCommon.h"
#include "CNTcpIpClient.h"
#include "CNTcpIpServer.h"
#include "CNPacketRuleSizeBody.h"
#include "CNPacketRuleFactory.h"
#include "CNProtocolFactory.h"
#include "CommNetManager.h"

DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(FCNTcpIpServerClientTest_Connected, FAutomationTestBase*, Test, UCommNetManagerTestHelper*, Helper, int32, Count);
bool FCNTcpIpServerClientTest_Connected::Update()
{ 
	check(Test);
	check(Helper);
	Test->TestEqual(TEXT("check connected"), Helper->ConnectedSocket.Num(), Count);
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(FCNTcpIpServerClientTest_Disonnected, FAutomationTestBase*, Test, UCommNetManagerTestHelper*, Helper, int32, Count);
bool FCNTcpIpServerClientTest_Disonnected::Update()
{
	check(Test);
	check(Helper);
	Test->TestEqual(TEXT("check connected"), Helper->DisconnectedSocket.Num(), Count);
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FCNTcpIpServerClientTest_DoFunc, TFunction<void()>, InWork);
bool FCNTcpIpServerClientTest_DoFunc::Update()
{
	InWork();

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest, "CommNet.ProtocolTest.CNTcpIpServerClientTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest::RunTest(const FString& Parameters)
{
	{
		auto serverHelper = NewObject<UCommNetManagerTestHelper>();
		
		auto commnetServer = NewObject<UCommNetManager>();
		commnetServer->Connected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnConnect);
		commnetServer->Disconnected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnDisConnect);
		commnetServer->Start(UCNProtocolFactory::CreateTcpIpServer(9099), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());

		auto clientHelper = NewObject<UCommNetManagerTestHelper>();

		auto commnetClient = NewObject<UCommNetManager>();
		commnetClient->Connected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnConnect);
		commnetClient->Disconnected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnDisConnect);
		commnetClient->Start(UCNProtocolFactory::CreateTcpIpClient("localhost", 9099), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());

		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
		ADD_LATENT_AUTOMATION_COMMAND(FCNTcpIpServerClientTest_Connected(this, serverHelper, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FCNTcpIpServerClientTest_Connected(this, clientHelper, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FCNTcpIpServerClientTest_DoFunc([commnetClient]() { commnetClient->Close(); }));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
		ADD_LATENT_AUTOMATION_COMMAND(FCNTcpIpServerClientTest_Disonnected(this, serverHelper, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FCNTcpIpServerClientTest_Disonnected(this, clientHelper, 0));

	}
	
	return true;
}
