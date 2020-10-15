// Copyright 2019 ayumax. All Rights Reserved.
#include "PacketRule/PacketRuleSizeBody.h"
#include "PacketRule/PacketRuleFactory.h"

UPacketRuleSizeBody::UPacketRuleSizeBody()
{
	
}

UPacketRuleSizeBody::~UPacketRuleSizeBody()
{
}

void UPacketRuleSizeBody::Initialize()
{
	BufferForSend.SetNum(1024);
	ReceiveMode = EReceiveMode::Size;
	BodySize = 0;
}

void UPacketRuleSizeBody::MakeSendPacket(const TArray<uint8>& BodyBuffer)
{
	auto BodyBufferNum = BodyBuffer.Num();
	auto SendSize = BodyBufferNum + SizeLength;
	BufferForSend.SetNum(SendSize, false);

	for (int i = 0; i < SizeLength; ++i)
	{
		int32 offset = 0;
		if (SizeBufferEndian == ECNBufferEndian::Big)
		{
			offset = 8 * (SizeLength - i - 1);
		}
		else
		{
			offset = 8 * i;
		}

		BufferForSend[i] = (uint8)((BodyBufferNum >> offset) & 0xFF);
	}

	FMemory::Memcpy(BufferForSend.GetData() + SizeLength, BodyBuffer.GetData(), BodyBufferNum);

	DispatchMadeSendBuffer(BufferForSend);

}

void UPacketRuleSizeBody::NotifyReceiveData(const TArray<uint8>& DataBuffer)
{
	if (ReceiveMode == EReceiveMode::Size)
	{
		OnReceivedSize(DataBuffer);
		return;
	}
	
	OnReceivedBody(DataBuffer);
}

void UPacketRuleSizeBody::OnReceivedSize(const TArray<uint8>& DataBuffer)
{
	BodySize = 0;
	for (int i = 0; i < SizeLength; ++i)
	{
		int32 offset = 0;
		if (SizeBufferEndian == ECNBufferEndian::Big)
		{
			offset = 8 * (SizeLength - i - 1);
		}
		else
		{
			offset = 8 * i;
		}
		BodySize |= (DataBuffer[i] << offset);
	}

	ReceiveMode = EReceiveMode::Body;
}

void UPacketRuleSizeBody::OnReceivedBody(const TArray<uint8>& DataBuffer)
{
	DispatchMadeReceiveBuffer(DataBuffer);

	BodySize = 0;

	ReceiveMode = EReceiveMode::Size;
}

int32 UPacketRuleSizeBody::GetWantSize()
{
	if (ReceiveMode == EReceiveMode::Size)
	{
		return SizeLength;
	}

	return BodySize;
}

UPacketRule* UPacketRuleSizeBody::Clone()
{
	return UPacketRuleFactory::CreatePacketRuleSizeBody(SizeLength, SizeBufferEndian);
}