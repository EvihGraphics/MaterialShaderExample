# Egaku source: `Shaders/Ocean/Modules/Distant/DistantWater.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#ifndef DISTANT_WATER_HLSL
#define DISTANT_WATER_HLSL
#include "DistantWaveSequence.hlsl"

struct DistantSurfaceData
{
    float3 normal;
    float3 displacementVec;
    float2 velocity;
    float foam;
    float mask;
    float divergence;
    float4 debug;
};

float2 CalcDistantWaterVelocity(DistantWaveSeqFrameData frameData,float3 normal)
{
    DeclareParamFetch(DistantWaveDesc,waveDesc);
    float waveVelocityScale = lerp(waveDesc.waveVelocityBottom, waveDesc.waveVelocityTop, frameData.height01);
    float2 waveVelocityDir = GetHorizontal(normal);
    return waveVelocityScale * waveVelocityDir +  waveDesc.windVelocity;
}

float3 CalcDistantWaterNormal(DistantWaveSeqFrameData frameData)
{
    DeclareParamFetch(DistantWaveDesc, waveDesc);
    float2 normalDerivativeXY = frameData.normalDerivativeXY;
    normalDerivativeXY *= 26.67f * waveDesc.waveHeightScale * rcp(waveDesc.waveLengthScale);
    float3 normal = TangentSpaceDefToWorldSpaceDef(DecodePartialDerivativeNormal(normalDerivativeXY));
    if (GetVertical(normal) < 0)
    {
        normal = -normal;//Make3DFromHorizontal(GetHorizontal(normal),-GetVertical(normal));
    }
    return normal;
}

float3 CalcDistantWaterDisplacement(DistantWaveSeqFrameData frameData)
{
    return Make3DFromHorizontal(frameData.normalDerivativeXY * (-1.25f * (1.0f - frameData.height01)),frameData.height);
}

DistantSurfaceData CalcDistantWaterSurfaceData(float2 positionWS_Horizontal,OceanTerrainContext terrainContext)
{
    DistantWaveSeqFrameData frameData = SampleDistantWaveSeqFrameMap(positionWS_Horizontal,GetTimeContext().timeInSec,terrainContext);
    DistantSurfaceData data;
    float2 uv = CalcDistantWaveSeqFrameMapUV(positionWS_Horizontal);
  //  float3 normalDerivativeXY *= 26.67*waveDesc.waveHeightScale/waveDesc.waveLengthScale;
    data.normal = CalcDistantWaterNormal(frameData);
    data.displacementVec = CalcDistantWaterDisplacement(frameData);
    data.foam = max(frameData.foamIntensity,0);
    data.divergence = 0;
    data.velocity = CalcDistantWaterVelocity(frameData,data.normal);
    data.mask = 0;
    data.debug = float4(uv,0,0);
    return data;
}

#endif
```
