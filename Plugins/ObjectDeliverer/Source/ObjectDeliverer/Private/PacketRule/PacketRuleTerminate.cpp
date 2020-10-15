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
	BufferForSend.SetNum(BodyBuffer.Num() + Terminate.Num(), false);

	FMemory::Memcpy(BufferForSend.GetData(), BodyBuffer.GetData(), BodyBuffer.Num());
	FMemory::Memcpy(BufferForSend.GetData() + BodyBuffer.Num(), Terminate.GetData(), Terminate.Num());

	DispatchMadeSendBuffer(BufferForSend);
}

void UPacketRuleTerminate::NotifyReceiveData(const TArray<uint8>& DataBuffer)
{
	ReceiveTempBuffer += DataBuffer;

	int32 findIndex = -1;

	while (true)
	{
		for (int i = 0; i <= ReceiveTempBuffer.Num() - Terminate.Num(); ++i)
		{
			bool notEqual = false;
			for (int j = 0; j < Terminate.Num(); ++j)
			{
				if (ReceiveTempBuffer[i + j] != Terminate[j])
				{
					notEqual = true;
					break;
				}
			}

			if (notEqual == false)
			{
				findIndex = i;
				break;
			}
		}

		if (findIndex == -1)
		{
			return;
		}

		BufferForReceive.SetNum(findIndex, false);
		FMemory::Memcpy(BufferForReceive.GetData(), ReceiveTempBuffer.GetData(), findIndex);
		DispatchMadeReceiveBuffer(BufferForReceive);

		ReceiveTempBuffer.RemoveAt(0, findIndex + Terminate.Num());

		findIndex = -1;
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