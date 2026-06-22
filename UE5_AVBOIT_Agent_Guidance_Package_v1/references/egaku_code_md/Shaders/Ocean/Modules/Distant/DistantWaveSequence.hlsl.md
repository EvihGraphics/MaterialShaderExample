# Egaku source: `Shaders/Ocean/Modules/Distant/DistantWaveSequence.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef DISTANT_WAVE_SEQUENCE_HLSL
#define DISTANT_WAVE_SEQUENCE_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Core.hlsl"
#include "Ocean.ParamStruct.hlsl"
#include "../Terrain/OceanTerrain.hlsl"
#include "../Utils.hlsl"

struct DistantWaveSeqFrameData
{
    float2 normalDerivativeXY;
    float foamIntensity;
    float height;
    float height01;
};


float2 CalcDistantWaveSeqFrameMapUV(float2 positionWS_Horizontal)
{
    TimeContext time = GetTimeContext();
    DeclareParamFetch(DistantWaveDesc, waveDesc);
    float2 uv = positionWS_Horizontal - (waveDesc.windVelocity * time.timeInSec);
    // a whole wave texture for 1x1 meter^2 in world. 
    // thus, sample with uv/waveLengthScale will
    // make a whole wave texture for waveLengthScale x waveLengthScale meter^2 
    uv /= waveDesc.waveLengthScale;
    return frac(uv);
}



DistantWaveSeqFrameData DecodeDistantWaveSeqFrameSampleValue(float4 value,DistantWaveDesc waveDesc)
{
    DistantWaveSeqFrameData data;

    float2 normalDerivativeXY = (value.rg - 0.5f)*waveDesc.mapNormalDecodeConst;
    float2 xy = normalDerivativeXY;
    float z  = sqrt(1-(xy.x*xy.x + xy.y*xy.y));
    normalDerivativeXY = xy/z;
    // Supposing that normal N = (n.x,n.y,n.z), normalDerivativeXY is (n.x/n.z,n.y/n.z), which is also
    // the X and Y derivative of height map (-dh/du,-dh/dv). (h for height map h(u,v))
    // As d(a*f)/dx = a*df/dx, scale the height with S will affect the derivative of height map. (-dh/du,-dh/dv) => S*(-dh/du,-dh/dv)
    // So we need to scale the normalDerivativeXY with S if we want to scale the height.
    // Also, if s(u,v) = scale*(u,v) is put into h(u,v), then dh/du = dh/ds * ds/du = n.x/n.z * scale.
    // So after all, don't forget to operate on normal derivative encoded value when operated on the height map and uv.
    // See https://zhuanlan.zhihu.com/p/1957489476411593977
    data.height = (value.b + waveDesc.waveHeightOffset) * waveDesc.waveHeightScale;
    data.height01 = value.b;
    data.normalDerivativeXY = normalDerivativeXY;
    data.foamIntensity = (value.a + waveDesc.foamOffset) * waveDesc.foamScale;
    return data;
}


DistantWaveSeqFrameData SampleDistantWaveSeqFrameMap(float2 positionWS_Horizontal,float timeInSec,OceanTerrainContext terrainContext)
{
    DeclareParamFetch(DistantWaveDesc, waveDesc);
    waveDesc.waveHeightScale *= terrainContext.waveScale;
    waveDesc.foamScale *= terrainContext.waveScale;
    float2 uv = CalcDistantWaveSeqFrameMapUV(positionWS_Horizontal);
    float slice = fmod(timeInSec*waveDesc.waveSeqFPSSliceCount.x,waveDesc.waveSeqFPSSliceCount.y);
    float4 value = SampleTextureArrayLevel(waveDesc.waveSeqFrameMap, sampler_LinearRepeat,uv,slice,0);
    DistantWaveSeqFrameData data = DecodeDistantWaveSeqFrameSampleValue(value,waveDesc);
    return data;
}

#endif
```
