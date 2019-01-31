#pragma once

#include "CoreMinimal.h"

class FileWriterUtil
{
public:
	FileWriterUtil();
	FileWriterUtil(const FString& FilePath, uint32 WriteFlags = 0);
	~FileWriterUtil();

	bool Open(const FString& FilePath, uint32 WriteFlags = 0);
	void Write(const TArray<uint8> Buffer, int32 Length = 0);
	void Close();

private:
	FArchive* Ar;
};

class FileReaderUtil
{
public:
	FileReaderUtil();
	FileReaderUtil(const FString& FilePath, uint32 ReadFlags = 0);
	~FileReaderUtil();

	bool Open(const FString& FilePath, uint32 ReadFlags = 0);
	void Read(TArray<uint8>& Buffer, int32 Length);
	void Close();

public:
	int64 FileSize;

private:
	FArchive* Ar;
};
