# Egaku source: `Shaders/OIT/AVBOIT/Utils/ParallelScan/AuxiliaryArray.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef AUXILIARY_ARRAY_HLSL
#define AUXILIARY_ARRAY_HLSL

#ifndef PARALLEL_SCAN_AUXILIARY_ARRAY_ELEM_TYPE 
#define PARALLEL_SCAN_AUXILIARY_ARRAY_ELEM_TYPE float
#endif

groupshared PARALLEL_SCAN_ELEM_TYPE scan_PreviousBlockSum;

PARALLEL_SCAN_AUXILIARY_ARRAY_ELEM_TYPE  ParallelScan_ReadFromAuxiliaryArray(uint index);
void ParallelScan_WriteToAuxiliaryArray(uint index, PARALLEL_SCAN_AUXILIARY_ARRAY_ELEM_TYPE value);
uint ParallelScan_GetAuxiliaryArrayLength();

void ParallelScan_LoadPreviousBlockSum(uint index)
{
    scan_PreviousBlockSum = ParallelScan_ReadFromAuxiliaryArray(index - 1);
}

PARALLEL_SCAN_AUXILIARY_ARRAY_ELEM_TYPE ParallelScan_GetPreviousBlockSum()
{
    return scan_PreviousBlockSum;
}



void ParallelScan_InitAuxiliaryArray()
{
    uint len = ParallelScan_GetAuxiliaryArrayLength();
    PARALLEL_SCAN_AUXILIARY_ARRAY_ELEM_TYPE lastSum = 0;
    for (uint i = 0; i < len; i++)
    {
        PARALLEL_SCAN_AUXILIARY_ARRAY_ELEM_TYPE currSum = lastSum + ParallelScan_ReadFromAuxiliaryArray(i);
        ParallelScan_WriteToAuxiliaryArray(i, currSum);
        lastSum = currSum;
    }
}

#endif
```
