// Copyright 2019 ayumax. All Rights Reserved.
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

	template<typename T>
	void Write(T Value)
	{
		if (!Ar) return;

		Ar->Serialize(&Value, sizeof(T));
	}
	

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
	double ReadDouble();

	template<typename T>
	T Read()
	{
		if (!Ar) return 0;

		T Value;
		Ar->Serialize(&Value, sizeof(T));

		return Value;
	}

	void Close();
	
	bool IsEnd();
	int64 RemainSize();

public:
	int64 FileSize;

private:
	FArchive* Ar;
};
