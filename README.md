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
+ Shared Memory(Windows Only)
+ LogFile Writer
+ LogFile Reader

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
1. Create an ObjectDelivererManager
1. Create a DeliveryBox(If you wish to send and receive data other than binary)
1. Set the send / receive protocol and PacketRule, then start the ObjectDelivererManager

![gallery 1](https://user-images.githubusercontent.com/8191970/52522069-f9a38980-2cc3-11e9-943b-2014184e6e1e.png)

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

# Change of communication protocol
You can switch to a different communication protocol by replacing the transmission / reception protocol set in ObjectDelivererManager.

![gallery 2](https://user-images.githubusercontent.com/8191970/52522072-00320100-2cc4-11e9-862e-885e0ecc576d.png)

```cpp
// TCP/IP Server
deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099),
                 UPacketRuleFactory::CreatePacketRuleSizeBody());

// TCP/IP Client
deliverer->Start(UProtocolFactory::CreateProtocolTcpIpClient("localhost", 9099, true),
                 UPacketRuleFactory::CreatePacketRuleSizeBody());

// UDP Sender
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketSender("localhost", 9099),
                 UPacketRuleFactory::CreatePacketRuleSizeBody());

// UDP Receiver
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099),
                 UPacketRuleFactory::CreatePacketRuleSizeBody());

// Shared Memory
deliverer->Start(UProtocolFactory::CreateProtocolSharedMemory("memory_test", 1024),
                 UPacketRuleFactory::CreatePacketRuleSizeBody());
		 
// LogFile Writer
deliverer->Start(UProtocolFactory::CreateProtocolLogWriter("log.bin", false),
                 UPacketRuleFactory::CreatePacketRuleSizeBody());	
		 
// LogFile Reader
deliverer->Start(UProtocolFactory::CreateProtocolLogReader("log.bin", false, true),
                 UPacketRuleFactory::CreatePacketRuleSizeBody());		 
```


# Change of data division rule
You can easily change the packet splitting rule.

![gallery 3](https://user-images.githubusercontent.com/8191970/52522075-04f6b500-2cc4-11e9-832d-3c684bdcac74.png)
```cpp
// FixedSize
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099),
                 UPacketRuleFactory::CreatePacketRuleFixedLength(1024));

// Header(BodySize) + Body
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099),
                 UPacketRuleFactory::CreatePacketRuleSizeBody(4, ECNBufferEndian::Big));

// Split by terminal symbol
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099),
                 UPacketRuleFactory::CreatePacketRuleTerminate({ TEXT('\r'), TEXT('\n') }));

// Nodivision
deliverer->Start(UProtocolFactory::CreateProtocolUdpSocketReceiver(9099),
                 UPacketRuleFactory::CreatePacketRuleNodivision());
```

# Change of Serialization method
Using DeliveryBox enables sending and receiving of non-binary data (character strings and objects).

![gallery 4](https://user-images.githubusercontent.com/8191970/52522078-088a3c00-2cc4-11e9-90f5-9cc29fe938e4.png)

```cpp
// UTF-8 string
auto deliverybox = UDeliveryBoxFactory::CreateUtf8StringDeliveryBox();
deliverybox->Received.AddDynamic(this, &UMyClass::OnReceiveString);
deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099),
                 UPacketRuleFactory::CreatePacketRuleSizeBody(),
                 deliverybox);

deliverybox->Send(TEXT("ABCDEFG"));

void UMyClass::OnReceiveString(FString ReceivedString)
{
    // received data string
}
```

```cpp
// Object(Json)
auto deliverybox = UDeliveryBoxFactory::CreateObjectDeliveryBoxUsingJson>(SampleObject::StaticClass());
deliverybox->Received.AddDynamic(this, &UMyClass::OnReceiveObject);
deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099),
                 UPacketRuleFactory::CreatePacketRuleSizeBody(),
                 deliverybox);

auto obj = NewObject<SampleObject>();
deliverybox->Send(obj);

void UMyClass::OnReceiveObject(UObject* ReceivedObject)
{
    // received data object
    USampleObject* obj = Cast<USampleObject>(ReceivedObject);
}
```
