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
	// Test basic client creation
	auto basicClient = UProtocolFactory::CreateProtocolWebSocketClient("ws://localhost:8080");
	TestNotNull("Basic WebSocket client should be created", basicClient);
	TestEqual("Basic client URL should be set", basicClient->ServerUrl, FString("ws://localhost:8080"));

	// Test client creation with protocols
	TArray<FString> protocols = {"objectdeliverer-v1", "binary"};
	auto clientWithProtocols = UProtocolFactory::CreateProtocolWebSocketClientWithProtocols("ws://localhost:9090", protocols);
	TestNotNull("WebSocket client with protocols should be created", clientWithProtocols);
	TestEqual("Client with protocols URL should be set", clientWithProtocols->ServerUrl, FString("ws://localhost:9090"));
	TestEqual("Client protocols should be set", clientWithProtocols->SubProtocols.Num(), 2);

	// Test client creation with headers
	TMap<FString, FString> headers;
	headers.Add("User-Agent", "ObjectDeliverer-Test");
	headers.Add("X-Custom-Header", "TestValue");
	auto clientWithHeaders = UProtocolFactory::CreateProtocolWebSocketClientWithHeaders("wss://secure.example.com", protocols, headers);
	TestNotNull("WebSocket client with headers should be created", clientWithHeaders);
	TestEqual("Client with headers URL should be set", clientWithHeaders->ServerUrl, FString("wss://secure.example.com"));
	TestEqual("Client headers should be set", clientWithHeaders->ConnectionHeaders.Num(), 2);

	// Test auto-reconnect functionality
	basicClient->SetAutoReconnect(true);
	TestEqual("Auto-reconnect should be enabled", basicClient->bAutoReconnect, true);

	basicClient->SetReconnectDelay(3.0f);
	TestEqual("Reconnect delay should be set", basicClient->ReconnectDelay, 3.0f);

	// Test protocol manager integration
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	
	// Start client (will fail to connect since no server, but shouldn't crash)
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolWebSocketClient("ws://localhost:8080"),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait a moment for connection attempt
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));

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

#endif // WITH_DEV_AUTOMATION_TESTS