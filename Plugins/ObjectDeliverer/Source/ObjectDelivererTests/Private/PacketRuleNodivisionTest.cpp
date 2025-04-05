// Copyright 2019 ayumax. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "PacketRule/PacketRuleFactory.h"
#include "PacketRule/PacketRuleNodivision.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPacketRuleNodivisionTest_MakeSendPacket, "ObjectDeliverer.PacketRuleTests.PacketRuleNodivisionTest.MakeSendPacket", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FPacketRuleNodivisionTest_MakeSendPacket::RunTest(const FString& Parameters)
{
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleNodivision();

		packetRule->MadeSendBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), 8);

			for (int i = 0; i < 8; ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPacketRuleNodivisionTest_NotifyReceiveData, "ObjectDeliverer.PacketRuleTests.PacketRuleNodivisionTest.NotifyReceiveData", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FPacketRuleNodivisionTest_NotifyReceiveData::RunTest(const FString& Parameters)
{
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleNodivision();

		bool isReceived = false;

		packetRule->MadeReceiveBuffer.BindLambda([this, &isReceived](const TArray<uint8>& Buffer)
		{
			isReceived = true;

			TestEqual(TEXT("NotifyReceiveData check size"), Buffer.Num(), 8);

			for (int i = 0; i < Buffer.Num(); ++i)
			{
				TestEqual(TEXT("NotifyReceiveData check buffer value"), Buffer[i], i);
			}

		});

		packetRule->Initialize();

		TArray<uint8> buffer;
		buffer.SetNum(8);
		for (int i = 0; i < 8; ++i)
		{
			buffer[i] = i;
		}

		packetRule->NotifyReceiveData(buffer);

		TestEqual(TEXT("NotifyReceiveData check receuved"), isReceived, true);
	}


	return true;
}