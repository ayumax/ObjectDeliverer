# ObjectDeliverer
ObjectDeliverer is a data transmission / reception library for Unreal Engine (C ++, Blueprint).
It has the following features.

+ Communication protocol, data division rule, serialization method can be switched by part replacement.
+ Available for both C ++ and Blueprint
+ It is also possible to add your own customized parts

## Communication protocol
The following protocols can be used with built-in.
You can also add your own protocol.
+ TCP/IP Server(Connectable to multiple clients)
+ TCP/IP Client
+ UDP(Sender)
+ UDP(Receiver)

## Data division rule
The following rules are available for built-in split rules of transmitted and received data.
+ FixedSize
+ Header(BodySize) + Body
+ Split by terminal symbol

## Serialization method
+ Byte Array
+ UTF-8 string
+ Object(Json)

# Installation
+ Please clone this repository
+ Please copy the Plugins directory to the project folder
+ Please activate ObjectDeliverer from Plugins after launching editor

# Quick Start

```cpp
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
```
