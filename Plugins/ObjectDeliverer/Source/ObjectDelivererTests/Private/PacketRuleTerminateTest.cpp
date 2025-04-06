// Copyright 2019 ayumax. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "PacketRule/PacketRuleFactory.h"
#include "PacketRule/PacketRuleTerminate.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPacketRuleTerminateTest_MakeSendPacket, "ObjectDeliverer.PacketRuleTests.PacketRuleTerminateTest.MakeSendPacket", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FPacketRuleTerminateTest_MakeSendPacket::RunTest(const FString& Parameters)
{
	// terminater : "\r\n"
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleTerminate({ TEXT('\r'), TEXT('\n') });

		packetRule->MadeSendBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), 10);

			for (int i = 0; i < 8; ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
			}

			TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[8], TEXT('\r'));
			TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[9], TEXT('\n'));

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

	// terminater : 0xff
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleTerminate({ 0xFF });

		packetRule->MadeSendBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), 9);

			for (int i = 0; i < 8; ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
			}

			TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[8], 0xFF);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPacketRuleTerminateTest_NotifyReceiveData, "ObjectDeliverer.PacketRuleTests.PacketRuleTerminateTest.NotifyReceiveData", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FPacketRuleTerminateTest_NotifyReceiveData::RunTest(const FString& Parameters)
{
	// packet num is 1
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleTerminate({ TEXT('\r'), TEXT('\n') });

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
		buffer.SetNum(10);
		for (int i = 0; i < 8; ++i)
		{
			buffer[i] = i;
		}

		buffer[8] = TEXT('\r');
		buffer[9] = TEXT('\n');

		packetRule->NotifyReceiveData(buffer);

		TestEqual(TEXT("NotifyReceiveData check receuved"), isReceived, true);
	}

	// packet num is 2
	{
		auto packetRule = UPacketRuleFactory::CreatePacketRuleTerminate({ TEXT('\r'), TEXT('\n') });
		
		int32 receivedCounter = 0;

		packetRule->MadeReceiveBuffer.BindLambda([this, &receivedCounter](const TArray<uint8>& Buffer)
		{
			receivedCounter++;

			TestEqual(TEXT("NotifyReceiveData check size"), Buffer.Num(), 8);

			for (int i = 0; i < Buffer.Num(); ++i)
			{
				TestEqual(TEXT("NotifyReceiveData check buffer value"), Buffer[i], i);
			}

		});

		packetRule->Initialize();

		TArray<uint8> buffer;
		buffer.SetNum(20);
		for (int i = 0; i < 8; ++i)
		{
			buffer[i] = i;
		}

		buffer[8] = TEXT('\r');
		buffer[9] = TEXT('\n');

		for (int i = 10; i < 18; ++i)
		{
			buffer[i] = i - 10;
		}

		buffer[18] = TEXT('\r');
		buffer[19] = TEXT('\n');

		packetRule->NotifyReceiveData(buffer);

		TestEqual(TEXT("NotifyReceiveData check received counter"), receivedCounter, 2);
	}

	// When the packet is split
	 {
		auto packetRule = UPacketRuleFactory::CreatePacketRuleTerminate({ TEXT('\r'), TEXT('\n') });
	
		int32 receivedCounter = 0;
	
		packetRule->MadeReceiveBuffer.BindLambda([this, &receivedCounter](const TArray<uint8>& Buffer)
		{
			receivedCounter++;
	
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

		TestEqual(TEXT("NotifyReceiveData check received counter"), receivedCounter, 0);

		buffer.SetNum(12);

		buffer[0] = TEXT('\r');
		buffer[1] = TEXT('\n');
	
		for (int i = 2; i < 10; ++i)
		{
			buffer[i] = i - 2;
		}
	
		buffer[10] = TEXT('\r');
		buffer[11] = TEXT('\n');
	
		packetRule->NotifyReceiveData(buffer);
	
		TestEqual(TEXT("NotifyReceiveData check received counter"), receivedCounter, 2);
	 }

	 // terminater : 0xFF
	 {
		 auto packetRule = UPacketRuleFactory::CreatePacketRuleTerminate({ 0xFF });

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
		 buffer.SetNum(9);
		 for (int i = 0; i < 8; ++i)
		 {
			 buffer[i] = i;
		 }

		 buffer[8] = 0xFF;

		 packetRule->NotifyReceiveData(buffer);

		 TestEqual(TEXT("NotifyReceiveData check receuved"), isReceived, true);
	 }

	return true;
}