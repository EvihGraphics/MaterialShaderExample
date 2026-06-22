# Egaku source: `Shaders/Mesh/CBT/CBTIndirect.compute`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿
#pragma enable_d3d11_debug_symbols

StructuredBuffer<uint> _CBTBuffer;
#define CBT_BUFFER_NAME _CBTBuffer
#include "CBTIndirect.hlsl"
#include "CBTHelperStructs.hlsl"

StructuredBuffer<CBTPackedEvalResult> _CBTEvalResultBuffer;
RWStructuredBuffer<CBTEvalResultMeta> _CBTEvalResultMetaBuffer;
RWStructuredBuffer<DrawIndirectArgs> _DrawIndirectBuffer;
RWStructuredBuffer<DispatchIndirectArgs> _DispatchUpdateIndirectBuffer;
RWStructuredBuffer<DispatchIndirectArgs> _DispatchEvalIndirectBuffer;



#pragma kernel CBTUpdateIndirectKernel
[numthreads(1, 1, 1)]
void CBTUpdateIndirectKernel()
{
    uint evalCount = _CBTEvalResultMetaBuffer[0].resultCount;
    DrawIndirectArgs indirectArgs = GetMeshDrawIndirectArgs(_DrawIndirectBuffer[0], evalCount);
    DispatchIndirectArgs dispatchArgs = GetCBTUpdateDispatchIndirectArgs(evalCount);
    _DrawIndirectBuffer[0] = indirectArgs;
    _DispatchUpdateIndirectBuffer[0] = dispatchArgs;
    _DispatchEvalIndirectBuffer[0] = GetCBTEvalDispatchIndirectArgs();
    _CBTEvalResultMetaBuffer[0].resultCount = 0;
}
```
