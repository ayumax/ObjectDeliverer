# ObjectDeliverer
ObjectDeliverer is a data transmission / reception library for Unreal Engine (C ++, Blueprint).
It has the following features.

+ Communication protocol, data division rule, serialization method can be switched by part replacement.
+ Available for both C ++ and Blueprint
+ It is also possible to add your own customized parts(protocol, data division rule, serialization method)

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

# Change of communication protocol

```cpp
// TCP/IP Server
deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

// TCP/IP Client
deliverer->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099, true), UPacketRuleFactory::CreatePacketRuleSizeBody());

// UDP Sender
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", 9099), UPacketRuleFactory::CreatePacketRuleSizeBody());

// UDP Receiver
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleSizeBody());
```


# Change of data division rule

```cpp
// FixedSize
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleFixedLength(1024));

// Header(BodySize) + Body
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleSizeBody(4, ECNBufferEndian::Big));

// Split by terminal symbol
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleTerminate({ TEXT('\r'), TEXT('\n') }));

// Nodivision
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099), UPacketRuleFactory::CreatePacketRuleNodivision());
```

# Change of Serialization method
```cpp
// UTF-8 string
auto deliverybox = NewObject<UUtf8StringDeliveryBox>();
deliverybox->Received.AddDynamic(this, &MyClass::OnReceiveString);
deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody(), deliverybox);

deliverybox->Send(TEXT("ABCDEFG"));

void MyClass::OnReceiveString(UObjectDelivererProtocol* ClientSocket, FString ReceivedString)
{
    // received data string
}
```

```cpp
// Object(Json)
auto deliverybox = NewObject<UObjectDeliveryBoxUsingJson>();
deliverybox->Received.AddDynamic(this, &MyClass::OnReceiveObject);
deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099), UPacketRuleFactory::CreatePacketRuleSizeBody(), deliverybox);

auto obj = NewObject<SampleObject>();
deliverybox->Send(obj);

void MyClass::OnReceiveObject(UObjectDelivererProtocol* ClientSocket, const UObject* ReceivedObject)
{
	// received data object
}
```