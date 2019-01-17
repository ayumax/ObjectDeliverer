#include "CNStringUtil.h"
#include <string>

void UCNStringUtil::StringToBuffer(const FString& message, TArray<uint8>& DataBuffer)
{
	std::string _str = TCHAR_TO_UTF8(*message);

	DataBuffer.SetNum(_str.size());
	FMemory::Memcpy(DataBuffer.GetData(), _str.c_str(), _str.size());

}

FString UCNStringUtil::BufferToString(const TArray<uint8>& DataBuffer)
{
	return UTF8_TO_TCHAR(DataBuffer.GetData());
}