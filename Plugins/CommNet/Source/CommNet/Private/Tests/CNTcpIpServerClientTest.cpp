#include "CoreMinimal.h"
#include "AutomationTest.h"
#include "AutomationCommon.h"
#include "CNTcpIpClient.h"
#include "CNTcpIpServer.h"
#include "CNPacketRuleSizeBody.h"
#include "CNPacketRuleFactory.h"
#include "CNProtocolFactory.h"
#include "CommNetManager.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest1, "CommNet.ProtocolTest.CNTcpIpServerClientTest1", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest1::RunTest(const FString& Parameters)
{
	// connect client to server and close client.
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
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 1);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([commnetClient]()
	{
		commnetClient->Close();
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, commnetClient, commnetServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 1);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

		commnetClient->Close();
		commnetServer->Close();
		return true;
	}));
	
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest2, "CommNet.ProtocolTest.CNTcpIpServerClientTest2", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest2::RunTest(const FString& Parameters)
{
	// connect client to server and close server.
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
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 1);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([commnetServer]()
	{
		commnetServer->Close();
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, commnetClient, commnetServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 0);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 1);

		commnetClient->Close();
		commnetServer->Close();
		return true;
	}));


	return true;
}



IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest3, "CommNet.ProtocolTest.CNTcpIpServerClientTest3", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest3::RunTest(const FString& Parameters)
{

	// connect client to server(not equal port)
	auto serverHelper = NewObject<UCommNetManagerTestHelper>();

	auto commnetServer = NewObject<UCommNetManager>();
	commnetServer->Connected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetServer->Disconnected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnDisConnect);
	commnetServer->Start(UCNProtocolFactory::CreateTcpIpServer(9099), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());

	auto clientHelper = NewObject<UCommNetManagerTestHelper>();

	auto commnetClient = NewObject<UCommNetManager>();
	commnetClient->Connected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetClient->Disconnected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnDisConnect);
	commnetClient->Start(UCNProtocolFactory::CreateTcpIpClient("localhost", 9100), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 0);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 0);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, commnetClient, commnetServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 0);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

		commnetClient->Close();
		commnetServer->Close();
		return true;
	}));


	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest4, "CommNet.ProtocolTest.CNTcpIpServerClientTest4", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest4::RunTest(const FString& Parameters)
{

	// connect multi client to server
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

	auto commnetClient2 = NewObject<UCommNetManager>();
	commnetClient2->Connected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetClient2->Disconnected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnDisConnect);
	commnetClient2->Start(UCNProtocolFactory::CreateTcpIpClient("localhost", 9099), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());

	auto commnetClient3 = NewObject<UCommNetManager>();
	commnetClient3->Connected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetClient3->Disconnected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnDisConnect);
	commnetClient3->Start(UCNProtocolFactory::CreateTcpIpClient("localhost", 9099), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 3);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 3);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, commnetClient, commnetServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 0);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

		commnetClient->Close();
		commnetServer->Close();
		return true;
	}));


	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest5, "CommNet.ProtocolTest.CNTcpIpServerClientTest5", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest5::RunTest(const FString& Parameters)
{

	// don't connected client to server.(not retry)
	auto clientHelper = NewObject<UCommNetManagerTestHelper>();
	
	auto commnetClient = NewObject<UCommNetManager>();
	commnetClient->Connected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetClient->Disconnected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnDisConnect);
	commnetClient->Start(UCNProtocolFactory::CreateTcpIpClient("localhost", 9099, false), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());
	auto serverHelper = NewObject<UCommNetManagerTestHelper>();

	auto commnetServer = NewObject<UCommNetManager>();
	commnetServer->Connected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetServer->Disconnected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnDisConnect);

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, commnetServer]()
	{
		commnetServer->Start(UCNProtocolFactory::CreateTcpIpServer(9099), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());
		return true;
	}));
	

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 0);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 0);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, commnetClient, commnetServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 0);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

		commnetClient->Close();
		commnetServer->Close();
		return true;
	}));


	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest6, "CommNet.ProtocolTest.CNTcpIpServerClientTest6", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest6::RunTest(const FString& Parameters)
{

	// connected client to server.(success retry)
	auto clientHelper = NewObject<UCommNetManagerTestHelper>();

	auto commnetClient = NewObject<UCommNetManager>();
	commnetClient->Connected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetClient->Disconnected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnDisConnect);
	commnetClient->Start(UCNProtocolFactory::CreateTcpIpClient("localhost", 9099, true), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());
	auto serverHelper = NewObject<UCommNetManagerTestHelper>();

	auto commnetServer = NewObject<UCommNetManager>();
	commnetServer->Connected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetServer->Disconnected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnDisConnect);

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, commnetServer]()
	{
		commnetServer->Start(UCNProtocolFactory::CreateTcpIpServer(9099), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());
		return true;
	}));
	


	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 1);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, commnetClient, commnetServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 0);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

		commnetClient->Close();
		commnetServer->Close();
		return true;
	}));


	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest7, "CommNet.ProtocolTest.CNTcpIpServerClientTest7", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest7::RunTest(const FString& Parameters)
{

	// send and receive.
	auto serverHelper = NewObject<UCommNetManagerTestHelper>();
	auto commnetServer = NewObject<UCommNetManager>();
	commnetServer->Connected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetServer->Disconnected.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnDisConnect);
	commnetServer->ReceiveData.AddDynamic(serverHelper, &UCommNetManagerTestHelper::OnReceive);
	commnetServer->Start(UCNProtocolFactory::CreateTcpIpServer(9099), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());

	auto clientHelper = NewObject<UCommNetManagerTestHelper>();

	auto commnetClient = NewObject<UCommNetManager>();
	commnetClient->Connected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnConnect);
	commnetClient->Disconnected.AddDynamic(clientHelper, &UCommNetManagerTestHelper::OnDisConnect);
	commnetClient->Start(UCNProtocolFactory::CreateTcpIpClient("localhost", 9099, true), UCNPacketRuleFactory::CreateCNPacketRuleSizeBody());

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