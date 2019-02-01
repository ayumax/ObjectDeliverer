#include "ProtocolLogReader.h"
#include "PacketRule.h"
#include "Utils/FileUtil.h"
#include "Utils/WorkerThread.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"


UProtocolLogReader::UProtocolLogReader()
	: Reader(nullptr)
	, CurrentLogTime(0)
{

}

UProtocolLogReader::~UProtocolLogReader()
{

}

void UProtocolLogReader::Initialize(const FString& _FilePath, bool _PathIsAblolute)
{
	FilePath = _FilePath;
	PathIsAblolute = _PathIsAblolute;
}

void UProtocolLogReader::Start()
{
	if (Reader) delete Reader;

	Reader = new FileReaderUtil();
	Reader->Open(FilePath, 0);

	StartTime = FDateTime::Now();

	CurrentInnerThread = new FWorkerThread([this] { return ReadData(); });
	CurrentThread = FRunnableThread::Create(CurrentInnerThread, TEXT("ObjectDeliverer ProtocolLogReader PollingThread"));

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

void UProtocolLogReader::Send(const TArray<uint8>& DataBuffer)
{
	
}

void UProtocolLogReader::RequestSend(const TArray<uint8>& DataBuffer)
{
	
}

bool UProtocolLogReader::ReadData()
{
	if (!Reader) return false;

	if (Reader->FileSize < sizeof(double)) return false;

	CurrentLogTime = Reader->Read<double>();

	if (Reader->FileSize < sizeof(int32)) return false;

	int32 bufferSize = Reader->Read<int32>();

	if (Reader->FileSize < bufferSize) return false;

	ReadBuffer.SetNum(bufferSize, false);

	Reader->Read(ReadBuffer, bufferSize);

	return true;
}

