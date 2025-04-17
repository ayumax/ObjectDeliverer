# ObjectDeliverer

| Branch | Status |
|---------|-----------|
| master  | ![workflow](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml/badge.svg?branch=main) |
| UE5.5   | ![workflow](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml/badge.svg?branch=UE5.5)|
| UE5.4   | ![workflow](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml/badge.svg?branch=UE5.4) |
| UE5.3   | ![workflow](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml/badge.svg?branch=UE5.3) |

ObjectDeliverer is a flexible data communication library for Unreal Engine. It's available for both C++ and Blueprint.

## Table of Contents
- [Overview](#overview)
- [Supported UE Versions](#supported-ue-versions)
- [How to Obtain](#how-to-obtain)
- [Installation Guide](#installation-guide)
- [Quick Start](#quick-start)
- [Feature Details](#feature-details)
  - [Communication Protocols](#communication-protocols)
  - [Data Division Rules](#data-division-rules)
  - [Serialization Methods](#serialization-methods)
- [Usage Examples](#usage-examples)
  - [C++ Example](#c-example)
  - [Blueprint Example](#blueprint-example)
- [Detailed Documentation](#detailed-documentation)
- [License](#license)
- [Contributing](#contributing)

## Overview

ObjectDeliverer excels as a data communication library with these key advantages:

- **Easy Protocol Switching** - Seamlessly switch between TCP/IP, UDP, shared memory, and other protocols
- **Flexible Data Division Rules** - Support for fixed size, header+body, terminal symbol, and more
- **Various Serialization Methods** - Handle byte arrays, UTF-8 strings, JSON objects, and more
- **C++ and Blueprint Support** - Use your preferred development method

It simplifies network communication logic, allowing you to focus on application development.

## Supported UE Versions

| UE Version | Support Status | Branch Name |
|------------|---------------|------------|
| UE 5.5     | ✅ Supported  | UE5.5      |
| UE 5.4     | ✅ Supported  | UE5.4      |
| UE 5.3     | ✅ Supported  | UE5.3      |
| Others     | 🔄 Check docs | master     |

### Branch Structure

```
master (latest UE version) ──┐
                           ├─ UE5.5 (UE5.5 specific)
                           ├─ UE5.4 (UE5.4 specific)
                           └─ UE5.3 (UE5.3 specific)
```

- **master** branch: Always compatible with the latest Unreal Engine version
- **UEX.X** branches: Stable versions for specific UE versions
- **Note**: Older version branches may not include the latest features

## How to Obtain

### UE Marketplace

[https://www.fab.com/ja/listings/b6ffd7d7-80da-483f-a7fa-09cb46b72651](https://www.fab.com/ja/listings/b6ffd7d7-80da-483f-a7fa-09cb46b72651)

If you acquire this plugin through the Marketplace, you will be charged the specified fee.

### GitHub

You can clone this repository and use it for free.

## Installation Guide

### From Marketplace
1. Purchase and download the plugin from the Marketplace
2. Add it to your project from the "Library" section in the UE Launcher
3. Open your project and enable ObjectDeliverer from "Edit" → "Plugins"

### From GitHub
1. Clone this repository: `git clone https://github.com/ayumax/ObjectDeliverer.git`
2. Copy the `Plugins` directory from the cloned repository to your project folder
3. Build the plugin (requires C++ build environment)
4. Open your project and enable ObjectDeliverer from "Edit" → "Plugins"

### Compatibility Check
Choose the appropriate branch based on your UE version:
```bash
# Example for UE5.4
git checkout UE5.4
```

## Quick Start

### Basic Usage Steps
1. Create an ObjectDelivererManager
2. Set up event handlers (connect, disconnect, data receive)
3. Configure communication protocol and packet rule, then start

![Setup Example](https://user-images.githubusercontent.com/8191970/52522481-48075700-2cc9-11e9-92a0-067992f56042.png)

## Feature Details

### Communication Protocols
The following protocols are available by default (you can also add your own):

- **TCP/IP Server** - Connects to multiple clients
- **TCP/IP Client** - Connects to a server
- **UDP (Sender)** - Sends UDP data
- **UDP (Receiver)** - Receives UDP data
- **Shared Memory** - Process-to-process communication in Windows
- **File Writer** - Saves data to a file
- **File Reader** - Reads data from a file

### Data Division Rules
Rules for appropriate packet division and reconstruction of received data:

#### FixedSize
Example) When using a fixed size of 1024 bytes
![Fixed Length](https://user-images.githubusercontent.com/8191970/56475737-7d999f00-64c7-11e9-8e9e-0182f1af8156.png)

#### Header(BodySize) + Body
Example) When the size area is 4 bytes
![Size and Body](https://user-images.githubusercontent.com/8191970/56475796-6e672100-64c8-11e9-8cf0-6524f2899be0.png)

#### Split by terminal symbol
Example) When 0x00 is the end
![Terminate](https://user-images.githubusercontent.com/8191970/56475740-82f6e980-64c7-11e9-91a6-05d77cfdbd60.png)

#### No division
Uses the received buffer as-is without any packet splitting or combining operations.

### Serialization Methods
- **Byte Array** - Raw binary data
- **UTF-8 String** - Text data
- **Object (JSON)** - Structured data

## Usage Examples

### C++ Example

```cpp
void UMyClass::Start()
{
    auto deliverer = UObjectDelivererManager::CreateObjectDelivererManager();

    // Set up event handlers
    deliverer->Connected.AddDynamic(this, &UMyClass::OnConnect);
    deliverer->Disconnected.AddDynamic(this, &UMyClass::OnDisConnect);
    deliverer->ReceiveData.AddDynamic(this, &UMyClass::OnReceive);

    // Start communication
    // Protocol: TCP/IP Server
    // Data division rule: Header(Size) + Body
    // Serialization method: Byte array
    deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099),
                     UPacketRuleFactory::CreatePacketRuleSizeBody());
}

void UMyClass::OnConnect(UObjectDelivererProtocol* ClientSocket)
{
    // Send data
    TArray<uint8> buffer;
    deliverer->Send(buffer);
}

void UMyClass::OnDisConnect(UObjectDelivererProtocol* ClientSocket)
{
    // Handle disconnection
    UE_LOG(LogTemp, Log, TEXT("closed"));
}

void UMyClass::OnReceive(UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
{
    // Handle received data
}
```

### Blueprint Example

![Blueprint Example](https://user-images.githubusercontent.com/8191970/52522481-48075700-2cc9-11e9-92a0-067992f56042.png)

## Detailed Documentation

For detailed usage of each feature, please refer to the Wiki:

[ObjectDeliverer Wiki](https://github.com/ayumax/ObjectDeliverer/wiki)

## License

- This plugin is provided under the MIT License
- However, if you download and use it from the Epic Games Marketplace, the Epic Games license terms will apply

## Contributing

We welcome contributions from everyone who wants to improve ObjectDeliverer!

### Contribution Process

1. Fork the repository
2. Create your feature branch from the `master` branch
3. Make your changes
4. Submit a pull request targeting the `master` branch

### Contribution Guidelines

- **All pull requests** should be directed to the `master` branch
- For major changes, please open an issue first to discuss what you would like to change(English or Japanese)
- Follow the code style of the existing codebase
- Add or update tests when possible

### UE Version Support

When adding new features, please try to make them work across multiple UE versions when possible. If a feature is specific to a particular UE version, please note this clearly.

We appreciate all contributions - bug fixes, feature additions, documentation improvements, suggestions, and more!
