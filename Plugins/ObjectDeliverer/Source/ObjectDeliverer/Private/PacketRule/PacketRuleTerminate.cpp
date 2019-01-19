#include "PacketRuleTerminate.h"
#include "ObjectDelivererProtocol.h"
#include "PacketRuleFactory.h"

UPacketRuleTerminate::UPacketRuleTerminate()
{
	Terminate.Add(TEXT('\r'));
	Terminate.Add(TEXT('\n'));
}

UPacketRuleTerminate::~UPacketRuleTerminate()
{
}

void UPacketRuleTerminate::Initialize_Implementation()
{
	BufferForSend.Reset(1024);
	ReceiveTempBuffer.Reset(1024);
	BufferForReceive.Reset(1024);
}

void UPacketRuleTerminate::MakeSendPacket_Implementation(const TArray<uint8>& BodyBuffer)
{
	BufferForSend.SetNum(BodyBuffer.Num() + Terminate.Num(), false);

	FMemory::Memcpy(BufferForSend.GetData(), BodyBuffer.GetData(), BodyBuffer.Num());
	FMemory::Memcpy(BufferForSend.GetData() + BodyBuffer.Num(), Terminate.GetData(), Terminate.Num());

	DispatchMadeSendBuffer(BufferForSend);
}

void UPacketRuleTerminate::NotifyReceiveData_Implementation(const TArray<uint8>& DataBuffer)
{
	ReceiveTempBuffer += DataBuffer;

	int32 findIndex = -1;

	while (true)
	{
		for (int i = 0; i <= ReceiveTempBuffer.Num() - Terminate.Num(); ++i)
		{
			bool notEqual = false;
			for (int j = 0; j <= Terminate.Num() - Terminate.Num(); ++j)
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

int32 UPacketRuleTerminate::GetWantSize_Implementation()
{
	return 0;
}

UPacketRule* UPacketRuleTerminate::Clone_Implementation()
{
	return UPacketRuleFactory::CreatePacketRuleTerminate(Terminate);
}