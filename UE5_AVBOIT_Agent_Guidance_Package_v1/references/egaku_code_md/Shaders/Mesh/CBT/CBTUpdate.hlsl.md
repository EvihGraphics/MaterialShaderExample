# Egaku source: `Shaders/Mesh/CBT/CBTUpdate.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CBT_UPDATE_HLSL
#define CBT_UPDATE_HLSL

#define CBT_FLAG_WRITE

#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Utilities/CBT/CBTLongestEdgeBisectionUtils.hlsl"

struct CBTUpdateContext
{
    float4x4 modelView;
    float4 frustumPlanes[6];
    float targetLod;
};

/*
bool TestShouldMerge(CBTUpdateContext context,float3x4 patchVertices)
{
    // culling test
  //  if (FrustumCullingTest(context.frustumPlanes,patchVertices) == true)
    {
        float lod = TriangleLevelOfDetail_Perspective(patchVertices,context.modelView,context.targetLod);
        return lod < 1.0;
    }
    return false;
}

bool TestShouldSplit(CBTUpdateContext context,float3x4 patchVertices)
{
    // culling test
  //  if (FrustumCullingTest(context.frustumPlanes,patchVertices) == true)
    {
        float lod = TriangleLevelOfDetail_Perspective(patchVertices,context.modelView,context.targetLod);
        return lod > 1.0;
    }
    //return false;
}

void CBTMergeNode(CBTUpdateContext context,uint nodeID)
{
    cbt_Node node = cbt_DecodeNode(nodeID);
    leb_DiamondParent diamond = leb_DecodeDiamondParent_Square(node);
    float3x4 baseVertices= DecodeTriangleVertices(diamond.base);
    float3x4 topVertices = DecodeTriangleVertices(diamond.top);
    bool shouldMergeBase = TestShouldMerge(context,baseVertices);
    bool shouldMergeTop = TestShouldMerge(context,topVertices);

    if (shouldMergeBase && shouldMergeTop) {
        leb_MergeNode_Square(node, diamond);
    }
}*/

void CBTMergeNode(uint nodeID)
{
    cbt_Node node = cbt_DecodeNode(nodeID);
    leb_DiamondParent diamond = leb_DecodeDiamondParent_Square(node);
    leb_MergeNode_Square(node, diamond);
}

/*void CBTSplitNode(CBTUpdateContext context,uint nodeID)
{
    cbt_Node node = cbt_DecodeNode(nodeID);
    float3x4 triangleVertices = DecodeTriangleVertices(node);
    bool shouldSplit = TestShouldSplit(context,triangleVertices);
    if (shouldSplit) {
        leb_SplitNode_Square(node);
    }
}*/

void CBTSplitNode(uint nodeID)
{
    cbt_Node node = cbt_DecodeNode(nodeID);
    leb_SplitNode_Square(node);
}




#endif
```
