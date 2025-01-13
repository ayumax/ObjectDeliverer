// Copyright 2019 ayumax. All Rights Reserved.
#include "PacketRule/PacketRuleTerminate.h"
#include "PacketRule/PacketRuleFactory.h"

UPacketRuleTerminate::UPacketRuleTerminate()
{
	Terminate.Add(TEXT('\r'));
	Terminate.Add(TEXT('\n'));
}

UPacketRuleTerminate::~UPacketRuleTerminate()
{
}

void UPacketRuleTerminate::Initialize()
{
	BufferForSend.Reset(1024);
	ReceiveTempBuffer.Reset(1024);
	BufferForReceive.Reset(1024);
}

void UPacketRuleTerminate::MakeSendPacket(const TArray<uint8>& BodyBuffer)
{
	BufferForSend.SetNum(BodyBuffer.Num() + Terminate.Num(), EAllowShrinking::No);
	FMemory::Memcpy(BufferForSend.GetData(), BodyBuffer.GetData(), BodyBuffer.Num());
	FMemory::Memcpy(BufferForSend.GetData() + BodyBuffer.Num(), Terminate.GetData(), Terminate.Num());
	DispatchMadeSendBuffer(BufferForSend);
}

void UPacketRuleTerminate::NotifyReceiveData(const TArray<uint8>& DataBuffer)
{
	ReceiveTempBuffer += DataBuffer;
	const auto TerminateCount{ Terminate.Num() };

	while (ReceiveTempBuffer.Num() >= TerminateCount)
	{
		int32 findIndex{ INDEX_NONE };

		for (int32 i{ 0 }, count{ ReceiveTempBuffer.Num() - TerminateCount }; i <= count; i++)
		{
			auto bTerminate{ true };

			for (int32 j{ 0 }; j < TerminateCount; j++)
			{
				if (ReceiveTempBuffer[i + j] == Terminate[j])
					continue;

				bTerminate = false;
				break;
			}

			if (!bTerminate)
				continue;

			findIndex = MoveTemp(i);
			break;
		}

		if (findIndex <= INDEX_NONE)
			return;

		BufferForReceive.SetNum(findIndex, EAllowShrinking::No);
		FMemory::Memcpy(BufferForReceive.GetData(), ReceiveTempBuffer.GetData(), findIndex);
		DispatchMadeReceiveBuffer(BufferForReceive);
		ReceiveTempBuffer.RemoveAt(0, findIndex + Terminate.Num());
	}
}

int32 UPacketRuleTerminate::GetWantSize()
{
	return 0;
}

UPacketRule* UPacketRuleTerminate::Clone()
{
	return UPacketRuleFactory::CreatePacketRuleTerminate(Terminate);
}