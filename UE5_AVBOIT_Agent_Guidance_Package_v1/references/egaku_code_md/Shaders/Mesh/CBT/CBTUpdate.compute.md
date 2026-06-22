# Egaku source: `Shaders/Mesh/CBT/CBTUpdate.compute`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿
#pragma enable_d3d11_debug_symbols
#include "CBTMacroDefs.hlsl"

RWStructuredBuffer<uint> _CBTBuffer;
#define CBT_BUFFER_NAME _CBTBuffer
#include "CBTUpdate.hlsl"
#include "CBTHelperStructs.hlsl"

StructuredBuffer<CBTPackedEvalResult> _CBTEvalResultBuffer;

/*
CBUFFER_BEGIN(CBTUpdateContextConstants)
    float4x4 _ModelView;
    float4 _CameraFrustumPlanes[6];
    float _TargetLod;
    float3 _paddings;
CBUFFER_END
*/



#pragma kernel CBTUpdateKernel_Split
[numthreads(CBT_UPDATE_GROUP_SIZE_DECL)]
void CBTUpdateKernel_Split (uint threadID : SV_DispatchThreadID)
{
    CBTEvalResult result = UnpackEvalResult(_CBTEvalResultBuffer[threadID]);
    if (result.shouldUpdate)
    {
        CBTSplitNode(result.nodeID);
    }
    /*uint nodeID = _CBTCullingResult.Consume();
    if (nodeID >= cbt_NodeCount()){
        return;
    }
    CBTUpdateContext context;
    context.frustumPlanes = _CameraFrustumPlanes;
    context.modelView = _ModelView;
    context.targetLod = _TargetLod;
    CBTSplitNode(context,nodeID);*/
}


#pragma kernel CBTUpdateKernel_Merge
[numthreads(CBT_UPDATE_GROUP_SIZE_DECL)]
void CBTUpdateKernel_Merge (uint threadID : SV_DispatchThreadID)
{
    CBTEvalResult result = UnpackEvalResult(_CBTEvalResultBuffer[threadID]);
    if (result.shouldUpdate)
    {
        CBTMergeNode(result.nodeID);
    }
    /*uint nodeID = threadID;
    if (nodeID >= cbt_NodeCount()){
        return;
    }
    CBTUpdateContext context;
    context.frustumPlanes = _CameraFrustumPlanes;
    context.modelView = _ModelView;
    context.targetLod = _TargetLod;
    CBTMergeNode(context,nodeID);*/
}
```
