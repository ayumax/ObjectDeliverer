#include "ProtocolSharedMemory.h"
#include "Utils/WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "PacketRule.h"
#include "Utils/MutexLock.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#endif

UProtocolSharedMemory::UProtocolSharedMemory()
	: SharedMemoryHandle(nullptr)
	, SharedMemoryMutex(nullptr)
	, SharedMemoryData(nullptr)
	, SharedMemorySize(0)
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

void UProtocolSharedMemory::Start_Implementation()
{
#ifdef PLATFORM_WINDOWS
	/*  Create a named mutex for inter-process protection of data */
	FString mutexName = SharedMemoryName + "MUTEX";
	int32 MemorySize = SharedMemorySize + sizeof(uint8) + sizeof(int32);

	SharedMemoryMutex = CreateMutex(NULL, false, *mutexName);

	if (SharedMemoryMutex == nullptr) return;

	SharedMemoryHandle = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		MemorySize,
		*SharedMemoryName);

	if (GetLastError() == ERROR_ALREADY_EXISTS || SharedMemoryHandle == nullptr)
	{
		CloseSharedMemory();
		return;
	}

	SharedMemoryData = (unsigned char *)MapViewOfFile(SharedMemoryHandle,
		FILE_MAP_ALL_ACCESS,
		0, 0,
		MemorySize);

	if (!SharedMemoryData)
	{
		CloseSharedMemory();
		return;
	}

	{
		MutexLock lock(SharedMemoryMutex);
		FMemory::Memset(SharedMemoryData, 0, MemorySize);
	}

	NowCounter = 0;

#else
	return;
#endif

	ReceiveBuffer.SetNum(MemorySize);
	CurrentInnerThread = new FWorkerThread([this] { return ReceivedData(); });
	CurrentThread = FRunnableThread::Create(CurrentInnerThread, TEXT("ObjectDeliverer ProtocolSharedMemory PollingThread"));
}

void UProtocolSharedMemory::Close_Implementation()
{
	if (!CurrentThread) return;
	CurrentThread->Kill(true);

	delete CurrentThread;
	CurrentThread = nullptr;

	if (!CurrentInnerThread) return;
	delete CurrentInnerThread;
	CurrentInnerThread = nullptr;

	CloseSharedMemory();
}


bool UProtocolSharedMemory::ReceivedData()
{
#ifdef PLATFORM_WINDOWS

	uint32 Size = 0;
	{
		uint8 counter = 0;
		MutexLock lock(SharedMemoryMutex);
		FMemory::Memcpy(&counter, SharedMemoryData, sizeof(uint8));
		if (counter == NowCounter) return true;

		NowCounter = counter;

		FMemory::Memcpy(&Size, SharedMemoryData + sizeof(uint8), sizeof(uint32));

		TempBuffer.SetNum(Size, false);

		FMemory::Memcpy(TempBuffer.GetData(), SharedMemoryData + sizeof(uint8) + sizeof(uint32), Size);
	}
	
	uint32 wantSize = PacketRule->GetWantSize();

	if (wantSize > 0)
	{
		if (Size < wantSize) return true;
	}

	int32 Offset = 0;
	while (Size > 0)
	{
		auto receiveSize = wantSize == 0 ? Size : wantSize;

		ReceiveBuffer.SetNum(receiveSize, false);

		int32 Read = 0;
		FMemory::Memcpy(ReceiveBuffer.GetData(), TempBuffer.GetData() + Offset, receiveSize);
		Offset += receiveSize;
		Size -= receiveSize;

		PacketRule->NotifyReceiveData(ReceiveBuffer);
	}

#endif
	return true;
}


void UProtocolSharedMemory::RequestSend(const TArray<uint8>& DataBuffer)
{
#ifdef PLATFORM_WINDOWS
	if (DataBuffer.Num() > SharedMemorySize)
		return;

	if (SharedMemoryMutex && SharedMemoryData)
	{
		int32 writeSize = DataBuffer.Num();

		NowCounter++;
		if (NowCounter == 0)
		{
			NowCounter = 1;
		}

		MutexLock lock(SharedMemoryMutex);
		FMemory::Memcpy(SharedMemoryData, &NowCounter, sizeof(uint8));

		FMemory::Memcpy(SharedMemoryData + sizeof(uint8), &writeSize, sizeof(int32));
		FMemory::Memcpy(SharedMemoryData + sizeof(uint8) + sizeof(int32), DataBuffer.GetData(), writeSize);

		FlushViewOfFile(SharedMemoryData, sizeof(uint8) + sizeof(int32) + writeSize);
	}
#endif
}

void UProtocolSharedMemory::CloseSharedMemory()
{
#ifdef PLATFORM_WINDOWS
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