// Copyright 2019 ayumax. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "../Utils/ODGrowBuffer.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ODGrowBuffer_Tests, "ObjectDeliverer.GrowBuffer.Test", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool ODGrowBuffer_Tests::RunTest(const FString& Parameters)
{
	const int packetSize = 1024;

	{
		auto buffer = ODGrowBuffer(100);

		TestEqual(TEXT("check buffer size"), buffer.GetLength(), 100);
		TestEqual(TEXT("check inner buffer size"), buffer.GetInnerBufferSize(), packetSize);

		buffer.SetLength(2000);

		TestEqual(TEXT("check buffer size"), buffer.GetLength(), 2000);
		TestEqual(TEXT("check inner buffer size"), buffer.GetInnerBufferSize(), packetSize * 2);

		uint8 testDataARray[] = { 1, 2, 3 };
		buffer.Add(ODByteSpan(testDataARray, sizeof(testDataARray)));

		TestEqual(TEXT("check buffer size"), buffer.GetLength(), 2003);
		TestEqual(TEXT("check inner buffer size"), buffer.GetInnerBufferSize(), packetSize * 2);
		TestEqual(TEXT("check Add Data1"), buffer[2000], 1);
		TestEqual(TEXT("check Add Data2"), buffer[2001], 2);
		TestEqual(TEXT("check Add Data3"), buffer[2002], 3);

		buffer.RemoveRangeFromStart(0, 2000);
		TestEqual(TEXT("check buffer size"), buffer.GetLength(), 3);
		TestEqual(TEXT("check inner buffer size"), buffer.GetInnerBufferSize(), packetSize * 2);
		TestEqual(TEXT("check Add Data1"), buffer[0], 1);
		TestEqual(TEXT("check Add Data2"), buffer[1], 2);
		TestEqual(TEXT("check Add Data3"), buffer[2], 3);

		uint8 testDataARray2[] = { 0xEE, 0xEF };
		buffer.CopyFrom(ODByteSpan(testDataARray2, sizeof(testDataARray2)), 1);
		TestEqual(TEXT("check buffer size"), buffer.GetLength(), 3);
		TestEqual(TEXT("check inner buffer size"), buffer.GetInnerBufferSize(), packetSize * 2);
		TestEqual(TEXT("check Add Data1"), buffer[0], 1);
		TestEqual(TEXT("check Add Data2"), buffer[1], 0xEE);
		TestEqual(TEXT("check Add Data3"), buffer[2], 0xEF);

		buffer.Clear();
		TestEqual(TEXT("check buffer size"), buffer.GetLength(), 0);
		TestEqual(TEXT("check inner buffer size"), buffer.GetInnerBufferSize(), packetSize * 2);
	}

	return true;
}

