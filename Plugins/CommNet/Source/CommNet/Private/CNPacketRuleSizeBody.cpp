#include "CNPacketRuleSizeBody.h"
#include "CommNetProtocol.h"

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

void UCNPacketRuleSizeBody::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer, TArray<uint8>& SendBuffer)
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

	SendBuffer = BufferForSend;

}

bool UCNPacketRuleSizeBody::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer)
{
	if (ReceiveMode == EReceiveMode::Size)
	{
		OnReceivedSize(DataBuffer);
		return false;
	}
	
	OnReceivedBody(DataBuffer, BodyBuffer);
	return true;
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

void UCNPacketRuleSizeBody::OnReceivedBody(const TArray<uint8>& DataBuffer, TArray<uint8>& BodyBuffer)
{
	BodyBuffer = DataBuffer;
	BodySize = GetWantSize_Implementation();

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

