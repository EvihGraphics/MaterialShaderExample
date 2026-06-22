# Egaku source: `Shaders/Mesh/CBT/CBTHelperStructs.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CBT_HELPER_STRUCTS_HLSL
#define CBT_HELPER_STRUCTS_HLSL


struct CBTPackedEvalResult
{
    uint val;
};

struct CBTEvalResult
{
    uint nodeID;
    bool shouldUpdate; 
};

struct CBTEvalResultMeta
{
    uint resultCount;
};

struct CBTEvalContext
{
    uint nodeID;
    float4x4 model;
    float4x4 modelView;
    float4 frustumPlanes[6];
    float targetLod;
};

struct CBTEvalUpdateContext
{
    float4x4 modelView;
    float targetLod;
};



CBTPackedEvalResult PackEvalResult(CBTEvalResult result)
{
    CBTPackedEvalResult packed;
    packed.val = 0;
    packed.val |= result.shouldUpdate & 0x1;
    packed.val |= (result.nodeID & 0x7FFFFFFF) << 1;
    return packed;
}

CBTEvalResult UnpackEvalResult(CBTPackedEvalResult packed)
{
    CBTEvalResult result;
    result.shouldUpdate = packed.val & 0x1;
    result.nodeID = (packed.val >> 1) & 0x7FFFFFFF;
    return result;
}


#endif
```
