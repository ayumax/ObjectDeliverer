#include "AutomationTest.h"
#include "CNPacketRuleTerminate.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNPacketRuleTerminateTest_MakeSendPacket, "CommNet.PacketRuleTests.CNPacketRuleTerminateTest.MakeSendPacket", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FCNPacketRuleTerminateTest_MakeSendPacket::RunTest(const FString& Parameters)
{
	// just size
	//{
	//	auto packetRule = NewObject<UCNPacketRuleTerminate>();
	//	packetRule->MadeSendBuffer.BindLambda([this](const TArray<uint8>& Buffer)
	//	{
	//		TestEqual(TEXT("MakeSendPacket check size"), Buffer.Num(), 8);

	//		for (int i = 0; i < Buffer.Num(); ++i)
	//		{
	//			TestEqual(TEXT("MakeSendPacket check buffer value"), Buffer[i], i);
	//		}

	//	});
	//	//packetRule->FixedSize = 8;
	//	packetRule->Initialize();

	//	TArray<uint8> buffer;
	//	buffer.SetNum(8);
	//	for (int i = 0; i < buffer.Num(); ++i)
	//	{
	//		buffer[i] = i;
	//	}

	//	packetRule->MakeSendPacket(buffer);
	//}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCNPacketRuleTerminateTest_NotifyReceiveData, "CommNet.PacketRuleTests.CNPacketRuleTerminateTest.NotifyReceiveData", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FCNPacketRuleTerminateTest_NotifyReceiveData::RunTest(const FString& Parameters)
{
	// 
	//{
	//	auto packetRule = NewObject<UCNPacketRuleTerminate>();
	//	packetRule->MadeReceiveBuffer.BindLambda([this](const TArray<uint8>& Buffer)
	//	{
	//		TestEqual(TEXT("NotifyReceiveData check size"), Buffer.Num(), 8);

	//		for (int i = 0; i < Buffer.Num(); ++i)
	//		{
	//			TestEqual(TEXT("NotifyReceiveData check buffer value"), Buffer[i], i);
	//		}

	//	});
	//	//packetRule->FixedSize = 8;
	//	packetRule->Initialize();

	//	TArray<uint8> buffer;
	//	buffer.SetNum(8);
	//	for (int i = 0; i < buffer.Num(); ++i)
	//	{
	//		buffer[i] = i;
	//	}

	//	packetRule->NotifyReceiveData(buffer);
	//}


	return true;
}