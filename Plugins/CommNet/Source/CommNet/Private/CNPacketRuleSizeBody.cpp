#include "CNPacketRuleSizeBody.h"
#include "CommNetProtocol.h"
#include "CNPacketRuleFactory.h"

UCNPacketRuleSizeBody::UCNPacketRuleSizeBody()
{
	
}

UCNPacketRuleSizeBody::~UCNPacketRuleSizeBody()
{
}

void UCNPacketRuleSizeBody::Initialize_Implementation()
{
	BufferForSend.SetNum(1024);
	ReceiveMode = EReceiveMode::Size;
	BodySize = 0;
}

void UCNPacketRuleSizeBody::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer)
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

void UCNPacketRuleSizeBody::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer)
{
	if (ReceiveMode == EReceiveMode::Size)
	{
		OnReceivedSize(DataBuffer);
		return;
	}
	
	OnReceivedBody(DataBuffer);
}

void UCNPacketRuleSizeBody::OnReceivedSize(const TArray<uint8>& DataBuffer)
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

void UCNPacketRuleSizeBody::OnReceivedBody(const TArray<uint8>& DataBuffer)
{
	DispatchMadeReceiveBuffer(DataBuffer);

	BodySize = 0;

	ReceiveMode = EReceiveMode::Size;
}

int32 UCNPacketRuleSizeBody::GetWantSize_Implementation()
{
	if (ReceiveMode == EReceiveMode::Size)
	{
		return SizeLength;
	}

	return BodySize;
}

UCNPacketRule* UCNPacketRuleSizeBody::Clone_Implementation()
{
	return UCNPacketRuleFactory::CreateCNPacketRuleSizeBody(SizeLength, SizeBufferEndian);
}