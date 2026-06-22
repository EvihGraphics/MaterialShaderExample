# Egaku source: `Shaders/Mesh/CBT/CBTSumReduction.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CBT_SUM_REDUCTION_HLSL
#define CBT_SUM_REDUCTION_HLSL

#define CBT_FLAG_WRITE
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Utilities/CBT/ConcurrentBinaryTree.hlsl"


void CBTCalcReducedSum(uint depth,uint nodeID)
{
    uint x0 = cbt_HeapRead(cbt_CreateNode(nodeID << 1u , depth + 1));
    uint x1 = cbt_HeapRead(cbt_CreateNode(nodeID << 1u | 1u, depth + 1));
    cbt__HeapWrite(cbt_CreateNode(nodeID, depth), x0 + x1);
}


// batch calculation for first five depth. 
void CBTCalcReducedSumFirstFiveDepth(uint depth,uint nodeID)
{
    uint nodeCnt = (1u<<depth);
    uint alignedBitOffset = cbt__NodeBitID(cbt_CreateNode(nodeID, depth));
    uint bitField = GetCBTBuffer()[alignedBitOffset >> 5u];
    uint bitData;
    // 2-bits
    bitField = (bitField & 0x55555555u) + ((bitField >> 1u) & 0x55555555u);
    bitData = bitField;

    GetCBTBuffer()[(alignedBitOffset - nodeCnt) >> 5u] = bitData;

    // 3-bits
    bitField = (bitField & 0x33333333u) + ((bitField >>  2u) & 0x33333333u);
    bitData = ((bitField >> 0u) & (7u <<  0u))
        | ((bitField >> 1u) & (7u <<  3u))
        | ((bitField >> 2u) & (7u <<  6u))
        | ((bitField >> 3u) & (7u <<  9u))
        | ((bitField >> 4u) & (7u << 12u))
        | ((bitField >> 5u) & (7u << 15u))
        | ((bitField >> 6u) & (7u << 18u))
        | ((bitField >> 7u) & (7u << 21u));
    cbt__HeapWriteExplicit(cbt_CreateNode(nodeID >> 2u, depth - 2), 24, bitData);

    // 4-bits
    bitField = (bitField & 0x0F0F0F0Fu) + ((bitField >>  4u) & 0x0F0F0F0Fu);
    bitData = ((bitField >>  0u) & (15u <<  0u))
        | ((bitField >>  4u) & (15u <<  4u))
        | ((bitField >>  8u) & (15u <<  8u))
        | ((bitField >> 12u) & (15u << 12u));
    cbt__HeapWriteExplicit(cbt_CreateNode(nodeID >> 3u, depth - 3), 16, bitData);

    // 5-bits
    bitField = (bitField & 0x00FF00FFu) + ((bitField >>  8u) & 0x00FF00FFu);
    bitData = ((bitField >>  0u) & (31u << 0u))
        | ((bitField >> 11u) & (31u << 5u));
    cbt__HeapWriteExplicit(cbt_CreateNode(nodeID >> 4u, depth - 4), 10, bitData);

    // 6-bits
    bitField = (bitField & 0x0000FFFFu) + ((bitField >> 16u) & 0x0000FFFFu);
    bitData = bitField;
    cbt__HeapWriteExplicit(cbt_CreateNode(nodeID >> 5u, depth - 5),  6, bitData);
}

#endif
```
