// Copyright 2019 ayumax. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "PacketRule/PacketRuleFactory.h"
#include "PacketRule/PacketRuleFixedLength.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNPacketRuleFixedLengthTest_MakeSendPacket, "ObjectDeliverer.PacketRuleTests.CNPacketRuleFixedLengthTest.MakeSendPacket", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FCNPacketRuleFixedLengthTest_MakeSendPacket::RunTest(const FString& Parameters)
{
	// just size
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleFixedLength(8);

		packetRule->MadeSendBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), 8);

			for (int i = 0; i < Buffer.Num(); ++i)
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

	// over size
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleFixedLength(8);

		packetRule->MadeSendBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), 8);

			for (int i = 0; i < Buffer.Num(); ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
			}

		});

		packetRule->Initialize();

		TArray<uint8> buffer;
		buffer.SetNum(10);
		for (int i = 0; i < buffer.Num(); ++i)
		{
			buffer[i] = i;
		}

		packetRule->MakeSendPacket(buffer);
	}
	
	// under size
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleFixedLength(8);

		packetRule->MadeSendBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), 8);

			for (int i = 0; i < 6; ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
			}

			for (int i = 6; i < 8; ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], 0);
			}

		});

		packetRule->Initialize();

		TArray<uint8> buffer;
		buffer.SetNum(6);
		for (int i = 0; i < buffer.Num(); ++i)
		{
			buffer[i] = i;
		}

		packetRule->MakeSendPacket(buffer);
	}


	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNPacketRuleFixedLengthTest_NotifyReceiveData, "ObjectDeliverer.PacketRuleTests.CNPacketRuleFixedLengthTest.NotifyReceiveData", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FCNPacketRuleFixedLengthTest_NotifyReceiveData::RunTest(const FString& Parameters)
{
	// 
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleFixedLength(8);

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
		for (int i = 0; i < buffer.Num(); ++i)
		{
			buffer[i] = i;
		}

		packetRule->NotifyReceiveData(buffer);

		TestEqual(TEXT("NotifyReceiveData check receuved"), isReceived, true);
	}


	return true;
}