// Copyright 2019 ayumax. All Rights Reserved.
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Protocol/ProtocolWebSocketClient.h"
#include "PacketRule/PacketRuleSizeBody.h"
#include "PacketRule/PacketRuleFactory.h"
#include "PacketRule/PacketRuleNodivision.h"
#include "Protocol/ProtocolFactory.h"
#include "ObjectDelivererManager.h"
#include "ObjectDelivererManagerTestHelper.h"
#include "Engine/World.h"
#include "Tests/AutomationCommon.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWebSocketClientCreationTest,
								 "ObjectDeliverer.ProtocolWebSocket.ClientCreationTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for WebSocket client creation and basic configuration
 * This test verifies that WebSocket clients can be created with different configurations
 */
bool FWebSocketClientCreationTest::RunTest(const FString &Parameters)
{
	// Get WebSocket test configuration from environment variables
	FString wsServerUrl = FPlatformMisc::GetEnvironmentVariable(TEXT("OBJECTDELIVERER_TEST_WS_URL"));
	FString wsToken = FPlatformMisc::GetEnvironmentVariable(TEXT("OBJECTDELIVERER_TEST_WS_TOKEN"));
	
	// Use default values for local testing if environment variables are not set
	if (wsServerUrl.IsEmpty())
	{
		// Default to a local mock server for testing without credentials
		wsServerUrl = TEXT("ws://localhost:8080");
		UE_LOG(LogTemp, Warning, TEXT("OBJECTDELIVERER_TEST_WS_URL not set, using default: %s"), *wsServerUrl);
	}
	
	// Build URL with token if provided
	FString baseUrl = wsServerUrl;
	if (!wsToken.IsEmpty())
	{
		baseUrl = FString::Printf(TEXT("%s/?token=%s"), *wsServerUrl, *wsToken);
	}

	// Test basic client creation
	FString basicUrl = wsToken.IsEmpty() ? baseUrl : FString::Printf(TEXT("%s&session=test-session-creation-basic"), *baseUrl);
	auto basicClient = UProtocolFactory::CreateProtocolWebSocketClient(basicUrl);
	TestNotNull("Basic WebSocket client should be created", basicClient);
	TestEqual("Basic client URL should be set", basicClient->ServerUrl, basicUrl);

	// Test client creation with protocols
	TArray<FString> protocols = {"objectdeliverer-v1", "binary"};
	FString protocolsUrl = wsToken.IsEmpty() ? baseUrl : FString::Printf(TEXT("%s&session=test-session-creation-protocols"), *baseUrl);
	auto clientWithProtocols = UProtocolFactory::CreateProtocolWebSocketClientWithProtocols(protocolsUrl, protocols);
	TestNotNull("WebSocket client with protocols should be created", clientWithProtocols);
	TestEqual("Client with protocols URL should be set", clientWithProtocols->ServerUrl, protocolsUrl);
	TestEqual("Client protocols should be set", clientWithProtocols->SubProtocols.Num(), 2);

	// Test client creation with headers
	TMap<FString, FString> headers;
	headers.Add("User-Agent", "ObjectDeliverer-Test");
	headers.Add("X-Custom-Header", "TestValue");
	FString headersUrl = wsToken.IsEmpty() ? baseUrl : FString::Printf(TEXT("%s&session=test-session-creation-headers"), *baseUrl);
	auto clientWithHeaders = UProtocolFactory::CreateProtocolWebSocketClientWithHeaders(headersUrl, protocols, headers);
	TestNotNull("WebSocket client with headers should be created", clientWithHeaders);
	TestEqual("Client with headers URL should be set", clientWithHeaders->ServerUrl, headersUrl);
	TestEqual("Client headers should be set", clientWithHeaders->ConnectionHeaders.Num(), 2);

	// Test auto-reconnect functionality
	basicClient->SetAutoReconnect(true);
	TestEqual("Auto-reconnect should be enabled", basicClient->bAutoReconnect, true);

	basicClient->SetReconnectDelay(3.0f);
	TestEqual("Reconnect delay should be set", basicClient->ReconnectDelay, 3.0f);

	// Test protocol manager integration with connection check
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->ReceiveData.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	
	// Start client
	FString testUrl = wsToken.IsEmpty() ? baseUrl : FString::Printf(TEXT("%s&session=test-session-creation-manager"), *baseUrl);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolWebSocketClient(testUrl),
								 UPacketRuleFactory::CreatePacketRuleNodivision());

	// Wait for connection
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));

	// Check connection status
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, clientHelper]()
	{
		TestTrue("Client should be connected", ObjectDelivererClient->IsConnected());
		TestEqual("Connection callback should be triggered", clientHelper->ConnectedSocket.Num(), 1);
		return true;
	}));

	// Cleanup
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient]()
	{
		ObjectDelivererClient->Close();
		return true;
	}));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWebSocketClientConfigurationTest,
								 "ObjectDeliverer.ProtocolWebSocket.ClientConfigurationTest", 
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for WebSocket client configuration options
 */
bool FWebSocketClientConfigurationTest::RunTest(const FString &Parameters)
{
	auto client = UProtocolFactory::CreateProtocolWebSocketClient("ws://test.example.com:9000");
	
	// Test URL configuration
	client->Initialize("ws://updated.example.com:9001");
	TestEqual("URL should be updated", client->ServerUrl, FString("ws://updated.example.com:9001"));

	// Test protocols configuration
	TArray<FString> testProtocols = {"protocol1", "protocol2", "protocol3"};
	client->InitializeWithProtocols("ws://protocol.test.com", testProtocols);
	TestEqual("URL should be updated with protocols", client->ServerUrl, FString("ws://protocol.test.com"));
	TestEqual("Protocols should be set", client->SubProtocols.Num(), 3);
	TestEqual("First protocol should match", client->SubProtocols[0], FString("protocol1"));

	// Test headers configuration
	TMap<FString, FString> testHeaders;
	testHeaders.Add("Authorization", "Bearer token123");
	testHeaders.Add("X-API-Key", "key456");
	client->InitializeWithHeaders("ws://headers.test.com", testProtocols, testHeaders);
	TestEqual("URL should be updated with headers", client->ServerUrl, FString("ws://headers.test.com"));
	TestEqual("Headers should be set", client->ConnectionHeaders.Num(), 2);
	TestTrue("Authorization header should be set", client->ConnectionHeaders.Contains("Authorization"));
	TestEqual("Authorization header value should match", client->ConnectionHeaders["Authorization"], FString("Bearer token123"));

	// Test reconnection settings
	client->SetAutoReconnect(true);
	client->SetReconnectDelay(5.5f);
	TestEqual("Auto-reconnect should be enabled", client->bAutoReconnect, true);
	TestEqual("Reconnect delay should be set correctly", client->ReconnectDelay, 5.5f);

	// Test minimum reconnect delay
	client->SetReconnectDelay(0.05f); // Very small value
	TestTrue("Reconnect delay should have minimum value", client->ReconnectDelay >= 0.1f);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWebSocketClientEchoTest,
								 "ObjectDeliverer.ProtocolWebSocket.ClientEchoTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for WebSocket client echo functionality
 * This test verifies that WebSocket clients can send and receive messages through echo server
 */
bool FWebSocketClientEchoTest::RunTest(const FString &Parameters)
{
	// Get WebSocket test configuration from environment variables
	FString wsServerUrl = FPlatformMisc::GetEnvironmentVariable(TEXT("OBJECTDELIVERER_TEST_WS_URL"));
	FString wsToken = FPlatformMisc::GetEnvironmentVariable(TEXT("OBJECTDELIVERER_TEST_WS_TOKEN"));
	
	// Use default values for local testing if environment variables are not set
	if (wsServerUrl.IsEmpty())
	{
		wsServerUrl = TEXT("ws://localhost:8080");
		UE_LOG(LogTemp, Warning, TEXT("OBJECTDELIVERER_TEST_WS_URL not set, using default: %s"), *wsServerUrl);
	}
	
	// Build URL with token if provided
	FString baseUrl = wsServerUrl;
	if (!wsToken.IsEmpty())
	{
		// Use timestamp to ensure unique session ID
		int64 Timestamp = FDateTime::Now().GetTicks();
		baseUrl = FString::Printf(TEXT("%s/?token=%s&session=test-echo-%lld"), *wsServerUrl, *wsToken, Timestamp);
	}

	// Create client and helper
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	
	// Setup callbacks
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->ReceiveData.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	
	// Start client
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolWebSocketClient(baseUrl),
								 UPacketRuleFactory::CreatePacketRuleNodivision());

	// Wait for connection
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));


	// Send test message
	FString testMessage = TEXT("Hello WebSocket Echo Server!");
	TArray<uint8> messageData;
	FTCHARToUTF8 UTF8String(*testMessage);
	messageData.Append((uint8*)UTF8String.Get(), UTF8String.Length());
	
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, messageData]()
	{
		TestTrue("Client should be connected before sending", ObjectDelivererClient->IsConnected());
		ObjectDelivererClient->Send(messageData);
		return true;
	}));

	// Wait for echo response
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));

	// Verify echo response
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, clientHelper, testMessage]()
	{
		TestTrue("Should receive echo response", clientHelper->ReceiveBuffers.Num() > 0);
		
		if (clientHelper->ReceiveBuffers.Num() > 0)
		{
			// Convert received data to string
			const TArray<uint8>& receivedData = clientHelper->ReceiveBuffers[0];
			FString receivedMessage = FString(FUTF8ToTCHAR((const ANSICHAR*)receivedData.GetData(), receivedData.Num()).Get());
			TestEqual("Echo message should match sent message", receivedMessage, testMessage);
		}
		return true;
	}));

	// Send multiple messages
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, clientHelper]()
	{
		clientHelper->ReceiveBuffers.Empty(); // Clear previous messages
		
		// Send multiple messages
		for (int32 i = 0; i < 5; i++)
		{
			FString msg = FString::Printf(TEXT("Test Message #%d"), i);
			TArray<uint8> data;
			FTCHARToUTF8 UTF8Msg(*msg);
			data.Append((uint8*)UTF8Msg.Get(), UTF8Msg.Length());
			ObjectDelivererClient->Send(data);
		}
		return true;
	}));

	// Wait for all echo responses
 	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));

	// Verify all messages were echoed back
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, clientHelper]()
	{
		TestEqual("Should receive all 5 echo responses", clientHelper->ReceiveBuffers.Num(), 5);
		
		// Verify each message
		for (int32 i = 0; i < clientHelper->ReceiveBuffers.Num(); i++)
		{
			const TArray<uint8>& receivedData = clientHelper->ReceiveBuffers[i];
			FString receivedMessage = FString(FUTF8ToTCHAR((const ANSICHAR*)receivedData.GetData(), receivedData.Num()).Get());
			FString expectedMessage = FString::Printf(TEXT("Test Message #%d"), i);
			TestEqual(FString::Printf(TEXT("Echo message #%d should match"), i), receivedMessage, expectedMessage);
		}
		return true;
	}));

	// Cleanup
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient]()
	{
		ObjectDelivererClient->Close();
		return true;
	}));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWebSocketClientConnectionTimeoutTest,
								 "ObjectDeliverer.ProtocolWebSocket.ClientConnectionTimeoutTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for WebSocket client connection timeout
 * This test verifies that client properly handles connection timeouts to invalid servers
 */
bool FWebSocketClientConnectionTimeoutTest::RunTest(const FString &Parameters)
{
	// Use an invalid URL that will timeout
	FString invalidUrl = TEXT("ws://192.0.2.0:9999"); // TEST-NET-1 address, guaranteed not to be routed

	// Create client and helper
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	
	// Setup callbacks
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	
	// Start client with invalid URL
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolWebSocketClient(invalidUrl),
								 UPacketRuleFactory::CreatePacketRuleNodivision());

	// Wait for connection timeout (typically 10-30 seconds)
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(5.0f));

	// Verify connection failed
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, clientHelper]()
	{
		TestFalse("Client should not be connected to invalid server", ObjectDelivererClient->IsConnected());
		TestEqual("No connection callbacks should be triggered", clientHelper->ConnectedSocket.Num(), 0);
		return true;
	}));

	// Cleanup
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient]()
	{
		ObjectDelivererClient->Close();
		return true;
	}));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWebSocketClientReconnectTest,
								 "ObjectDeliverer.ProtocolWebSocket.ClientReconnectTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for WebSocket client disconnection and reconnection
 * This test verifies that client can properly disconnect and reconnect
 */
bool FWebSocketClientReconnectTest::RunTest(const FString &Parameters)
{
	
	// Get WebSocket test configuration from environment variables
	FString wsServerUrl = FPlatformMisc::GetEnvironmentVariable(TEXT("OBJECTDELIVERER_TEST_WS_URL"));
	FString wsToken = FPlatformMisc::GetEnvironmentVariable(TEXT("OBJECTDELIVERER_TEST_WS_TOKEN"));
	
	// Use default values for local testing if environment variables are not set
	if (wsServerUrl.IsEmpty())
	{
		wsServerUrl = TEXT("ws://localhost:8080");
		UE_LOG(LogTemp, Warning, TEXT("OBJECTDELIVERER_TEST_WS_URL not set, using default: %s"), *wsServerUrl);
	}
	
	// Build URL with token if provided
	FString baseUrl = wsServerUrl;
	if (!wsToken.IsEmpty())
	{
		// Use timestamp to ensure unique session ID
		int64 Timestamp = FDateTime::Now().GetTicks();
		baseUrl = FString::Printf(TEXT("%s/?token=%s&session=test-reconnect-%lld"), *wsServerUrl, *wsToken, Timestamp);
	}

	// Create client and helper
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	
	// Setup callbacks
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->ReceiveData.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	
	// Create client with auto-reconnect disabled for manual control
	auto wsClient = UProtocolFactory::CreateProtocolWebSocketClient(baseUrl);
	wsClient->SetAutoReconnect(false);
	
	// Start client
	ObjectDelivererClient->Start(wsClient, UPacketRuleFactory::CreatePacketRuleNodivision());

	// Wait for initial connection
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));

	// Verify initial connection
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, clientHelper]()
	{
		TestTrue("Client should be initially connected", ObjectDelivererClient->IsConnected());
		TestEqual("Should have one connection callback", clientHelper->ConnectedSocket.Num(), 1);
		return true;
	}));

	// Send a test message before disconnect
	FString testMessage = TEXT("Message before disconnect");
	TArray<uint8> messageData;
	FTCHARToUTF8 UTF8String(*testMessage);
	messageData.Append((uint8*)UTF8String.Get(), UTF8String.Length());
	
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient, messageData]()
	{
		ObjectDelivererClient->Send(messageData);
		return true;
	}));

	// Wait for message to be sent and echoed
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));

	// Store the current protocol before closing to check its state directly
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([clientHelper](){ 
		// Clear the disconnect callback counter before closing
		clientHelper->DisconnectedSocket.Empty();
		return true;
	}));

	// Disconnect
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient]()
	{
		ObjectDelivererClient->Close();
		return true;
	}));

	// Wait for disconnect to complete
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));

	// Verify disconnection
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient]()
	{
		TestFalse("Client should be disconnected after Close()", ObjectDelivererClient->IsConnected());
		return true;
	}));

	// Prepare for reconnection  
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([clientHelper]()
	{
		// Clear previous connection states for reconnection test
		clientHelper->ConnectedSocket.Empty();
		clientHelper->ReceiveBuffers.Empty();
		return true;
	}));

	// Reconnect
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient, baseUrl]()
	{
		// Create new client for reconnection
		auto newWsClient = UProtocolFactory::CreateProtocolWebSocketClient(baseUrl);
		ObjectDelivererClient->Start(newWsClient, UPacketRuleFactory::CreatePacketRuleNodivision());
		return true;
	}));

	// Wait for reconnection
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));

	// Verify reconnection
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, clientHelper]()
	{
		TestTrue("Client should be reconnected", ObjectDelivererClient->IsConnected());
		TestEqual("Should have new connection callback", clientHelper->ConnectedSocket.Num(), 1);
		return true;
	}));

	// Send message after reconnect to verify functionality
	FString reconnectMessage = TEXT("Message after reconnect");
	TArray<uint8> reconnectData;
	FTCHARToUTF8 UTF8ReconnectString(*reconnectMessage);
	reconnectData.Append((uint8*)UTF8ReconnectString.Get(), UTF8ReconnectString.Length());
	
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient, reconnectData]()
	{
		ObjectDelivererClient->Send(reconnectData);
		return true;
	}));

	// Wait for echo
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));

	// Verify message after reconnect
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, clientHelper, reconnectMessage]()
	{
		TestTrue("Should receive message after reconnect", clientHelper->ReceiveBuffers.Num() > 0);
		
		if (clientHelper->ReceiveBuffers.Num() > 0)
		{
			const TArray<uint8>& receivedData = clientHelper->ReceiveBuffers.Last();
			FString receivedMessage = FString(FUTF8ToTCHAR((const ANSICHAR*)receivedData.GetData(), receivedData.Num()).Get());
			TestEqual("Reconnect message should match", receivedMessage, reconnectMessage);
		}
		return true;
	}));

	// Final cleanup
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ObjectDelivererClient]()
	{
		ObjectDelivererClient->Close();
		return true;
	}));

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS