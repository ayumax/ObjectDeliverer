// Copyright 2019 ayumax. All Rights Reserved.
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Protocol/ProtocolTcpIpClient.h"
#include "Protocol/ProtocolTcpIpServer.h"
#include "PacketRule/PacketRuleSizeBody.h"
#include "PacketRule/PacketRuleFactory.h"
#include "PacketRule/PacketRuleNodivision.h"
#include "Protocol/ProtocolFactory.h"
#include "ObjectDelivererManager.h"
#include "ObjectDelivererManagerTestHelper.h"

#if WITH_DEV_AUTOMATION_TESTS

/**
 * Helper function to get an available TCP port for testing
 * Returns a unique port number for each call to avoid port conflicts between tests
 */
int32 GetAvailableTCPPort()
{
	static int32 BasePort = 9013;
	return BasePort++;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FClientDisconnectionTest,
								 "ObjectDeliverer.ProtocolTCP.ClientDisconnectionTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for client disconnection scenario
 * This test connects a client to a server and then closes the client to verify proper disconnection handling
 */
bool FClientDisconnectionTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();

	// Connect client to server and close client
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for connection to establish
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Verify connection is established
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
														 {
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 1);
		return true; }));

	// Disconnect client
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient]()
														 {
		ObjectDelivererClient->Close();
		return true; }));

	// Wait for disconnection to propagate
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	// Verify server was notified of client disconnection
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			TestEqual("check disconnected server", serverHelper->DisconnectedSocket.Num(), 1);
			TestEqual("check disconnected client", clientHelper->DisconnectedSocket.Num(), 0);

			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProtocolTcpIpServerClientTest1,
								 "ObjectDeliverer.ProtocolTCP.ProtocolTcpIpServerClientTest1",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FProtocolTcpIpServerClientTest1::RunTest(const FString &Parameters)
{
	int port = GetAvailableTCPPort();

	// connect client to server and close client.
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
														 {
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 1);
		return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient]()
														 {
		ObjectDelivererClient->Close();
		return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			TestEqual("check diconnected server", serverHelper->DisconnectedSocket.Num(), 1);
			TestEqual("check diconnected client", clientHelper->DisconnectedSocket.Num(), 0);

			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FServerDisconnectionTest,
								 "ObjectDeliverer.ProtocolTCP.ServerDisconnectionTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for server disconnection scenario
 * This test connects a client to a server and then closes the server to verify proper disconnection handling
 */
bool FServerDisconnectionTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();

	// Connect client to server and then close server
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for connection to establish
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Verify connection is established
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
														 {
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 1);
		return true; }));

	// Disconnect server
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererServer]()
														 {
		ObjectDelivererServer->Close();
		return true; }));

	// Wait for disconnection to propagate
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	// Verify client was notified of server disconnection
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			TestEqual("check disconnected server", serverHelper->DisconnectedSocket.Num(), 0);
			TestEqual("check disconnected client", clientHelper->DisconnectedSocket.Num(), 1);

			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInvalidPortConnectionTest,
								 "ObjectDeliverer.ProtocolTCP.InvalidPortConnectionTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for connection attempt with incorrect port number
 * This test tries to connect a client to a server with a different port, expecting no connection
 */
bool FInvalidPortConnectionTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();

	// Setup server on one port
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	// Try to connect client with incorrect port (port + 1)
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port + 1),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for attempted connection
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Verify no connection was established
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
														 {
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 0);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 0);
		return true; }));

	// Verify no disconnection events were fired
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			TestEqual("check disconnected server", serverHelper->DisconnectedSocket.Num(), 0);
			TestEqual("check disconnected client", clientHelper->DisconnectedSocket.Num(), 0);

			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMultiClientConnectionBasicTest,
								 "ObjectDeliverer.ProtocolTCP.MultiClientConnectionBasicTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for multiple clients connecting to a single server
 * This test connects three clients to one server and verifies all connections are established
 */
bool FMultiClientConnectionBasicTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();

	// Setup server
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	// Create and connect first client
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Create and connect second client
	auto ObjectDelivererClient2 = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient2->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient2->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient2->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port),
								  UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Create and connect third client
	auto ObjectDelivererClient3 = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient3->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient3->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient3->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port),
								  UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for all connections to establish
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Verify all connections were established
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
														 {
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 3);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 3);
		return true; }));

	// Verify no disconnection events occurred and cleanup
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererClient2,
								ObjectDelivererClient3, ObjectDelivererServer]()
							   {
			TestEqual("check disconnected server", serverHelper->DisconnectedSocket.Num(), 0);
			TestEqual("check disconnected client", clientHelper->DisconnectedSocket.Num(), 0);

			ObjectDelivererClient->Close();
			ObjectDelivererClient2->Close();
			ObjectDelivererClient3->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNoRetryConnectionTest,
								 "ObjectDeliverer.ProtocolTCP.NoRetryConnectionTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for client with no retry behavior when server is not available at start
 * This test attempts to connect a client to a non-existent server with retry disabled,
 * then starts the server later, expecting no connection to be established
 */
bool FNoRetryConnectionTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();

	// Setup client with retry disabled (false)
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port, false),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);

	// Wait to ensure client has attempted and failed to connect
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Start server after client has likely given up
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererServer, port]()
														 {
		ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
		                            UPacketRuleFactory::CreatePacketRuleSizeBody());
		return true; }));

	// Wait for potential connection (should not happen)
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Verify no connection was established
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
														 {
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 0);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 0);
		return true; }));

	// Verify no disconnection events were fired and cleanup
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			TestEqual("check disconnected server", serverHelper->DisconnectedSocket.Num(), 0);
			TestEqual("check disconnected client", clientHelper->DisconnectedSocket.Num(), 0);

			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRetryConnectionSuccessTest,
								 "ObjectDeliverer.ProtocolTCP.RetryConnectionSuccessTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for client with retry behavior when server is not available at start
 * This test attempts to connect a client to a non-existent server with retry enabled,
 * then starts the server later, expecting a connection to be established
 */
bool FRetryConnectionSuccessTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();

	// Setup client with retry enabled (true)
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port, true),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);

	// Wait before starting server to allow client to make initial connection attempts
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Start server after client has started retrying
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererServer, port]()
														 {
		ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
		                            UPacketRuleFactory::CreatePacketRuleSizeBody());
		return true; }));

	// Wait for connection to be established via client's retry mechanism
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(10.0f));

	// Verify connection was established
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
														 {
		TestEqual("check connected server", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("check connected client", clientHelper->ConnectedSocket.Num(), 1);
		return true; }));

	// Verify no disconnection events were fired and cleanup
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			TestEqual("check disconnected server", serverHelper->DisconnectedSocket.Num(), 0);
			TestEqual("check disconnected client", clientHelper->DisconnectedSocket.Num(), 0);

			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDataTransferTest,
								 "ObjectDeliverer.ProtocolTCP.DataTransferTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for data transmission between client and server
 * This test sends multiple data packets from client to server and verifies they are correctly received
 */
bool FDataTransferTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();

	// Setup server with data receiving capability
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Setup client
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port, true),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for connection to establish
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Send 1000 data packets from client to server
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient]()
														 {
		for (int i = 0; i < 1000; ++i)
		{
			uint8 data = i;
			TArray<uint8> sendbuffer = { data };
			ObjectDelivererClient->Send(sendbuffer);
		}
		return true; }));

	// Wait for all data to be received
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Verify data was correctly received
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			// Check total number of received packets
			TestEqual("check received count", serverHelper->ReceiveBuffers.Num(), 1000);

			// Verify content of each received packet
			if (serverHelper->ReceiveBuffers.Num() == 1000)
			{
				for (int i = 0; i < 1000; ++i)
				{
					TArray<uint8>& receivebuf = serverHelper->ReceiveBuffers[i];
					TestEqual("check received data", receivebuf[0], static_cast<uint8>(i));
				}
			}

			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

/**
 * Test for connection error handling
 * This test attempts to connect to a non-existent server and verifies proper error handling
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectionErrorTest,
								 "ObjectDeliverer.ProtocolTCP.ConnectionErrorTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FConnectionErrorTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();

	// Attempt to connect to a non-existent server
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->ReceiveData.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnReceive);

	// Try connecting to a server that doesn't exist
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for connection attempt to time out
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Verify connection was not established
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, clientHelper, ObjectDelivererClient]()
														 {
		// Check that no connection was established
		TestEqual("Client should not be connected", clientHelper->ConnectedSocket.Num(), 0);

		// Verify IsConnected method returns correct status
		TestEqual("IsConnected should return false", ObjectDelivererClient->IsConnected(), false);

		ObjectDelivererClient->Close();
		return true; }));

	return true;
}

/**
 * Test for server disconnection handling
 * This test establishes a connection and then abruptly closes the server
 * to verify that client properly detects the disconnection
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDisconnectionHandlingTest,
								 "ObjectDeliverer.ProtocolTCP.DisconnectionHandlingTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FDisconnectionHandlingTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();

	// Create server
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Connect client
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->ReceiveData.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for connection to establish
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Verify connection was established
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, clientHelper]()
														 {
		TestEqual("Server should have 1 connection", serverHelper->ConnectedSocket.Num(), 1);
		TestEqual("Client should have 1 connection", clientHelper->ConnectedSocket.Num(), 1);
		return true; }));

	// Abruptly close server
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererServer]()
														 {
		ObjectDelivererServer->Close();
		return true; }));

	// Wait for client to detect disconnection
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));

	// Verify client detected the disconnection
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, clientHelper, ObjectDelivererClient]()
														 {
		// Check disconnection callback was called
		TestEqual("Disconnection should be detected", clientHelper->DisconnectedSocket.Num(), 1);

		// Verify IsConnected correctly reflects disconnected state
		TestEqual("IsConnected should return false after server disconnect",
		         ObjectDelivererClient->IsConnected(), false);

		ObjectDelivererClient->Close();
		return true; }));

	return true;
}

/**
 * Test for handling invalid data reception
 * This test connects a client with a different packet rule than the server
 * and sends data to verify server robustness against invalid packet formats
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInvalidDataHandlingTest,
								 "ObjectDeliverer.ProtocolTCP.InvalidDataHandlingTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FInvalidDataHandlingTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();

	// Create server with SizeBody packet rule
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Connect client with a different packet rule (Nodivision)
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->ReceiveData.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnReceive);

	// Connect client with different packet rule
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port),
								 UPacketRuleFactory::CreatePacketRuleNodivision());

	// Wait for connection to establish
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Send invalid packet format from client to server
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient]()
														 {
		// Create invalid formatted data
		TArray<uint8> invalidData;
		for (int i = 0; i < 100; ++i)
		{
			invalidData.Add(static_cast<uint8>(i));
		}

		ObjectDelivererClient->Send(invalidData);
		return true; }));

	// Wait for server to process data
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));

	// Verify server maintains connection despite invalid data
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			// Verify server maintains connection despite invalid data
			TestEqual("Server should maintain connection despite invalid data",
			         ObjectDelivererServer->IsConnected(), true);

			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

/**
 * Comprehensive test for multiple client connections and data transfer
 * This test connects multiple clients, sends data between clients and server,
 * and verifies all data is correctly received by all parties
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMultiClientConnectionTest,
								 "ObjectDeliverer.ProtocolTCP.MultiConnectionTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMultiClientConnectionTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableTCPPort();
	constexpr int32 ClientCount = 5; // Number of simultaneous client connections

	// Create server
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolTcpIpServer(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Arrays for multiple clients
	TArray<UObjectDelivererManager *> Clients;
	TArray<UObjectDelivererManagerTestHelper *> ClientHelpers;

	// Create and connect multiple clients
	for (int32 i = 0; i < ClientCount; ++i)
	{
		auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
		auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();

		ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
		ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
		ObjectDelivererClient->ReceiveData.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnReceive);
		ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", port),
									 UPacketRuleFactory::CreatePacketRuleSizeBody());

		Clients.Add(ObjectDelivererClient);
		ClientHelpers.Add(clientHelper);
	}

	// Wait for all connections to establish
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Verify all connections are established
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, serverHelper, ClientHelpers, ClientCount]()
														 {
		// Verify server accepted all client connections
		TestEqual("Server should have accepted all client connections",
		         serverHelper->ConnectedSocket.Num(), ClientCount);

		// Verify each client established connection
		for (int32 i = 0; i < ClientCount; ++i)
		{
			TestEqual(FString::Printf(TEXT("Client %d should be connected"), i),
			         ClientHelpers[i]->ConnectedSocket.Num(), 1);
		}

		return true; }));

	// Send data from each client to server
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Clients, ClientCount]()
														 {
		for (int32 i = 0; i < ClientCount; ++i)
		{
			// Generate client-specific data
			TArray<uint8> sendBuffer;
			sendBuffer.SetNum(8);

			// Embed client ID for identification
			for (int32 j = 0; j < 8; ++j)
			{
				sendBuffer[j] = i;
			}

			Clients[i]->Send(sendBuffer);
		}
		return true; }));

	// Wait for data reception
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));

	// Broadcast data from server to all clients
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererServer]()
														 {
		// Create broadcast data
		TArray<uint8> broadcastBuffer;
		broadcastBuffer.SetNum(8);

		// Use special value to indicate broadcast data
		for (int32 j = 0; j < 8; ++j)
		{
			broadcastBuffer[j] = 0xFF;
		}

		ObjectDelivererServer->Send(broadcastBuffer);
		return true; }));

	// Wait for broadcast data reception
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));

	// Verify results
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, ClientHelpers, ClientCount, Clients, ObjectDelivererServer]()
							   {
			// Verify server received data from all clients
			TestEqual("Server should have received data from all clients",
			         serverHelper->ReceiveBuffers.Num(), ClientCount);

			// Verify client-specific data
			for (int32 i = 0; i < serverHelper->ReceiveBuffers.Num() && i < ClientCount; ++i)
			{
				// Check data sent by clients (client IDs can arrive in any order)
				bool foundMatch = false;

				for (int32 clientId = 0; clientId < ClientCount; ++clientId)
				{
					bool isMatch = true;

					for (int32 j = 0; j < serverHelper->ReceiveBuffers[i].Num() && j < 8; ++j)
					{
						if (serverHelper->ReceiveBuffers[i][j] != clientId)
						{
							isMatch = false;
							break;
						}
					}

					if (isMatch)
					{
						foundMatch = true;
						break;
					}
				}

				TestTrue(FString::Printf(TEXT("Server should have received valid client data for packet %d"), i),
				        foundMatch);
			}

			// Verify each client received broadcast data
			for (int32 i = 0; i < ClientCount; ++i)
			{
				TestEqual(FString::Printf(TEXT("Client %d should have received broadcast data"), i),
				         ClientHelpers[i]->ReceiveBuffers.Num(), 1);

				if (ClientHelpers[i]->ReceiveBuffers.Num() > 0)
				{
					// Verify broadcast data values (0xFF)
					for (int32 j = 0; j < ClientHelpers[i]->ReceiveBuffers[0].Num() && j < 8; ++j)
					{
						TestEqual(FString::Printf(TEXT("Client %d broadcast data byte %d"), i, j),
						         ClientHelpers[i]->ReceiveBuffers[0][j], static_cast<uint8>(0xFF));
					}
				}
			}

			// Cleanup
			for (auto Client : Clients)
			{
				Client->Close();
			}

			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

#endif
