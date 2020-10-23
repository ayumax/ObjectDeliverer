// Copyright 2019 ayumax. All Rights Reserved.
#include "ODGrowBuffer.h"

ODGrowBuffer::ODGrowBuffer(int32 initialSize /*= 1024*/, int32 packetSize /*= 1024*/)
    : currentSize(0)
{
    this->packetSize = packetSize;
    SetLength(initialSize);
}

int32 ODGrowBuffer::GetLength() const
{
    return currentSize;
}

int32 ODGrowBuffer::GetInnerBufferSize() const
{
    return innerBuffer.Num();
}

ODByteSpan ODGrowBuffer::AsSpan()
{
    return AsSpan(0, GetLength());
}

ODByteSpan ODGrowBuffer::AsSpan(int32 Position, int32 Length)
{
    ODByteSpan stSpan;
    stSpan.Buffer = innerBuffer.GetData() + Position;
    stSpan.Length = Length;
    return stSpan;
}

bool ODGrowBuffer::SetLength(int32 NewSize /*= 0*/)
{
    bool isGrow = false;

    if (innerBuffer.Num() < NewSize)
    {
        auto packetCount = NewSize / packetSize;
        if (NewSize % packetSize)
        {
            ++packetCount;
        }
        innerBuffer.SetNum(packetSize * packetCount);

        isGrow = true;
    }

    currentSize = NewSize;

    return isGrow;
}

void ODGrowBuffer::Add(ODByteSpan addBuffer)
{
    SetLength(GetLength() + addBuffer.Length);

    AsSpan(GetLength() - addBuffer.Length, addBuffer.Length).CopyFrom(addBuffer);
}

void ODGrowBuffer::CopyFrom(ODByteSpan fromBuffer, int32 myOffset /*= 0*/)
{
    auto newSize = FMath::Max(GetLength(), fromBuffer.Length + myOffset);
    SetLength(newSize);

    ODByteSpan spanBuffer = AsSpan(myOffset, fromBuffer.Length);

    spanBuffer.CopyFrom(fromBuffer);
}

void ODGrowBuffer::RemoveRangeFromStart(int32 start, int32 length)
{
    auto moveLength = GetLength() - (start + length);
    TArray<uint8> tempBuffer = TArray<uint8>();
    tempBuffer.SetNum(moveLength);
    auto moveSpan = AsSpan(start + length, moveLength);
    ODByteSpan(tempBuffer).CopyFrom(moveSpan);
    AsSpan(start, moveLength).CopyFrom(tempBuffer);

    currentSize = moveLength;
}

void ODGrowBuffer::Clear()
{
    currentSize = 0;
}