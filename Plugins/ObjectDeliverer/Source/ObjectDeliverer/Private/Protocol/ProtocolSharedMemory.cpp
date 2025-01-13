// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolSharedMemory.h"
#include "Utils/ODWorkerThread.h"
#include "HAL/RunnableThread.h"
#include "PacketRule/PacketRule.h"

#if PLATFORM_WINDOWS
#include "Utils/ODMutexLock.h"
#include "Windows/WindowsHWrapper.h"
#endif

UProtocolSharedMemory::UProtocolSharedMemory()
	: SharedMemoryHandle(nullptr)
	, SharedMemoryData(nullptr)
	, SharedMemoryMutex(nullptr)
	, NowCounter(0)
{

}

UProtocolSharedMemory::~UProtocolSharedMemory()
{

}

void UProtocolSharedMemory::Initialize(const FString& _SharedMemoryName/* = "SharedMemory"*/, int32 _SharedMemorySize/* = 1024*/)
{
	SharedMemoryName = _SharedMemoryName;
	SharedMemorySize = _SharedMemorySize;
}

void UProtocolSharedMemory::Start()
{
#if PLATFORM_WINDOWS
	/*  Create a named mutex for inter-process protection of data */
	FString mutexName = SharedMemoryName + "MUTEX";
	SharedMemoryTotalSize = SharedMemorySize + sizeof(uint8) + sizeof(int32);

	SharedMemoryMutex = CreateMutex(NULL, false, *mutexName);

	if (SharedMemoryMutex == nullptr) return;

	SharedMemoryHandle = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		SharedMemoryTotalSize,
		*SharedMemoryName);

	if (SharedMemoryHandle == nullptr)
	{
		CloseSharedMemory();
		return;
	}

	SharedMemoryData = (unsigned char*)MapViewOfFile(SharedMemoryHandle,
		FILE_MAP_ALL_ACCESS,
		0, 0,
		SharedMemoryTotalSize);

	if (!SharedMemoryData)
	{
		CloseSharedMemory();
		return;
	}

	ODMutexLock::Lock(SharedMemoryMutex, [this]()
		{
			FMemory::Memset(SharedMemoryData, 0, SharedMemoryTotalSize);
		});

	NowCounter = 0;

#else
	return;
#endif

	ReceiveBuffer.SetNum(SharedMemoryTotalSize);
	CurrentInnerThread = new FODWorkerThread([this] { return ReceivedData(); });
	CurrentThread = FRunnableThread::Create(CurrentInnerThread, TEXT("ObjectDeliverer ProtocolSharedMemory PollingThread"));

	DispatchConnected(this);
}

void UProtocolSharedMemory::Close()
{
#if PLATFORM_WINDOWS
	ODMutexLock::Lock(SharedMemoryMutex, [this]()
		{
			if (!CurrentThread) return;
			CurrentThread->Kill(true);

			delete CurrentThread;
			CurrentThread = nullptr;

			if (!CurrentInnerThread) return;
			delete CurrentInnerThread;
			CurrentInnerThread = nullptr;

			CloseSharedMemory();
		});
#endif

}


bool UProtocolSharedMemory::ReceivedData()
{
#if PLATFORM_WINDOWS

	uint32 Size{ 0u };

	ODMutexLock::Lock
	(
		SharedMemoryMutex,
		[this, &Size]() noexcept
		{
			uint8 counter{ 0u };
			const auto uint8size{ sizeof(uint8) };
			FMemory::Memcpy(&counter, SharedMemoryData, uint8size);

			if (counter == NowCounter)
				return;

			NowCounter = MoveTemp(counter);
			const auto NextSharedMemoryData{ SharedMemoryData + uint8size };
			const auto uint32size{ sizeof(uint32) };
			FMemory::Memcpy(&Size, NextSharedMemoryData, uint32size);
			TempBuffer.SetNum(Size, EAllowShrinking::No);
			FMemory::Memcpy(TempBuffer.GetData(), NextSharedMemoryData + uint32size, FMath::Min(StaticCast<decltype(Size)>(SharedMemorySize), Size));
		}
	);

	if (Size == 0)
		return true;

	auto wantSize{ PacketRule->GetWantSize() };

	if (wantSize > 0 && Size < StaticCast<uint32>(wantSize))
		return true;

	decltype(Size) Offset{ 0u };

	while (Size > 0u)
	{
		wantSize = PacketRule->GetWantSize();
		const auto receiveSize{ wantSize == 0 ? Size : wantSize };
		ReceiveBuffer.SetNum(receiveSize, EAllowShrinking::No);
		FMemory::Memcpy(ReceiveBuffer.GetData(), TempBuffer.GetData() + Offset, receiveSize);
		Offset += receiveSize;
		Size -= receiveSize;
		PacketRule->NotifyReceiveData(ReceiveBuffer);
	}

	return true;

#else

	return false;

#endif
}

void UProtocolSharedMemory::Send(const TArray<uint8>& DataBuffer) const
{
#if PLATFORM_WINDOWS
	if (!SharedMemoryHandle) return;

	PacketRule->MakeSendPacket(DataBuffer);
#endif
}

void UProtocolSharedMemory::RequestSend(const TArray<uint8>& DataBuffer)
{
#if PLATFORM_WINDOWS
	if (DataBuffer.Num() > SharedMemorySize)
		return;

	if (SharedMemoryMutex && SharedMemoryData)
	{
		int32 writeSize = DataBuffer.Num();

		ODMutexLock::Lock(SharedMemoryMutex, [this, writeSize, &DataBuffer]()
			{
				NowCounter++;
				if (NowCounter == 0)
				{
					NowCounter = 1;
				}

				FMemory::Memcpy(SharedMemoryData, &NowCounter, sizeof(uint8));

				FMemory::Memcpy(SharedMemoryData + sizeof(uint8), &writeSize, sizeof(int32));
				FMemory::Memcpy(SharedMemoryData + sizeof(uint8) + sizeof(int32), DataBuffer.GetData(), writeSize);

				FlushViewOfFile(SharedMemoryData, sizeof(uint8) + sizeof(int32) + writeSize);
			});

	}
#endif
}

void UProtocolSharedMemory::CloseSharedMemory()
{
#if PLATFORM_WINDOWS
	if (SharedMemoryMutex != nullptr)
	{
		ReleaseMutex(SharedMemoryMutex);
		CloseHandle(SharedMemoryMutex);
		SharedMemoryMutex = nullptr;
	}

	if (SharedMemoryData != nullptr)
	{
		UnmapViewOfFile(SharedMemoryData);
		SharedMemoryData = nullptr;
	}

	if (SharedMemoryHandle != nullptr)
	{
		CloseHandle(SharedMemoryHandle);
		SharedMemoryHandle = nullptr;
	}
#endif
}