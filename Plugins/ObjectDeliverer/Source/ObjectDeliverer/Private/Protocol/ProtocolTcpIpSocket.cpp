// Copyright 2019 ayumax. All Rights Reserved.
#include "Protocol/ProtocolTcpIpSocket.h"
#include "Common/TcpSocketBuilder.h"
#include "Utils/ODWorkerThread.h"
#include "HAL/RunnableThread.h"
#include "PacketRule/PacketRule.h"

UProtocolTcpIpSocket::UProtocolTcpIpSocket()
{

}

UProtocolTcpIpSocket::~UProtocolTcpIpSocket()
{
}


void UProtocolTcpIpSocket::Close()
{
	CloseSocket();
}


void UProtocolTcpIpSocket::CloseSocket()
{
	if (!InnerSocket) return;

	IsSelfClose = true;

	InnerSocket->Close();

	FScopeLock lock(&ct);

	if (!CurrentThread) return;
	CurrentThread->Kill(true);

	delete CurrentThread;
	CurrentThread = nullptr;

	if (!CurrentInnerThread) return;
	delete CurrentInnerThread;
	CurrentInnerThread = nullptr;

	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(InnerSocket);
	InnerSocket = nullptr;
}

void UProtocolTcpIpSocket::Send(const TArray<uint8>& DataBuffer) const
{
	if (!InnerSocket) return;

	PacketRule->MakeSendPacket(DataBuffer);
}

void UProtocolTcpIpSocket::OnConnected(FSocket* ConnectionSocket)
{
	InnerSocket = ConnectionSocket;
	StartPollilng();
}

void UProtocolTcpIpSocket::StartPollilng()
{
	ReceiveBuffer.SetLength(0);
	CurrentInnerThread = new FODWorkerThread([this] 
	{ 
		FScopeLock lock(&ct);
		return ReceivedData();
	});
	CurrentThread = FRunnableThread::Create(CurrentInnerThread, TEXT("ObjectDeliverer TcpIpSocket PollingThread"));
}

bool UProtocolTcpIpSocket::ReceivedData()
{
	if (!InnerSocket)
	{
		DispatchDisconnected(this);
		return false;
	}

	// check if the socket has closed
	{
		int32 BytesRead;
		uint8 Dummy;
		if (!InnerSocket->Recv(&Dummy, 1, BytesRead, ESocketReceiveFlags::Peek))
		{
			if (!IsSelfClose)
			{
				CloseInnerSocket();
				DispatchDisconnected(this);
			}
			
			return false;
		}
	}

	// Block waiting for some data
	if (!InnerSocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(0.1)))
	{
		if (InnerSocket->GetConnectionState() == SCS_ConnectionError)
		{
			if (!IsSelfClose)
			{
				CloseInnerSocket();
				DispatchDisconnected(this);
			}
			return false;
		}
		return true;
	}

	uint32 Size = 0;
	while (InnerSocket->HasPendingData(Size))
	{
		ReceiveBuffer.SetLength(Size);

		int32 Read = 0;
		if (!InnerSocket->Recv(ReceiveBuffer.AsSpan().Buffer, ReceiveBuffer.GetLength(), Read, ESocketReceiveFlags::WaitAll))
		{
			if (!IsSelfClose)
			{
				CloseInnerSocket();
				DispatchDisconnected(this);
			}
			return false;
		}

		ReceiveBuffer.SetLength(Read);

		while(ReceiveBuffer.GetLength() > 0)
		{
			const int32 wantSize = PacketRule->GetWantSize();

			if (wantSize > 0)
			{
				if (ReceiveBuffer.GetLength() < wantSize) return true;
			}

			const auto receiveSize = wantSize == 0 ? ReceiveBuffer.GetLength() : wantSize;

			PacketRule->NotifyReceiveData(ReceiveBuffer.AsSpan(0, receiveSize).ToArray());

			ReceiveBuffer.RemoveRangeFromStart(0, receiveSize);
		}		
	}

	return true;
}

void UProtocolTcpIpSocket::RequestSend(const TArray<uint8>& DataBuffer)
{
	SendToConnected(DataBuffer);
}

bool UProtocolTcpIpSocket::GetIPAddress(TArray<uint8>& IPAddress)
{
	if (InnerSocket == nullptr) return false;

	TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InnerSocket->GetPeerAddress(*Addr);
	IPAddress.SetNum(0);
	IPAddress = Addr->GetRawIp();

	return true;
}

bool UProtocolTcpIpSocket::GetIPAddressInString(FString& IPAddress)
{
	if (InnerSocket == nullptr) return false;

	TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InnerSocket->GetPeerAddress(*Addr);

	IPAddress = Addr->ToString(false);

	return true;
}
