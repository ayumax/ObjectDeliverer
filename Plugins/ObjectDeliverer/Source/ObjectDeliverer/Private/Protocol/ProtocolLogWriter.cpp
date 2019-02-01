#include "ProtocolLogWriter.h"
#include "PacketRule.h"
#include "Utils/FileUtil.h"


UProtocolLogWriter::UProtocolLogWriter()
	: Writer(nullptr)
{

}

UProtocolLogWriter::~UProtocolLogWriter()
{

}

void UProtocolLogWriter::Initialize(const FString& _FilePath, bool _PathIsAblolute)
{
	FilePath = _FilePath;
	PathIsAblolute = _PathIsAblolute;
}

void UProtocolLogWriter::Start()
{
	if (Writer) delete Writer;

	Writer = new FileWriterUtil();
	Writer->Open(FilePath, 0);

	StartTime = FDateTime::Now();
}

void UProtocolLogWriter::Close()
{
	if (!Writer) return;

	Writer->Close();
	delete Writer;
	Writer = nullptr;
}

void UProtocolLogWriter::Send(const TArray<uint8>& DataBuffer)
{
	PacketRule->MakeSendPacket(DataBuffer);
}

void UProtocolLogWriter::RequestSend(const TArray<uint8>& DataBuffer)
{
	if (!Writer) return;

	auto time = (FDateTime::Now() - StartTime).GetTotalMilliseconds();
	Writer->Write(time);
	Writer->Write(DataBuffer.Num());
	Writer->Write(DataBuffer);
}

