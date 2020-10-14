// Copyright 2019 ayumax. All Rights Reserved.
#include "ODGrowBuffer.h"

ODGrowBuffer::ODGrowBuffer(int initialSize /*= 1024*/, int packetSize /*= 1024*/)
    : bufferSize(0)
{
    this->packetSize = packetSize;
    SetBufferSize(initialSize);
}

int ODGrowBuffer::GetLength() const
{
    return bufferSize;
}

int ODGrowBuffer::GetInnerBufferSize() const
{
    return innerBuffer.Num();
}

ODByteSpan ODGrowBuffer::AsSpan()
{
    return AsSpan(0, GetLength());
}

ODByteSpan ODGrowBuffer::AsSpan(int Position, int Length)
{
    ODByteSpan stSpan;
    stSpan.Buffer = innerBuffer.GetData() + Position;
    stSpan.Length = Length;
    return stSpan;
}

bool ODGrowBuffer::SetBufferSize(int NewSize /*= 0*/)
{
    bool isGrow = false;

    if (this.innerBuffer.Num() < NewSize)
    {
        this.innerBuffer.SetNum(packetSize * (NewSize / packetSize + 1));

        isGrow = true;
    }

    bufferSize = NewSize;

    return isGrow;
}

void ODGrowBuffer::Add(ODByteSpan addBuffer)
{
    SetBufferSize(GetLength() + addBuffer.Length);

    AsSpan(GetLength() - addBuffer.Length, addBuffer.Length).CopyFrom(addBuffer);
}

void ODGrowBuffer::CopyFrom(ODByteSpan fromBuffer, int myOffset /*= 0*/)
{
    ODByteSpan& spanBuffer = AsSpan(myOffset, FMath::Min(fromBuffer.Length, GetLength() - myOffset));

    spanBuffer.CopyFrom(fromBuffer);
}

void ODGrowBuffer::RemoveRangeFromStart(int start, int length)
{
    auto moveLength = GetLength() - length;
    TArray<uint8> tempBuffer = TArray<uint8>();
    tempBuffer.SetNum(moveLength);
    auto moveSpan = AsSpan(start + length, moveLength);
    ODByteSpan(tempBuffer).CopyFrom(moveSpan);
    AsSpan(start, moveLength).CopyFrom(tempBuffer);

    bufferSize = moveLength;
}

void ODGrowBuffer::Clear()
{
    innerBuffer.Reset();
    bufferSize = 0;
}