// Copyright 2019 ayumax. All Rights Reserved.
#include "ODStringUtil.h"
#include <string>

void UODStringUtil::StringToBuffer(const FString& message, TArray<uint8>& DataBuffer)
{
	std::string _str = TCHAR_TO_UTF8(*message);

	DataBuffer.SetNum(_str.size() + 1);
	DataBuffer[DataBuffer.Num() - 1] = 0x00;

	FMemory::Memcpy(DataBuffer.GetData(), _str.c_str(), _str.size());

}

FString UODStringUtil::BufferToString(const TArray<uint8>& DataBuffer)
{
	if (DataBuffer[DataBuffer.Num() - 1] == 0x00)
	{
		return UTF8_TO_TCHAR(DataBuffer.GetData());
	}

	TArray<uint8> tempBuffer;
	tempBuffer.SetNum(DataBuffer.Num() + 1);
	FMemory::Memcpy(tempBuffer.GetData(), DataBuffer.GetData(), DataBuffer.Num());
	tempBuffer[tempBuffer.Num() - 1] = 0x00;

	return UTF8_TO_TCHAR(tempBuffer.GetData());
}