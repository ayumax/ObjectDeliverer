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
    ODGrowBuffer(int initialSize = 1024, int packetSize = 1024);

    int GetLength() const;
    int GetInnerBufferSize() const;

    ODByteSpan AsSpan();
    ODByteSpan AsSpan(int Position, int Length);

    bool SetBufferSize(int NewSize = 0);

    void Add(ODByteSpan addBuffer);

    void CopyFrom(ODByteSpan fromBuffer, int myOffset = 0);
    void RemoveRangeFromStart(int start, int length);
    void Clear();
    
private:
    int packetSize = 1024;
    TArray<uint8> innerBuffer;

    int bufferSize;
    int currentSize;
};
