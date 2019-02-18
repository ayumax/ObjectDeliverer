// Copyright 2019 ayumax. All Rights Reserved.

#include "MyClass.h"
#include "Utf8StringDeliveryBox.h"
#include "ObjectDeliveryBoxUsingJson.h"
#include "ObjectDelivererManager.h"
#include "ProtocolTcpIpClient.h"
#include "ProtocolTcpIpServer.h"
#include "PacketRuleSizeBody.h"
#include "ProtocolUdpSocketSender.h"
#include "ProtocolUdpSocketReceiver.h"
#include "PacketRuleFixedLength.h"
#include "PacketRuleNodivision.h"
#include "PacketRuleTerminate.h"

USampleObject::USampleObject()
{
}

USampleObject::~USampleObject()
{
}

UMyClass::UMyClass()
{
}

UMyClass::~UMyClass()
{
}

void UMyClass::Start()
{
	auto deliverer = NewObject<UObjectDelivererManager>();

	// bind connected event
	deliverer->Connected.AddDynamic(this, &UMyClass::OnConnect);
	// bind disconnected event
	deliverer->Disconnected.AddDynamic(this, &UMyClass::OnDisConnect);
	// bind receive event
	deliverer->ReceiveData.AddDynamic(this, &UMyClass::OnReceive);

	// start deliverer
	// + protocol : TCP/IP Server
	// + Data division rule : Header(BodySize) + Body
	// + Serialization method : Byte Array
	deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());


	// Change of communication protocol
	// TCP/IP Server
	deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	// TCP/IP Client
	deliverer->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099, true), UPacketRuleFactory::CreatePacketRuleSizeBody());

	// UDP Sender
	deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", 9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	// UDP Receiver
	deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	// FixedSize
	deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleFixedLength(1024));

	// Header(BodySize) + Body
	deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleSizeBody(4, ECNBufferEndian::Big));

	// Split by terminal symbol
	deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleTerminate({ TEXT('\r'), TEXT('\n') }));

	// Nodivision
	deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleNodivision());


	// Byte array
	deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

	// UTF-8 string
	auto deliverybox = NewObject<UUtf8StringDeliveryBox>();
	deliverybox->Received.AddDynamic(this, &UMyClass::OnReceiveString);
	deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody(), deliverybox);

	deliverybox->Send(TEXT("ABCDEFG"));


	// Object(Json)
	auto deliverybox2 = NewObject<UObjectDeliveryBoxUsingJson>();
	deliverybox2->Initialize(USampleObject::StaticClass());
	deliverybox2->Received.AddDynamic(this, &UMyClass::OnReceiveObject);
	deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody(), deliverybox);

	auto obj = NewObject<USampleObject>();
	deliverybox2->Send(obj);
}

void UMyClass::OnConnect(UObjectDelivererProtocol* ClientSocket)
{
	// send data
	TArray<uint8> buffer;
	//deliverer->Send(buffer);
}

void UMyClass::OnDisConnect(UObjectDelivererProtocol* ClientSocket)
{
	// closed
	UE_LOG(LogTemp, Log, TEXT("closed"));
}

void UMyClass::OnReceive(UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
{
	// received data buffer
}

void UMyClass::OnReceiveString(FString ReceivedString)
{
	// received data string
}

void UMyClass::OnReceiveObject(UObject* ReceivedObject)
{
	// received data object
	USampleObject* obj = Cast<USampleObject>(ReceivedObject);
}