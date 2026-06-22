# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/Volume/VolumeUtils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_UTILS_HLSL
#define VOLUME_UTILS_HLSL

struct VolumeSliceAccessDesc
{
    float sliceIndex;
    uint sliceIndexLower;
    uint sliceIndexUpper;
    float lowerFraction;
    float upperFraction;
};

VolumeSliceAccessDesc GetVolumeSliceAccessDesc(float sliceIndex)
{
    float sliceIndexLower = floor(sliceIndex);//sliceIndex - 0.5;
    float sliceIndexUpper = sliceIndexLower + 1; //sliceIndex + 0.5;
    /*
     * slice index:                    0 0.5 1 1.5 2 2.5...
     *                              |--+--|--+--|--+--| ...
     *       some slice                     l--i--u     
     * (index = 1.75 for example)  lower bound index (l) = 1 fraction: 0.75
     *                                   slice index (i) = 1.75
     *                             upper bound index (u) = 2 fraction: 0.25
     */
    VolumeSliceAccessDesc desc;
    desc.sliceIndex = sliceIndex;
    desc.sliceIndexLower = uint(sliceIndexLower);
    desc.sliceIndexUpper = uint(sliceIndexUpper);
    desc.lowerFraction = desc.sliceIndexUpper - sliceIndex;
    desc.upperFraction = sliceIndex - desc.sliceIndexLower; 
    return desc;
}

float GetExtinctionFromBaseColor(float4 baseColor)
{
    float transmittance = 1.0f - baseColor.a;
    return -log(transmittance);
}

float3 GetExtinctionRGBFromBaseColor(float4 baseColor)
{
    baseColor.rgb *= baseColor.a;
    float3 transmittance = 1.0f - baseColor.rgb;
    return -log(transmittance);
}

#endif
```
