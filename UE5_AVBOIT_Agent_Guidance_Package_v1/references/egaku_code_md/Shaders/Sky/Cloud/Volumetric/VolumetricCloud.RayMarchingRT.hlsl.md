# Egaku source: `Shaders/Sky/Cloud/Volumetric/VolumetricCloud.RayMarchingRT.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUMETRIC_CLOUD_RAY_MARCHING_RT_HLSL
#define VOLUMETRIC_CLOUD_RAY_MARCHING_RT_HLSL

RWTexture2D<float4> _VolumetricCloud_RayMarchingRT;
int _VolumetricCloud_SelectedQuadId;


void SetRayMarchingRTValue(uint2 pixelId, float4 value)
{ 
    _VolumetricCloud_RayMarchingRT[pixelId] = value;
}


uint2 GetRTResolution()
{
    uint width, height;
    _VolumetricCloud_RayMarchingRT.GetDimensions(width, height);
    return uint2(width, height);
}

float2 GetRTSampleDuv()
{
    return rcp(GetRTResolution());
}


uint GetPixelQuadIndex(uint2 pixelId)
{
    int2 quadIndex2 = pixelId % 2;
    int quadIndex = quadIndex2.x + quadIndex2.y * 2; 
    return quadIndex;
}

uint GetPixelQuadIndex(float2 uv)
{
    int2 pixelIndex = int2(uv/GetRTSampleDuv());
    return GetPixelQuadIndex(pixelIndex);
}


uint GetPixelQuadIndexSelected()
{
    return _VolumetricCloud_SelectedQuadId;
    float fps = 60;
    float time = GetTimeContext().timeInSec;
    int frame = time * fps;
    return frame % 4;
}

float2 GetRayMarchingRTSampleUV(uint2 pixelId)
{
    return pixelId*GetRTSampleDuv() + GetRTSampleDuv()*0.5f;
}


uint2 GetRayMarchingRTPixelId(uint2 threadId)
{
    //return threadId;
    uint2 pixelResolution = GetRTResolution();
    uint2 pixelId = (threadId * 2) % pixelResolution;
	uint2 offset = (threadId * 2) / pixelResolution;
    pixelId += offset;
    return pixelId;
}


#endif
```
