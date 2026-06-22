# Egaku source: `Shaders/Mesh/CBT/CBTEval.compute`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#include "CBTMacroDefs.hlsl"

StructuredBuffer<uint> _CBTBuffer;
#define CBT_BUFFER_NAME _CBTBuffer

#include "CBTEval.hlsl"
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/CBuffer.hlsl"

RWStructuredBuffer<CBTPackedEvalResult> _CBTEvalResultBuffer;
RWStructuredBuffer<CBTEvalResultMeta> _CBTEvalResultMetaBuffer;


CBUFFER_BEGIN(CBTEvalContextConstants)
    float4x4 _Model;
    float4x4 _ModelView;
    float4 _CameraFrustumPlanes[6];
    float _TargetLod;
    float3 _paddings;
CBUFFER_END

bool ShouldDiscard(CBTEvalContext context)
{
    if (context.nodeID >= cbt_NodeCount()){
        return true;
    }
    if (ShouldCull(context)){
        return true;
    }
    return false;
}


#pragma kernel CBTEvalNodesKernel_EvalSplit
[numthreads(CBT_UPDATE_GROUP_SIZE_DECL)]
void CBTEvalNodesKernel_EvalSplit(uint threadID : SV_DispatchThreadID)
{
    CBTEvalContext context;
    context.nodeID = threadID;
    context.model = _Model;
    context.modelView = _ModelView;
    context.frustumPlanes = _CameraFrustumPlanes;
    /*context.frustumPlanes[1] = _CameraFrustumPlanes[1];
    context.frustumPlanes[2] = _CameraFrustumPlanes[2];
    context.frustumPlanes[3] = _CameraFrustumPlanes[3];
    context.frustumPlanes[4] = _CameraFrustumPlanes[4];
    context.frustumPlanes[5] = _CameraFrustumPlanes[5];*/
    context.targetLod = _TargetLod;
    if (ShouldDiscard(context))
    {
        return;
    }

    bool shouldSplit = EvalNodeTriangleShouldSplit(context);
    CBTEvalResult result;
    result.shouldUpdate = shouldSplit;
    result.nodeID = context.nodeID;
    CBTPackedEvalResult packedResult = PackEvalResult(result);
    uint bufferIndex;
    InterlockedAdd(_CBTEvalResultMetaBuffer[0].resultCount,1,bufferIndex);
    _CBTEvalResultBuffer[bufferIndex] = packedResult;
}

#pragma kernel CBTEvalNodesKernel_EvalMerge
[numthreads(CBT_UPDATE_GROUP_SIZE_DECL)]
void CBTEvalNodesKernel_EvalMerge(uint threadID : SV_DispatchThreadID)
{
    CBTEvalContext context;
    context.nodeID = threadID;
    context.model = _Model;
    context.modelView = _ModelView;
    context.frustumPlanes = _CameraFrustumPlanes;
    context.targetLod = _TargetLod;
    if (ShouldDiscard(context))
    {
        return;
    }

    bool shouldMerge = EvalNodeTriangleShouldMerge(context);
    CBTEvalResult result;
    result.shouldUpdate = shouldMerge;
    result.nodeID = context.nodeID;
    CBTPackedEvalResult packedResult = PackEvalResult(result);
    uint bufferIndex;
    InterlockedAdd(_CBTEvalResultMetaBuffer[0].resultCount,1,bufferIndex);
    _CBTEvalResultBuffer[bufferIndex] = packedResult;
}
```
