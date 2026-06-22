# Egaku source: `Shaders/Mesh/CBT/CBTCulling.compute`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#include "CBTMacroDefs.hlsl"
#define CBT_BUFFER_NAME _CBTBuffer
#include "CBTCulling.hlsl"
#include "CBTEval.hlsl"
#pragma enable_d3d11_debug_symbols

StructuredBuffer<uint> _CBTBuffer;
AppendStructuredBuffer<uint> _CBTCullingResult;
int _CBTNodeDepth;



/*
CBTCullingContext _CBTCullingContext;

#pragma kernel CBTCullingKernel
[numthreads(CBT_CULLING_GROUP_SIZE)]
void CBTCullingKernel(uint threadID : SV_DispatchThreadID)
{
    uint nodeID = threadID;
    if (ShouldCull(_CBTCullingContext, nodeID) == false)
    {
        _CBTCullingResult.Append(nodeID);
    }
}*/
```
