#include "CoreMinimal.h"
#include "AutomationTest.h"
#include "AutomationCommon.h"
#include "ProtocolTcpIpClient.h"
#include "ProtocolTcpIpServer.h"
#include "PacketRuleSizeBody.h"
#include "PacketRuleFactory.h"
#include "ProtocolFactory.h"
#include "ObjectDelivererManager.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest1, "ObjectDeliverer.ProtocolTest.CNTcpIpServerClientTest1", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest1::RunTest(const FString& Parameters)
{
	// connect client to server and close client.
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 1);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient]()
	{
		ObjectDelivererClient->Close();
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 1);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

		ObjectDelivererClient->Close();
		ObjectDelivererServer->Close();
		return true;
	}));
	
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest2, "ObjectDeliverer.ProtocolTest.CNTcpIpServerClientTest2", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest2::RunTest(const FString& Parameters)
{
	// connect client to server and close server.
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 1);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererServer]()
	{
		ObjectDelivererServer->Close();
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 0);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 1);

		ObjectDelivererClient->Close();
		ObjectDelivererServer->Close();
		return true;
	}));


	return true;
}



IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest3, "ObjectDeliverer.ProtocolTest.CNTcpIpServerClientTest3", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest3::RunTest(const FString& Parameters)
{

	// connect client to server(not equal port)
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9100), UPacketRuleFactory::CreatePacketRuleSizeBody());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 0);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 0);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 0);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

		ObjectDelivererClient->Close();
		ObjectDelivererServer->Close();
		return true;
	}));


	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest4, "ObjectDeliverer.ProtocolTest.CNTcpIpServerClientTest4", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest4::RunTest(const FString& Parameters)
{

	// connect multi client to server
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto ObjectDelivererClient2 = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient2->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient2->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient2->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto ObjectDelivererClient3 = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient3->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient3->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient3->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 3);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 3);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 0);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

		ObjectDelivererClient->Close();
		ObjectDelivererServer->Close();
		return true;
	}));


	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest5, "ObjectDeliverer.ProtocolTest.CNTcpIpServerClientTest5", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest5::RunTest(const FString& Parameters)
{

	// don't connected client to server.(not retry)
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099, false), UPacketRuleFactory::CreatePacketRuleSizeBody());
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererServer]()
	{
		ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());
		return true;
	}));
	

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 0);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 0);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 0);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

		ObjectDelivererClient->Close();
		ObjectDelivererServer->Close();
		return true;
	}));


	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest6, "ObjectDeliverer.ProtocolTest.CNTcpIpServerClientTest6", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest6::RunTest(const FString& Parameters)
{

	// connected client to server.(success retry)
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099, true), UPacketRuleFactory::CreatePacketRuleSizeBody());
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererServer]()
	{
		ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());
		return true;
	}));
	


	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
	{
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 1);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
	{
		TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 0);
		TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

		ObjectDelivererClient->Close();
		ObjectDelivererServer->Close();
		return true;
	}));


	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNTcpIpServerClientTest7, "ObjectDeliverer.ProtocolTest.CNTcpIpServerClientTest7", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCNTcpIpServerClientTest7::RunTest(const FString& Parameters)
{

	// send and receive.
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099, true), UPacketRuleFactory::CreatePacketRuleSizeBody());

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

	return true;
}