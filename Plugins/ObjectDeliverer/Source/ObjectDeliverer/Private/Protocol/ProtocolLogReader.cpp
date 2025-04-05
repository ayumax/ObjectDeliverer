// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolLogReader.h"
#include "PacketRule/PacketRule.h"
#include "Utils/ODFileUtil.h"
#include "Utils/ODWorkerThread.h"
#include "HAL/RunnableThread.h"
#include "Misc/Paths.h"


UProtocolLogReader::UProtocolLogReader()
	: Reader(nullptr)
	, CurrentLogTime(-1)
	, FilePosition(0)
	, IsFirst(true)
{

}

UProtocolLogReader::~UProtocolLogReader()
{

}

void UProtocolLogReader::Initialize(const FString& _FilePath, bool _PathIsAbsolute, bool _CutFirstInterval)
{
	FilePath = _FilePath;
	PathIsAbsolute = _PathIsAbsolute;
	CutFirstInterval = _CutFirstInterval;

	ReceiveBuffer.SetNum(1024);
}

void UProtocolLogReader::Start()
{
	if (Reader) delete Reader;

	auto readPath = FilePath;
	if (!PathIsAbsolute)
	{
		readPath = FPaths::Combine(FPaths::ProjectLogDir(), FilePath);
	}

	Reader = new ODFileReaderUtil();
	Reader->Open(readPath, 0);

	StartTime = FDateTime::Now();
	CurrentLogTime = -1;
	IsFirst = true;

	CurrentInnerThread = new FODWorkerThread([this] { return ReadData(); }, [this] { return ReadEnd(); }, 0.001);
	CurrentThread = FRunnableThread::Create(CurrentInnerThread, TEXT("ObjectDeliverer ProtocolLogReader PollingThread"));

	DispatchConnected(this);
}

void UProtocolLogReader::Close()
{
	if (!CurrentThread) return;
	CurrentThread->Kill(true);

	delete CurrentThread;
	CurrentThread = nullptr;

	if (!CurrentInnerThread) return;
	delete CurrentInnerThread;
	CurrentInnerThread = nullptr;

	if (!Reader) return;

	Reader->Close();
	delete Reader;
	Reader = nullptr;
}

void UProtocolLogReader::Send(const TArray<uint8>& DataBuffer) const
{

}

void UProtocolLogReader::RequestSend(const TArray<uint8>& DataBuffer)
{

}

bool UProtocolLogReader::ReadData()
{
	if (!Reader)
		return false;

	while (!Reader->IsEnd() || CurrentLogTime >= 0)
	{
		if (CurrentLogTime >= 0)
		{
			const auto nowTime{ (FDateTime::Now() - StartTime).GetTotalMilliseconds() };

			if (CurrentLogTime > nowTime)
				break;

			auto Size{ ReadBuffer.Num() };
			auto wantSize{ PacketRule->GetWantSize() };

			if (wantSize > 0 && Size < wantSize)
				return true;

			auto Offset{ 0u };

			while (Size > 0)
			{
				wantSize = PacketRule->GetWantSize();
				const auto receiveSize{ wantSize == 0 ? Size : wantSize };
				ReceiveBuffer.SetNum(receiveSize, EAllowShrinking::No);
				FMemory::Memcpy(ReceiveBuffer.GetData(), ReadBuffer.GetData() + Offset, receiveSize);
				Offset += receiveSize;
				Size -= receiveSize;
				PacketRule->NotifyReceiveData(ReceiveBuffer);
			}

			CurrentLogTime = -1;
		}

		if (Reader->RemainSize() < sizeof(double))
			return false;

		CurrentLogTime = Reader->Read<double>();

		if (IsFirst && CutFirstInterval)
			StartTime -= FTimespan::FromMilliseconds(CurrentLogTime);

		IsFirst = false;

		if (Reader->RemainSize() < sizeof(int32))
			return false;

		const auto bufferSize{ Reader->Read<int32>() };

		if (Reader->RemainSize() < bufferSize)
			return false;

		ReadBuffer.SetNum(bufferSize, EAllowShrinking::No);
		Reader->Read(ReadBuffer, bufferSize);
	}

	return true;
}

void UProtocolLogReader::ReadEnd()
{
	DispatchDisconnected(this);
}
