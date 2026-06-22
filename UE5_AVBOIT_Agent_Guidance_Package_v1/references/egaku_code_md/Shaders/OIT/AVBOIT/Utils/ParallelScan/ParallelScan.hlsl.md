# Egaku source: `Shaders/OIT/AVBOIT/Utils/ParallelScan/ParallelScan.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef PARALLEL_SCAN_HLSL
#define PARALLEL_SCAN_HLSL

#ifndef PARALLEL_SCAN_BLOCK_THREAD_COUNT
#define PARALLEL_SCAN_BLOCK_THREAD_COUNT 32
#endif

#ifndef PARALLEL_SCAN_ELEM_TYPE 
#define PARALLEL_SCAN_ELEM_TYPE float
#endif

#define PARALLEL_SCAN_SHARED_MEM_ELEM_COUNT (2*PARALLEL_SCAN_BLOCK_THREAD_COUNT)
#define PARALLEL_SCAN_SHARED_MEM_ELEM_COUNT_CONFLICT_FREE (PARALLEL_SCAN_SHARED_MEM_ELEM_COUNT + PARALLEL_SCAN_SHARED_MEM_ELEM_COUNT/32)

groupshared PARALLEL_SCAN_ELEM_TYPE scan_tempBuf[PARALLEL_SCAN_SHARED_MEM_ELEM_COUNT_CONFLICT_FREE];

// interfaces to be defined by specific tasks.
PARALLEL_SCAN_ELEM_TYPE ParallelScan_GetElem(uint index);
void ParallelScan_Operate(PARALLEL_SCAN_ELEM_TYPE src,inout PARALLEL_SCAN_ELEM_TYPE dst);
void ParallelScan_SetZero(inout PARALLEL_SCAN_ELEM_TYPE t);
void ParallelScan_SetElem(uint index, PARALLEL_SCAN_ELEM_TYPE value);
void ParallelScan_Assign(PARALLEL_SCAN_ELEM_TYPE src,inout PARALLEL_SCAN_ELEM_TYPE dst);

uint ParallelScan_BlockThreadCount()
{
    return PARALLEL_SCAN_BLOCK_THREAD_COUNT;
}

uint ParallelScan_BlockSharedArrayLength()
{
    return PARALLEL_SCAN_BLOCK_THREAD_COUNT*2;
}

uint ParallelScan_ToLocalIndex(uint index)
{
    uint cnt = ParallelScan_BlockThreadCount();
    return index%cnt;
}

uint ParallelScan_GetBankConflictFreeIndex(uint virtualIndex)
{
    const uint bankCount = 32;
    const uint logBankCount = 5;
    return virtualIndex + (virtualIndex >> logBankCount);
}

PARALLEL_SCAN_ELEM_TYPE ParallelScan_ReadFromShared(uint virtualIndex)
{
    uint index = ParallelScan_GetBankConflictFreeIndex(virtualIndex);
    return scan_tempBuf[index];
}

void ParallelScan_WriteToShared(uint virtualIndex, PARALLEL_SCAN_ELEM_TYPE value)
{
    uint index = ParallelScan_GetBankConflictFreeIndex(virtualIndex);
    scan_tempBuf[index] = value;
}

void Sweep(uint localIndex)
{
    uint offset = 1;
    uint d;
    for (d = ParallelScan_BlockThreadCount(); d > 0; d >>= 1)
    {
        GroupMemoryBarrierWithGroupSync();
        if (localIndex < d)
        {
            uint _localIndexA = offset*(2*localIndex+1) - 1;
            uint _localIndexB = offset*(2*localIndex+2) - 1;
            PARALLEL_SCAN_ELEM_TYPE t = ParallelScan_ReadFromShared(_localIndexB);
            ParallelScan_Operate(ParallelScan_ReadFromShared(_localIndexA), t);
            ParallelScan_WriteToShared(_localIndexB, t);
        }
        offset <<= 1;
    }

    if (localIndex == 0){
        uint lastIndex = ParallelScan_BlockSharedArrayLength() - 1;
        PARALLEL_SCAN_ELEM_TYPE t = ParallelScan_ReadFromShared(lastIndex);
        ParallelScan_SetZero(t);
        ParallelScan_WriteToShared(lastIndex,t);
    }
    
    for (d = 1; d <= ParallelScan_BlockThreadCount(); d <<= 1)
    {
        offset >>= 1;
        GroupMemoryBarrierWithGroupSync();
        if (localIndex < d)
        {
            uint _localIndexA = offset*(2*localIndex+1) - 1;
            uint _localIndexB = offset*(2*localIndex+2) - 1;
            PARALLEL_SCAN_ELEM_TYPE a = ParallelScan_ReadFromShared(_localIndexA);
            PARALLEL_SCAN_ELEM_TYPE b = ParallelScan_ReadFromShared(_localIndexB);
            PARALLEL_SCAN_ELEM_TYPE t = a;
            ParallelScan_Assign(b,a);
            ParallelScan_Operate(t,b);
            ParallelScan_WriteToShared(_localIndexA, a);
            ParallelScan_WriteToShared(_localIndexB, b);
        }
    }
}

void ParallelScan_Exclusive_Block(uint index)
{
    uint localIndex = ParallelScan_ToLocalIndex(index);
    uint indexA = 2*index;
    uint indexB = 2*index+1;
    uint localIndexA = 2*localIndex;
    uint localIndexB = 2*localIndex+1;
    PARALLEL_SCAN_ELEM_TYPE localValA = ParallelScan_GetElem(indexA);
    PARALLEL_SCAN_ELEM_TYPE localValB = ParallelScan_GetElem(indexB);
    ParallelScan_WriteToShared(localIndexA, localValA);
    ParallelScan_WriteToShared(localIndexB, localValB);
    Sweep(localIndex);
    GroupMemoryBarrierWithGroupSync();
    ParallelScan_SetElem(indexA, ParallelScan_ReadFromShared(localIndexA));
    ParallelScan_SetElem(indexB, ParallelScan_ReadFromShared(localIndexB));
}



void ParallelScan_Inclusive_Block(uint index)
{
    uint localIndex = ParallelScan_ToLocalIndex(index);
    uint indexA = 2*index;
    uint indexB = 2*index+1;
    uint localIndexA = 2*localIndex;
    uint localIndexB = 2*localIndex+1;
    PARALLEL_SCAN_ELEM_TYPE localValA = ParallelScan_GetElem(indexA);
    PARALLEL_SCAN_ELEM_TYPE localValB = ParallelScan_GetElem(indexB);
    ParallelScan_WriteToShared(localIndexA, localValA);
    ParallelScan_WriteToShared(localIndexB, localValB);
    Sweep(localIndex);
    GroupMemoryBarrierWithGroupSync();
    PARALLEL_SCAN_ELEM_TYPE a = ParallelScan_ReadFromShared(localIndexA);
    PARALLEL_SCAN_ELEM_TYPE b = ParallelScan_ReadFromShared(localIndexB);
    ParallelScan_Operate(localValA,a);
    ParallelScan_Operate(localValB,b);
    ParallelScan_SetElem(indexA, a);
    ParallelScan_SetElem(indexB, b);
}

void ParallelScan_Inclusive(uint globalThreadId)
{
    ParallelScan_Inclusive_Block(globalThreadId);
}



#endif
```
