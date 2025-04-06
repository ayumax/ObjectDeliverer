// Copyright 2019 ayumax. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "PacketRule/PacketRuleFactory.h"
#include "PacketRule/PacketRuleSizeBody.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPacketRuleSizeBodyTest_MakeSendPacket, "ObjectDeliverer.PacketRuleTests.FPacketRuleSizeBodyTest.MakeSendPacket", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FPacketRuleSizeBodyTest_MakeSendPacket::RunTest(const FString& Parameters)
{
	// size buffer endian is big
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleSizeBody(4, ECNBufferEndian::Big);
		packetRule->MadeSendBuffer.BindLambda([this, &packetRule](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), packetRule->SizeLength + 8);

			TestEqual(TEXT("MakeSendPacket check sizebuffer"), Buffer[0], 0);
			TestEqual(TEXT("MakeSendPacket check sizebuffer"), Buffer[1], 0);
			TestEqual(TEXT("MakeSendPacket check sizebuffer"), Buffer[2], 0);
			TestEqual(TEXT("MakeSendPacket check sizebuffer"), Buffer[3], 8);

			for (int i = packetRule->SizeLength; i < Buffer.Num(); ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i - packetRule->SizeLength);
			}

		});

		packetRule->Initialize();

		TArray<uint8> buffer;
		buffer.SetNum(8);
		for (int i = 0; i < buffer.Num(); ++i)
		{
			buffer[i] = i;
		}

		packetRule->MakeSendPacket(buffer);
	}

	// size buffer endian is little
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleSizeBody(4, ECNBufferEndian::Little);

		packetRule->MadeSendBuffer.BindLambda([this, &packetRule](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), packetRule->SizeLength + 8);

			TestEqual(TEXT("MakeSendPacket check sizebuffer"), Buffer[0], 8);
			TestEqual(TEXT("MakeSendPacket check sizebuffer"), Buffer[1], 0);
			TestEqual(TEXT("MakeSendPacket check sizebuffer"), Buffer[2], 0);
			TestEqual(TEXT("MakeSendPacket check sizebuffer"), Buffer[3], 0);

			for (int i = packetRule->SizeLength; i < Buffer.Num(); ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i - packetRule->SizeLength);
			}

		});

		packetRule->Initialize();

		TArray<uint8> buffer;
		buffer.SetNum(8);
		for (int i = 0; i < buffer.Num(); ++i)
		{
			buffer[i] = i;
		}

		packetRule->MakeSendPacket(buffer);
	}


	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPacketRuleSizeBodyTest_NotifyReceiveData, "ObjectDeliverer.PacketRuleTests.FPacketRuleSizeBodyTest.NotifyReceiveData", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FPacketRuleSizeBodyTest_NotifyReceiveData::RunTest(const FString& Parameters)
{
	// size buffer is big endian
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleSizeBody(4, ECNBufferEndian::Big);

		packetRule->Initialize();

		TArray<uint8> buffer;
		buffer.SetNum(4);
		buffer[0] = 0;
		buffer[1] = 0;
		buffer[2] = 0;
		buffer[3] = 8;

		bool notCall = true;

		packetRule->MadeReceiveBuffer.BindLambda([this, &notCall](const TArray<uint8>& Buffer)
		{
			// must not be called here
			notCall = false;
		});

		packetRule->NotifyReceiveData(buffer);
		
		TestEqual(TEXT("NotifyReceiveData not call"), notCall, true);

		buffer.SetNum(8);
		for (int i = 0; i < buffer.Num(); ++i)
		{
			buffer[i] = i;
		}

		bool isReceived = false;

		packetRule->MadeReceiveBuffer.BindLambda([this, &isReceived](const TArray<uint8>& Buffer)
		{
			isReceived = true;

			TestEqual(TEXT("NotifyReceiveData check size"), Buffer.Num(), 8);

			for (int i = 0; i < 8; ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
			}
		});

		packetRule->NotifyReceiveData(buffer);

		TestEqual(TEXT("NotifyReceiveData check receuved"), isReceived, true);
	}

	// size buffer is little endian
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleSizeBody(4, ECNBufferEndian::Little);

		packetRule->Initialize();

		TArray<uint8> buffer;
		buffer.SetNum(4);
		buffer[0] = 8;
		buffer[1] = 0;
		buffer[2] = 0;
		buffer[3] = 0;

		bool notCall = true;

		packetRule->MadeReceiveBuffer.BindLambda([this, &notCall](const TArray<uint8>& Buffer)
		{
			// must not be called here
			notCall = false;
		});


		packetRule->NotifyReceiveData(buffer);

		TestEqual(TEXT("NotifyReceiveData not call"), notCall, true);

		buffer.SetNum(8);
		for (int i = 0; i < buffer.Num(); ++i)
		{
			buffer[i] = i;
		}

		bool isReceived = false;

		packetRule->MadeReceiveBuffer.BindLambda([this, &isReceived](const TArray<uint8>& Buffer)
		{
			isReceived = true;

			TestEqual(TEXT("NotifyReceiveData check size"), Buffer.Num(), 8);

			for (int i = 0; i < 8; ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
			}
		});

		packetRule->NotifyReceiveData(buffer);

		TestEqual(TEXT("NotifyReceiveData check receuved"), isReceived, true);
	}



	return true;
}