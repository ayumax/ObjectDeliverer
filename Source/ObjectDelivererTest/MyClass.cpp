// Fill out your copyright notice in the Description page of Project Settings.

#include "MyClass.h"
#include "Utf8StringDeliveryBox.h"
#include "ObjectDeliveryBoxUsingJson.h"
#include "ObjectDelivererManager.h"

MyClass::MyClass()
{
}

MyClass::~MyClass()
{
}

void MyClass::Start()
{
	auto deliverer = NewObject<UObjectDelivererManager>();

	// bind connected event
	deliverer->Connected.AddDynamic(this, &MyClass::OnConnect);
	// bind disconnected event
	deliverer->Disconnected.AddDynamic(this, &MyClass::OnDisConnect);
	// bind receive event
	deliverer->ReceiveData.AddDynamic(this, &MyClass::OnReceive);

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
	deliverybox->Received.AddDynamic(this, &MyClass::OnReceiveString);
	deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody(), deliverybox);

	deliverybox->Send(TEXT("ABCDEFG"));

	// Object(Json)
	auto deliverybox = NewObject<UObjectDeliveryBoxUsingJson>();
	deliverybox->Received.AddDynamic(this, &MyClass::OnReceiveObject);
	deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody(), deliverybox);

	auto obj = NewObject<SampleObject>();
	deliverybox->Send(obj);
}

void MyClass::OnConnect(UObjectDelivererProtocol* ClientSocket)
{
	// send data
	TArray<uint8> buffer;
	deliverer->Send(buffer);
}

void MyClass::OnDisConnect(UObjectDelivererProtocol* ClientSocket)
{
	// closed
	UE_LOG(LogTemp, Log, TEXT("closed"));
}

void MyClass::OnReceive(UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
{
	// received data buffer
}

void MyClass::OnReceiveString(UObjectDelivererProtocol* ClientSocket, FString ReceivedString)
{
	// received data string
}

void MyClass::OnReceiveObject(UObjectDelivererProtocol* ClientSocket, const UObject* ReceivedObject)
{
	// received data object
}