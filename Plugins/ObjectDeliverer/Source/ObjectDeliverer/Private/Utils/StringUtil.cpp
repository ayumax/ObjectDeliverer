// Copyright 2019 ayumax. All Rights Reserved.
#include "StringUtil.h"
#include <string>

void UStringUtil::StringToBuffer(const FString& message, TArray<uint8>& DataBuffer)
{
	std::string _str = TCHAR_TO_UTF8(*message);

	DataBuffer.SetNum(_str.size() + 2);
	FMemory::Memset(DataBuffer.GetData(), 0, DataBuffer.Num());

	FMemory::Memcpy(DataBuffer.GetData(), _str.c_str(), _str.size());

}

FString UStringUtil::BufferToString(const TArray<uint8>& DataBuffer)
{
	return UTF8_TO_TCHAR(DataBuffer.GetData());
}