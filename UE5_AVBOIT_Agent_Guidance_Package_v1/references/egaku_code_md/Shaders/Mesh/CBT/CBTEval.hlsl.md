# Egaku source: `Shaders/Mesh/CBT/CBTEval.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CBT_EVAL_HLSL
#define CBT_EVAL_HLSL

#include "CBTMeshUtils.hlsl"
#include "CBTHelperStructs.hlsl"
#include "CBTCulling.hlsl"

const int CBT_EVAL_OPERATION_MERGE = -1;
const int CBT_EVAL_OPERATION_NONE = 0;
const int CBT_EVAL_OPERATION_SPLIT = 1;


bool ShouldCull(CBTEvalContext context)
{
    cbt_Node node = cbt_DecodeNode(context.nodeID);
    float3x4 triangleVertices = DecodeTriangleVertices(node);
    triangleVertices[0] = mul(context.modelView,triangleVertices[0]);
    triangleVertices[1] = mul(context.modelView,triangleVertices[1]);
    triangleVertices[2] = mul(context.modelView,triangleVertices[2]);
    return FrustumCullingTest(context.frustumPlanes,triangleVertices,5) == false;
}


bool TestShouldMerge(float3x4 patchVertices,float targetLod)
{
    float lod = TriangleLevelOfDetail_Perspective(patchVertices,targetLod);
    return lod < 0.9;
}

bool TestShouldSplit(float3x4 patchVertices,float targetLod)
{
    float lod = TriangleLevelOfDetail_Perspective(patchVertices,targetLod);
    return lod > 1;
}


bool EvalNodeTriangleShouldSplit(cbt_Node node,float4x4 modelView,float targetLod)
{
    float3x4 patchVertices = DecodeTriangleVertices(node);
    patchVertices[0] = mul(modelView,patchVertices[0]); // patchVertex[1] is not used
    patchVertices[2] = mul(modelView,patchVertices[2]);
    return TestShouldSplit(patchVertices,targetLod);
}

bool EvalNodeTriangleShouldMerge(cbt_Node node,float4x4 modelView,float targetLod)
{
    leb_DiamondParent diamond = leb_DecodeDiamondParent_Square(node);
    float3x4 baseVertices = DecodeTriangleVertices(diamond.base);
    float3x4 topVertices = DecodeTriangleVertices(diamond.top);
    baseVertices[0] = mul(modelView,baseVertices[0]);
    baseVertices[2] = mul(modelView,baseVertices[2]);
    topVertices[0] = mul(modelView,topVertices[0]);
    topVertices[2] = mul(modelView,topVertices[2]);
    bool shouldMergeBase = TestShouldMerge(baseVertices,targetLod);
    bool shouldMergeTop = TestShouldMerge(topVertices,targetLod);
    return shouldMergeBase && shouldMergeTop;
}

bool EvalNodeTriangleShouldSplit(CBTEvalContext context)
{
    cbt_Node node = cbt_DecodeNode(context.nodeID);
    return EvalNodeTriangleShouldSplit(node,context.modelView,context.targetLod);
}

bool EvalNodeTriangleShouldMerge(CBTEvalContext context)
{
    cbt_Node node = cbt_DecodeNode(context.nodeID);
    return EvalNodeTriangleShouldMerge(node,context.modelView,context.targetLod);
}

#endif
```
