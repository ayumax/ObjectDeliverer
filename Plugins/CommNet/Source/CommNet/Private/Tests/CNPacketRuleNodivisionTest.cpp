#include "AutomationTest.h"
#include "CNPacketRuleNodivision.h"
#include "CNPacketRuleFactory.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNPacketRuleNodivisionTest_MakeSendPacket, "CommNet.PacketRuleTests.CNPacketRuleNodivisionTest.MakeSendPacket", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FCNPacketRuleNodivisionTest_MakeSendPacket::RunTest(const FString& Parameters)
{
	{
		auto packetRule = UCNPacketRuleFactory::CreateCNPacketRuleNodivision();

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNPacketRuleNodivisionTest_NotifyReceiveData, "CommNet.PacketRuleTests.CNPacketRuleNodivisionTest.NotifyReceiveData", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FCNPacketRuleNodivisionTest_NotifyReceiveData::RunTest(const FString& Parameters)
{
	{
		auto packetRule = UCNPacketRuleFactory::CreateCNPacketRuleNodivision();

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