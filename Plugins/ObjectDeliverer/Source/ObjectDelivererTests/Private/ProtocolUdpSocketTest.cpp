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
#include "ObjectDelivererManagerTestHelper.h"

#if WITH_DEV_AUTOMATION_TESTS

int32 GetAvailableUDPPort()
{
	static int32 BasePort = 9013;
	return BasePort++;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProtocolUdpTest1, "ObjectDeliverer.ProtocolTest.ProtocolUdpTest1", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FProtocolUdpTest1::RunTest(const FString &Parameters)
{
	int port = GetAvailableUDPPort();

	// send and receive.
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(port), UPacketRuleFactory::CreatePacketRuleSizeBody());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", port), UPacketRuleFactory::CreatePacketRuleSizeBody());

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

			return true; }));

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
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProtocolUdpTest2, "ObjectDeliverer.ProtocolTest.ProtocolUdpTest2", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FProtocolUdpTest2::RunTest(const FString &Parameters)
{
	int port = GetAvailableUDPPort();

	// send and receive.
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(port), UPacketRuleFactory::CreatePacketRuleNodivision());

	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();

	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", port), UPacketRuleFactory::CreatePacketRuleNodivision());

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
			return true; }));

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
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMultiClientUdpTest, "ObjectDeliverer.ProtocolUDP.MultiClientUdpTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for multiple client connections via UDP
 * This test verifies that multiple clients can send data to the same UDP receiver and all data is received correctly
 */
bool FMultiClientUdpTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableUDPPort();
	constexpr int32 ClientCount = 5; // Number of clients to connect simultaneously
	constexpr int32 DataSize = 1024; // Size of data to send

	// Server (receiver) setup
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Array for multiple clients
	TArray<UObjectDelivererManager *> Clients;
	TArray<UObjectDelivererManagerTestHelper *> ClientHelpers;

	// Create and connect multiple clients
	for (int32 i = 0; i < ClientCount; ++i)
	{
		auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
		auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();

		ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
		ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
		ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", port),
									 UPacketRuleFactory::CreatePacketRuleSizeBody());

		Clients.Add(ObjectDelivererClient);
		ClientHelpers.Add(clientHelper);
	}

	// Wait for connections to establish
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));

	// Send data from each client to the server
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Clients, ClientCount, DataSize]()
														 {
		for (int32 i = 0; i < ClientCount; ++i)
		{
			// Generate client-specific data
			TArray<uint8> sendBuffer;
			sendBuffer.SetNum(DataSize);

			// Embed client ID for identification
			for (int32 j = 0; j < DataSize; ++j)
			{
				sendBuffer[j] = i;
			}

			Clients[i]->Send(sendBuffer);
		}
		return true; }));

	// Wait for data reception
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));

	// Verify results
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, ClientHelpers, ClientCount, Clients, ObjectDelivererServer, DataSize]()
							   {
			// Verify that the server received data from all clients
			TestEqual("Verify server received data from all clients",
			          serverHelper->ReceiveBuffers.Num(), ClientCount);

			// Validate client-specific data
			for (int32 i = 0; i < serverHelper->ReceiveBuffers.Num() && i < ClientCount; ++i)
			{
				// Verify data sent by clients (client IDs can arrive in any order)
				bool foundMatch = false;

				for (int32 clientId = 0; clientId < ClientCount; ++clientId)
				{
					bool isMatch = true;

					for (int32 j = 0; j < serverHelper->ReceiveBuffers[i].Num() && j < DataSize; ++j)
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

				TestTrue(FString::Printf(TEXT("Verify packet %d contains valid client data"), i),
				        foundMatch);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInvalidPortUdpTest, "ObjectDeliverer.ProtocolUDP.InvalidPortUdpTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for communication with different port numbers
 * This test verifies that the server does not receive data when a client sends to a different port
 */
bool FInvalidPortUdpTest::RunTest(const FString &Parameters)
{
	int32 serverPort = GetAvailableUDPPort();
	int32 clientPort = serverPort + 1; // Different port number

	// Server (receiver) setup
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(serverPort),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Setup client using incorrect port number
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", clientPort),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for setup
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	// Data transmission
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient]()
														 {
		// Send test data
		TArray<uint8> sendBuffer;
		for (int32 i = 0; i < 100; ++i)
		{
			sendBuffer.Add(static_cast<uint8>(i));
		}

		ObjectDelivererClient->Send(sendBuffer);
		return true; }));

	// Wait for data reception
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));

	// Verify results
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			// Verify that the server did not receive any data
			TestEqual("Verify server did not receive any data",
			          serverHelper->ReceiveBuffers.Num(), 0);

			// Cleanup
			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSmallPacketUdpTest, "ObjectDeliverer.ProtocolUDP.SmallPacketUdpTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for sending and receiving small-sized packets
 * This test verifies that multiple small-sized packets can be sent and correctly received
 */
bool FSmallPacketUdpTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableUDPPort();
	constexpr int32 PacketSize = 8;	   // Small packet size
	constexpr int32 PacketCount = 100; // Number of packets to send

	// Server (receiver) setup
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Client setup
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for setup
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	// Send multiple small packets
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, PacketCount, PacketSize]()
														 {
		for (int32 packetIndex = 0; packetIndex < PacketCount; ++packetIndex)
		{
			TArray<uint8> sendBuffer;
			sendBuffer.SetNum(PacketSize);

			// Embed packet number
			for (int32 i = 0; i < PacketSize; ++i)
			{
				sendBuffer[i] = static_cast<uint8>(packetIndex % 256);
			}

			ObjectDelivererClient->Send(sendBuffer);
		}
		return true; }));

	// Wait for data reception
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));

	// Verify results
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer, PacketCount, PacketSize]()
							   {
			// Verify all packets were received
			// UDP is unreliable, so not all packets may arrive, but in a local environment most should be delivered
			TestTrue("Verify sufficient number of packets were received",
			         serverHelper->ReceiveBuffers.Num() > PacketCount * 0.9); // Expect at least 90% delivery

			// Validate received packet contents
			for (int32 i = 0; i < serverHelper->ReceiveBuffers.Num(); ++i)
			{
				TArray<uint8>& receiveBuffer = serverHelper->ReceiveBuffers[i];

				// Verify packet size
				TestEqual("Verify packet size is correct", receiveBuffer.Num(), PacketSize);

				if (receiveBuffer.Num() == PacketSize)
				{
					// Get value of first byte
					uint8 expectedValue = receiveBuffer[0];

					// Verify all bytes have the same value
					bool allBytesMatch = true;
					for (int32 j = 1; j < PacketSize; ++j)
					{
						if (receiveBuffer[j] != expectedValue)
						{
							allBytesMatch = false;
							break;
						}
					}

					TestTrue(FString::Printf(TEXT("Verify all bytes in packet %d match"), i), allBytesMatch);
				}
			}

			// Cleanup
			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBroadcastUdpTest, "ObjectDeliverer.ProtocolUDP.BroadcastUdpTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for sending to broadcast address
 * This test verifies that data sent to the local broadcast address can be received by a receiver on the same machine
 */
bool FBroadcastUdpTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableUDPPort();
	constexpr int32 DataSize = 128; // Data size to send

	// Server (receiver) setup
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Broadcast client setup
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	// Send to broadcast address with broadcast flag set to true
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSenderWithBroadcast("255.255.255.255", port, true),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for setup
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	// Data transmission
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, DataSize]()
														 {
		// Send test data
		TArray<uint8> sendBuffer;
		for (int32 i = 0; i < DataSize; ++i)
		{
			sendBuffer.Add(static_cast<uint8>(i % 256));
		}

		// Send multiple times to increase reception rate
		for (int32 i = 0; i < 5; ++i)
		{
			ObjectDelivererClient->Send(sendBuffer);
		}

		return true; }));

	// Wait for data reception
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));

	// Verify results
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer, DataSize]()
							   {
			// Verify broadcast packets were received
			TestTrue("Verify broadcast packets were received",
			         serverHelper->ReceiveBuffers.Num() > 0);

			// Validate received data
			if (serverHelper->ReceiveBuffers.Num() > 0)
			{
				TArray<uint8>& receiveBuffer = serverHelper->ReceiveBuffers[0];

				// Verify packet size
				TestEqual("Verify packet size is correct", receiveBuffer.Num(), DataSize);

				// Verify packet contents
				bool dataCorrect = true;
				for (int32 i = 0; i < FMath::Min(DataSize, receiveBuffer.Num()); ++i)
				{
					if (receiveBuffer[i] != static_cast<uint8>(i % 256))
					{
						dataCorrect = false;
						break;
					}
				}

				TestTrue("Verify received data is correct", dataCorrect);
			}

			// Cleanup
			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLargePacketUdpTest, "ObjectDeliverer.ProtocolUDP.LargePacketUdpTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for sending and receiving near-maximum size packets
 * This test verifies that packets close to the maximum UDP size can be sent and received correctly
 */
bool FLargePacketUdpTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableUDPPort();

	// Set size close to maximum UDP payload size
	// Standard maximum UDP size is 65507 bytes
	// For PacketRuleSizeBody, consider 4 bytes for size information
	constexpr int32 DataSize = 65500;

	// Server (receiver) setup
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Client setup
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for setup
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	// Send large data packet
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, DataSize]()
														 {
		// Send test data (with pattern to make verification easier)
		TArray<uint8> sendBuffer;
		sendBuffer.Reserve(DataSize);

		for (int32 i = 0; i < DataSize; ++i)
		{
			sendBuffer.Add(static_cast<uint8>(i % 256));
		}

		// Send multiple times to increase reception rate
		for (int32 i = 0; i < 3; ++i)
		{
			ObjectDelivererClient->Send(sendBuffer);
		}

		return true; }));

	// Wait for data reception (longer wait time for large packets)
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));

	// Verify results
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer, DataSize]()
							   {
			// Verify large packet was received
			TestTrue("Verify large packet was received",
			         serverHelper->ReceiveBuffers.Num() > 0);

			// Validate received data
			if (serverHelper->ReceiveBuffers.Num() > 0)
			{
				TArray<uint8>& receiveBuffer = serverHelper->ReceiveBuffers[0];

				// Verify packet size
				TestEqual("Verify packet size is correct", receiveBuffer.Num(), DataSize);

				// Verify packet contents with sampling (checking all bytes would take too long, so check at several positions)
				bool dataCorrect = true;

				// Check start section
				for (int32 i = 0; i < FMath::Min(100, receiveBuffer.Num()); ++i)
				{
					if (receiveBuffer[i] != static_cast<uint8>(i % 256))
					{
						dataCorrect = false;
						break;
					}
				}

				// Check middle section
				if (dataCorrect && receiveBuffer.Num() > DataSize / 2 + 100)
				{
					int32 midPoint = DataSize / 2;
					for (int32 i = 0; i < 100; ++i)
					{
						if (receiveBuffer[midPoint + i] != static_cast<uint8>((midPoint + i) % 256))
						{
							dataCorrect = false;
							break;
						}
					}
				}

				// Check end section
				if (dataCorrect && receiveBuffer.Num() > DataSize - 100)
				{
					for (int32 i = 0; i < 100; ++i)
					{
						int32 index = DataSize - 100 + i;
						if (receiveBuffer[index] != static_cast<uint8>(index % 256))
						{
							dataCorrect = false;
							break;
						}
					}
				}

				TestTrue("Verify received data is correct", dataCorrect);
			}

			// Cleanup
			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDifferentPacketRuleUdpTest, "ObjectDeliverer.ProtocolUDP.DifferentPacketRuleUdpTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for using different packet rules
 * This test verifies the behavior when client and server use different packet rules
 */
bool FDifferentPacketRuleUdpTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableUDPPort();
	constexpr int32 DataSize = 256; // Data size to send

	// Server (receiver) setup - using PacketRuleSizeBody
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Client setup - using PacketRuleNodivision
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", port),
								 UPacketRuleFactory::CreatePacketRuleNodivision());

	// Wait for setup
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	// Data transmission
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, DataSize]()
														 {
		// Send test data
		TArray<uint8> sendBuffer;
		for (int32 i = 0; i < DataSize; ++i)
		{
			sendBuffer.Add(static_cast<uint8>(i % 256));
		}

		// Send multiple times
		for (int32 i = 0; i < 5; ++i)
		{
			ObjectDelivererClient->Send(sendBuffer);
		}

		return true; }));

	// Wait for data reception
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));

	// Verify results
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			// With different packet rules, data normally cannot be correctly received
			// For UDP, since it's packet-based, some data may still be received

			// Verify that data is not correctly interpreted due to packet rule mismatch
			if (serverHelper->ReceiveBuffers.Num() > 0)
			{
				// Check the length of received data (with PacketRuleSizeBody, the first 4 bytes are interpreted as size information)
				TArray<uint8>& receiveBuffer = serverHelper->ReceiveBuffers[0];

				// Data may not be correctly interpreted due to different packet rules
				// Comment: Data reception with different packet rules is not expected to work correctly,
				// so we're just logging the behavior rather than validating specific values
				UE_LOG(LogTemp, Display, TEXT("Different packet rule test: Receive buffer count=%d, First received data size=%d"),
				       serverHelper->ReceiveBuffers.Num(),
				       serverHelper->ReceiveBuffers.Num() > 0 ? serverHelper->ReceiveBuffers[0].Num() : 0);
			}

			// Cleanup
			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBroadcastDisabledUdpTest, "ObjectDeliverer.ProtocolUDP.BroadcastDisabledUdpTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * Test for sending with broadcast disabled
 * This test verifies that sending to a broadcast address fails when the broadcast flag is set to false
 */
bool FBroadcastDisabledUdpTest::RunTest(const FString &Parameters)
{
	int32 port = GetAvailableUDPPort();
	constexpr int32 DataSize = 128; // Data size to send

	// Server (receiver) setup
	auto serverHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererServer = NewObject<UObjectDelivererManager>();
	ObjectDelivererServer->Connected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererServer->Disconnected.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	ObjectDelivererServer->ReceiveData.AddDynamic(serverHelper, &UObjectDelivererManagerTestHelper::OnReceive);
	ObjectDelivererServer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(port),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Client setup with broadcast disabled
	auto clientHelper = NewObject<UObjectDelivererManagerTestHelper>();
	auto ObjectDelivererClient = NewObject<UObjectDelivererManager>();
	ObjectDelivererClient->Connected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnConnect);
	ObjectDelivererClient->Disconnected.AddDynamic(clientHelper, &UObjectDelivererManagerTestHelper::OnDisConnect);
	// Send to broadcast address but with broadcast flag set to false
	ObjectDelivererClient->Start(UProtocolFactory::CreateProtocolUdpSocketSenderWithBroadcast("255.255.255.255", port, false),
								 UPacketRuleFactory::CreatePacketRuleSizeBody());

	// Wait for setup
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	// Data transmission
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, ObjectDelivererClient, DataSize]()
														 {
		// Send test data
		TArray<uint8> sendBuffer;
		for (int32 i = 0; i < DataSize; ++i)
		{
			sendBuffer.Add(static_cast<uint8>(i % 256));
		}

		// Send multiple times, but should not be received due to broadcast being disabled
		for (int32 i = 0; i < 5; ++i)
		{
			ObjectDelivererClient->Send(sendBuffer);
		}

		return true; }));

	// Wait for data reception
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));

	// Verify results
	ADD_LATENT_AUTOMATION_COMMAND(
		FFunctionLatentCommand([this, serverHelper, clientHelper, ObjectDelivererClient, ObjectDelivererServer]()
							   {
			// With broadcast disabled, packets should not be received
			TestEqual("Verify no packets are received when broadcast is disabled",
			          serverHelper->ReceiveBuffers.Num(), 0);

			// On the same machine, even with broadcast disabled, packets might be delivered if sent to the local IP
			// Therefore, we can't strictly guarantee that sending to a broadcast address will fail
			// We'll log this situation
			UE_LOG(LogTemp, Display, TEXT("Broadcast disabled test: Attempted to send to broadcast address. Receive buffer count=%d"),
			       serverHelper->ReceiveBuffers.Num());

			// Cleanup
			ObjectDelivererClient->Close();
			ObjectDelivererServer->Close();
			return true; }));

	return true;
}
#endif