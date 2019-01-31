#include "ProtocolLogWriter.h"
#include "PacketRule.h"
#include "Kismet/BlueprintFunctionLibrary.h"

UProtocolLogWriter::UProtocolLogWriter()
{

}

UProtocolLogWriter::~UProtocolLogWriter()
{

}

void UProtocolLogWriter::Initialize(const FString& IpAddress, int32 Port)
{
	DestinationIpAddress = IpAddress;
	DestinationPort = Port;
}

void UProtocolLogWriter::Start()
{
}

void UProtocolLogWriter::Close()
{
}

void UProtocolLogWriter::Send(const TArray<uint8>& DataBuffer)
{
	PacketRule->MakeSendPacket(DataBuffer);
}

void UProtocolLogWriter::RequestSend(const TArray<uint8>& DataBuffer)
{
}

