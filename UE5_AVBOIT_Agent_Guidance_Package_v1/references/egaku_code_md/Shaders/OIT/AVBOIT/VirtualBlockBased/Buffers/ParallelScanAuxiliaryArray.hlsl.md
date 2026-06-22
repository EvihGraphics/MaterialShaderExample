# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Buffers/ParallelScanAuxiliaryArray.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef PARALLEL_SCAN_AUXILIARY_ARRAY_HLSL
#define PARALLEL_SCAN_AUXILIARY_ARRAY_HLSL

#define PARALLEL_SCAN_AUXILIARY_ARRAY_ELEM_TYPE float
#include "../../Utils/ParallelScan/AuxiliaryArray.hlsl"

RWStructuredBuffer<float> _AVBOIT_ParallelScanAuxiliaryArray;

void ParallelScan_WriteToAuxiliaryArray(uint index, PARALLEL_SCAN_AUXILIARY_ARRAY_ELEM_TYPE value)
{
    _AVBOIT_ParallelScanAuxiliaryArray[index] = value;
}

uint ParallelScan_GetAuxiliaryArrayLength()
{
    uint x,y;
    _AVBOIT_ParallelScanAuxiliaryArray.GetDimensions(x,y);
    return x;
}

PARALLEL_SCAN_AUXILIARY_ARRAY_ELEM_TYPE ParallelScan_ReadFromAuxiliaryArray(uint index)
{
     return _AVBOIT_ParallelScanAuxiliaryArray[index];    
}

#endif
```
