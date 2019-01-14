#include "AutomationTest.h"
#include "CNPacketRuleSizeBody.h"
#include "CNPacketRuleFactory.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNPacketRuleSizeBodyTest_MakeSendPacket, "CommNet.PacketRuleTests.FCNPacketRuleSizeBodyTest.MakeSendPacket", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FCNPacketRuleSizeBodyTest_MakeSendPacket::RunTest(const FString& Parameters)
{
	// size buffer endian is big
	{
		auto packetRule = UCNPacketRuleFactory::CreateCNPacketRuleSizeBody(4, ECNBufferEndian::Big);
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
		auto packetRule = UCNPacketRuleFactory::CreateCNPacketRuleSizeBody(4, ECNBufferEndian::Little);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNPacketRuleSizeBodyTest_NotifyReceiveData, "CommNet.PacketRuleTests.FCNPacketRuleSizeBodyTest.NotifyReceiveData", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FCNPacketRuleSizeBodyTest_NotifyReceiveData::RunTest(const FString& Parameters)
{
	{
		auto packetRule = UCNPacketRuleFactory::CreateCNPacketRuleSizeBody(4, ECNBufferEndian::Big);

		packetRule->Initialize();

		TArray<uint8> buffer;
		buffer.SetNum(4);
		buffer[0] = 0;
		buffer[1] = 0;
		buffer[2] = 0;
		buffer[3] = 8;

		packetRule->MadeReceiveBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("NotifyReceiveData not call"), 1, 0);
		});

		packetRule->NotifyReceiveData(buffer);

		buffer.SetNum(8);
		for (int i = 0; i < buffer.Num(); ++i)
		{
			buffer[i] = i;
		}

		packetRule->MadeReceiveBuffer.BindLambda([this](const TArray<uint8>& Buffer)
		{
			TestEqual(TEXT("NotifyReceiveData check size"), Buffer.Num(), 8);

			for (int i = 0; i < 8; ++i)
			{
				TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
			}
		});

		packetRule->NotifyReceiveData(buffer);
	}



	return true;
}