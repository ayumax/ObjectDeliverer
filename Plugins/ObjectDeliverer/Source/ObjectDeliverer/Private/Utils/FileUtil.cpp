#include "FileUtil.h"
#include "HAL/FileManager.h"

FileWriterUtil::FileWriterUtil()
	: Ar(nullptr)
{

}

FileWriterUtil::FileWriterUtil(const FString& FilePath, uint32 WriteFlags)
{
	Open(FilePath, WriteFlags);
}

FileWriterUtil::~FileWriterUtil()
{
	Close();
}

bool FileWriterUtil::Open(const FString& FilePath, uint32 WriteFlags)
{
	Ar = IFileManager::Get().CreateFileWriter(*FilePath, WriteFlags);
	
	if (!Ar)
	{
		return false;
	}

	return true;
}

void FileWriterUtil::Write(const TArray<uint8> Buffer, int32 Length)
{
	if (!Ar) return;

	if (Length == 0)
	{
		Length = Buffer.Num();
	}

	Ar->Serialize(const_cast<uint8*>(Buffer.GetData()), Length);
}

void FileWriterUtil::Close()
{
	if (!Ar) return;

	delete Ar;
	Ar = nullptr;
}

/*************************************************************/

FileReaderUtil::FileReaderUtil()
	: Ar(nullptr)
{

}

FileReaderUtil::FileReaderUtil(const FString& FilePath, uint32 ReadFlags)
{
	Open(FilePath, ReadFlags);
}

FileReaderUtil::~FileReaderUtil()
{
	Close();
}

bool FileReaderUtil::Open(const FString& FilePath, uint32 ReadFlags)
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

void FileReaderUtil::Read(TArray<uint8>& Buffer, int32 Length)
{
	if (!Ar) return;

	Ar->Serialize(Buffer.GetData(), Length);
}

double FileReaderUtil::ReadDouble()
{
	if (!Ar) return 0;

	double retValue = 0;

	Ar->Serialize(&retValue, sizeof(double));

	return retValue;
}

void FileReaderUtil::Close()
{
	if (!Ar) return;

	delete Ar;
	Ar = nullptr;
}