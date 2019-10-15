# ObjectDeliverer

## UE4 Marketplace
[https://www.unrealengine.com/marketplace/ja/slug/objectdeliverer](https://www.unrealengine.com/marketplace/ja/slug/objectdeliverer)

## Description
ObjectDeliverer is a data transmission / reception library for Unreal Engine (C ++, Blueprint).

It has the following features.

+ Communication protocol, data division rule, serialization method can be switched by part replacement.
+ Available for both C ++ and Blueprint

## Communication protocol
The following protocols can be used with built-in.
You can also add your own protocol.
+ TCP/IP Server(Connectable to multiple clients)
+ TCP/IP Client
+ UDP(Sender)
+ UDP(Receiver)
+ Shared Memory(Windows Only)
+ LogFile Writer
+ LogFile Reader

## Data division rule
The following rules are available for built-in split rules of transmitted and received data.
+ FixedSize  
	Example) In the case of fixed 1024 bytes
	![fixedlength](https://user-images.githubusercontent.com/8191970/56475737-7d999f00-64c7-11e9-8e9e-0182f1af8156.png)


+ Header(BodySize) + Body  
	Example) When the size area is 4 bytes  
	![sizeandbody](https://user-images.githubusercontent.com/8191970/56475796-6e672100-64c8-11e9-8cf0-6524f2899be0.png)


+ Split by terminal symbol  
	Example) When 0x00 is the end
	![terminate](https://user-images.githubusercontent.com/8191970/56475740-82f6e980-64c7-11e9-91a6-05d77cfdbd60.png)

## Serialization method
+ Byte Array
+ UTF-8 string
+ Object(Json)

# Installation
+ Please clone this repository
+ Please copy the Plugins directory to the project folder
+ Please activate ObjectDeliverer from Plugins after launching editor

# Quick Start
1. Create an ObjectDelivererManager
1. Create a DeliveryBox(If you wish to send and receive data other than binary)
1. Set the send / receive protocol and PacketRule, then start the ObjectDelivererManager

![gallery 1](https://user-images.githubusercontent.com/8191970/52522481-48075700-2cc9-11e9-92a0-067992f56042.png)



```cpp
void UMyClass::Start()
{
    auto deliverer = UObjectDelivererManager::CreateObjectDelivererManager();

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
    deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099),
                     UPacketRuleFactory::CreatePacketRuleSizeBody());
}

void UMyClass::OnConnect(UObjectDelivererProtocol* ClientSocket)
{
    // send data
    TArray<uint8> buffer;
    deliverer->Send(buffer);
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
```

# How to use each function
Look at the Wiki
https://github.com/ayumax/ObjectDeliverer/wiki
