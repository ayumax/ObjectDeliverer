// Copyright 2019 ayumax. All Rights Reserved.
#include "ODFileUtil.h"
#include "HAL/FileManager.h"

ODFileWriterUtil::ODFileWriterUtil()
	: Ar(nullptr)
{

}

ODFileWriterUtil::ODFileWriterUtil(const FString& FilePath, uint32 WriteFlags)
{
	Open(FilePath, WriteFlags);
}

ODFileWriterUtil::~ODFileWriterUtil()
{
	Close();
}

bool ODFileWriterUtil::Open(const FString& FilePath, uint32 WriteFlags)
{
	Ar = IFileManager::Get().CreateFileWriter(*FilePath, WriteFlags);
	
	if (!Ar)
	{
		return false;
	}

	return true;
}

void ODFileWriterUtil::Write(const TArray<uint8> Buffer, int32 Length)
{
	if (!Ar) return;

	if (Length == 0)
	{
		Length = Buffer.Num();
	}

	Ar->Serialize(const_cast<uint8*>(Buffer.GetData()), Length);
}

void ODFileWriterUtil::Close()
{
	if (!Ar) return;

	delete Ar;
	Ar = nullptr;
}

/*************************************************************/

ODFileReaderUtil::ODFileReaderUtil()
	: Ar(nullptr)
{

}

ODFileReaderUtil::ODFileReaderUtil(const FString& FilePath, uint32 ReadFlags)
{
	Open(FilePath, ReadFlags);
}

ODFileReaderUtil::~ODFileReaderUtil()
{
	Close();
}

bool ODFileReaderUtil::Open(const FString& FilePath, uint32 ReadFlags)
{
	Ar = IFileManager::Get().CreateFileReader(*FilePath, ReadFlags);

	if (!Ar)
	{
		if (!(ReadFlags & FILEREAD_Silent))
		{
			UE_LOG(LogStreaming, Warning, TEXT("Failed to read file '%s' error."), *FilePath);
		}
		return false;
	}

	FileSize = Ar->TotalSize();

	return true;
}

void ODFileReaderUtil::Read(TArray<uint8>& Buffer, int32 Length)
{
	if (!Ar) return;

	Ar->Serialize(Buffer.GetData(), Length);
}

double ODFileReaderUtil::ReadDouble()
{
	if (!Ar) return 0;

	double retValue = 0;

	Ar->Serialize(&retValue, sizeof(double));

	return retValue;
}

void ODFileReaderUtil::Close()
{
	if (!Ar) return;

	delete Ar;
	Ar = nullptr;
}

bool ODFileReaderUtil::IsEnd()
{
	if (!Ar) return true;

	return Ar->AtEnd();
}

int64 ODFileReaderUtil::RemainSize()
{
	if (!Ar) return 0;

	return Ar->TotalSize() - Ar->Tell();
}