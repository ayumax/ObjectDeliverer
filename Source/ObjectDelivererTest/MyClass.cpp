// Fill out your copyright notice in the Description page of Project Settings.

#include "MyClass.h"
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
	deliverer->Connected.AddDynamic(serverHelper, &MyClass::OnConnect);
	// bind disconnected event
	deliverer->Disconnected.AddDynamic(serverHelper, &MyClass::OnDisConnect);
	// bind receive event
	deliverer->ReceiveData.AddDynamic(serverHelper, &MyClass::OnReceive);

	// start deliverer
	// + protocol : TCP/IP Server
	// + Data division rule : Header(BodySize) + Body
	// + Serialization method : Byte Array
	deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());
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