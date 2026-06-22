# Egaku source: `Shaders/Mesh/CBT/CBTSumReduction.compute`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿
#pragma enable_d3d11_debug_symbols
#include "CBTMacroDefs.hlsl"
RWStructuredBuffer<uint> _CBTBuffer;
int _CBTNodeDepth;

#define CBT_BUFFER_NAME _CBTBuffer
#include "CBTSumReduction.hlsl"

// count of sibling nodes in a binary tree at a given depth
uint GetSiblingNodeCount(int depth)
{
    return 1u<<depth;
}

#pragma kernel CBTSumReductionKernel
[numthreads(CBT_SUM_REDUCTION_GROUP_SIZE_DECL)]
void CBTSumReductionKernel (uint threadID : SV_DispatchThreadID)
{
    uint currentNodeOffsetInSiblings = threadID;
    uint siblingsCount = GetSiblingNodeCount(_CBTNodeDepth);
    if (currentNodeOffsetInSiblings < siblingsCount)
    {
        // node count of all small depth (depth < cbtNodeDepth) is just equal to siblingsCount. 
        uint nodeID = currentNodeOffsetInSiblings + siblingsCount;
        CBTCalcReducedSum(_CBTNodeDepth, nodeID);
    }
}


#pragma kernel CBTSumReductionKernel_Prepass
[numthreads(CBT_SUM_REDUCTION_GROUP_SIZE_DECL)]
void CBTSumReductionKernel_Prepass(uint threadID : SV_DispatchThreadID)
{
    // Update for first five depth is batched into 32 bits per reduced-sum calculation.
    // So every 32 nodes needs to be passed as parameter.
    uint currentNodeOffsetInSiblings = threadID << 5;
    
    uint siblingsCount = GetSiblingNodeCount(_CBTNodeDepth);
    if (currentNodeOffsetInSiblings < siblingsCount)
    {
        uint nodeID = currentNodeOffsetInSiblings + siblingsCount; 
        CBTCalcReducedSumFirstFiveDepth(_CBTNodeDepth, nodeID);
    }
}
```
