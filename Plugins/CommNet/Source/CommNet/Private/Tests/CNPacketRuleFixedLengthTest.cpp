#include "AutomationTest.h"
#include "CNPacketRuleFixedLength.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNPacketRuleFixedLengthTest_MakeSendPacket, "CommNet.PacketRuleTests.CNPacketRuleFixedLengthTest.MakeSendPacket", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FCNPacketRuleFixedLengthTest_MakeSendPacket::RunTest(const FString& Parameters)
{
	// just size
	{
		auto packetRule = NewObject<UCNPacketRuleFixedLength>();
		packetRule->MadeSendBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), 8);

			for (int i = 0; i < Buffer.Num(); ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
			}

		});
		packetRule->FixedSize = 8;
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
		auto packetRule = NewObject<UCNPacketRuleFixedLength>();
		packetRule->MadeSendBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), 8);

			for (int i = 0; i < Buffer.Num(); ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
			}

		});
		packetRule->FixedSize = 8;
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
		auto packetRule = NewObject<UCNPacketRuleFixedLength>();
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
		packetRule->FixedSize = 8;
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNPacketRuleFixedLengthTest_NotifyReceiveData, "CommNet.PacketRuleTests.CNPacketRuleFixedLengthTest.NotifyReceiveData", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FCNPacketRuleFixedLengthTest_NotifyReceiveData::RunTest(const FString& Parameters)
{
	// 
	{
		auto packetRule = NewObject<UCNPacketRuleFixedLength>();
		packetRule->MadeReceiveBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("NotifyReceiveData check size"), Buffer.Num(), 8);

			for (int i = 0; i < Buffer.Num(); ++i)
			{
				TestEqual(TEXT("NotifyReceiveData check buffer value"), Buffer[i], i);
			}

		});
		packetRule->FixedSize = 8;
		packetRule->Initialize();

		TArray<uint8> buffer;
		buffer.SetNum(8);
		for (int i = 0; i < buffer.Num(); ++i)
		{
			buffer[i] = i;
		}

		packetRule->NotifyReceiveData(buffer);
	}


	return true;
}