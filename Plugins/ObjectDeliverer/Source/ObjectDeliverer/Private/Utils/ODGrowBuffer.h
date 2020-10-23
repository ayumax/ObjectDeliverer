// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct ODByteSpan
{
    uint8* Buffer;
    int32 Length;

    ODByteSpan(){}
    ODByteSpan(uint8* RawBuffer, int32 RawBufferLength)
    {
        Buffer = RawBuffer;
        Length = RawBufferLength;
    }
    ODByteSpan(TArray<uint8>& FromBuffer)
    {
        Buffer = FromBuffer.GetData();
        Length = FromBuffer.Num();
    }

    void CopyFrom(const ODByteSpan& FromBuffer)
    {
        if (Length < FromBuffer.Length)
        {
            checkf(Length >= FromBuffer.Length, TEXT("ODByteSpan::CopyFrom SizeOver!"));
        }
        FMemory::Memcpy((void*)Buffer, (const void*)FromBuffer.Buffer, FromBuffer.Length);
    }

    TArray<uint8> ToArray() const
    {
        return TArray<uint8>(Buffer, Length);
    }
};

class ODGrowBuffer
{
public:
    ODGrowBuffer(int32 initialSize = 1024, int32 packetSize = 1024);

    uint8 operator [](int32 index)
    {
        return innerBuffer[index];
    }

    int32 GetLength() const;
    int32 GetInnerBufferSize() const;

    ODByteSpan AsSpan();
    ODByteSpan AsSpan(int32 Position, int32 Length);

    bool SetLength(int32 NewSize = 0);

    void Add(ODByteSpan addBuffer);

    void CopyFrom(ODByteSpan fromBuffer, int32 myOffset = 0);
    void RemoveRangeFromStart(int32 start, int32 length);
    void Clear();
    
private:
    int32 packetSize = 1024;
    TArray<uint8> innerBuffer;
    int32 currentSize;
};
