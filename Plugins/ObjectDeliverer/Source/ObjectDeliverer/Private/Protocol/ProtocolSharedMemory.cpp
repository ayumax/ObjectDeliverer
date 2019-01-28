#include "ProtocolSharedMemory.h"
#include "Utils/WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "PacketRule.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#endif

UProtocolSharedMemory::UProtocolSharedMemory()
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
	ReceiveBuffer.SetNum(1024);
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

}


bool UProtocolSharedMemory::ReceivedData()
{
	uint32 Size = 0;
	while (true)
	{
		uint32 wantSize = PacketRule->GetWantSize();

		if (wantSize > 0)
		{
			if (Size < wantSize) return true;
		}

		auto receiveSize = wantSize == 0 ? Size : wantSize;

		ReceiveBuffer.SetNum(receiveSize, false);

		int32 Read = 0;
		/*if (!InnerSocket->Recv(ReceiveBuffer.GetData(), ReceiveBuffer.Num(), Read, ESocketReceiveFlags::WaitAll))
		{
			CloseInnerSocket();
			DispatchDisconnected(this);
			return false;
		}*/

		PacketRule->NotifyReceiveData(ReceiveBuffer);
	}

	return true;
}


void UProtocolSharedMemory::RequestSend(const TArray<uint8>& DataBuffer)
{

}