// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolLogWriter.h"
#include "PacketRule/PacketRule.h"
#include "Utils/ODFileUtil.h"
#include "Misc/Paths.h"

UProtocolLogWriter::UProtocolLogWriter()
	: Writer(nullptr)
{

}

UProtocolLogWriter::~UProtocolLogWriter()
{

}

void UProtocolLogWriter::Initialize(const FString& _FilePath, bool _PathIsAbsolute)
{
	FilePath = _FilePath;
	PathIsAbsolute = _PathIsAbsolute;
}

void UProtocolLogWriter::Start()
{
	if (Writer) delete Writer;

	auto writePath = FilePath;
	if (!PathIsAbsolute)
	{
		writePath = FPaths::Combine(FPaths::ProjectLogDir(), FilePath);
	}
		
	Writer = new ODFileWriterUtil();
	Writer->Open(writePath, 0);

	StartTime = FDateTime::Now();

	DispatchConnected(this);
}

void UProtocolLogWriter::Close()
{
	if (!Writer) return;

	Writer->Close();
	delete Writer;
	Writer = nullptr;
}

void UProtocolLogWriter::Send(const TArray<uint8>& DataBuffer) const
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

