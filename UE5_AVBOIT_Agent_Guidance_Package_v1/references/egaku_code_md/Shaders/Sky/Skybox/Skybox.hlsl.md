# Egaku source: `Shaders/Sky/Skybox/Skybox.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef SKYBOX_HLSL
#define SKYBOX_HLSL
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"
#include "Skybox.VolumetricCloudTexture.hlsl"
#include "Skybox.AtmospherePrt.hlsl"
#include "Skybox.SunDisk.hlsl"
#include "Skybox.ParamStruct.hlsl"
#include "Skybox.AtmosphereTransmittanceLut.hlsl"

#pragma enable_d3d11_debug_symbols


struct Attributes {
    float4 vertex : POSITION;
};

struct Varyings {
    float4 position : SV_POSITION;
    float3 rayDirection : TEXCOORD0;
};

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    output.rayDirection = TransformObjectToWorldDir(input.vertex, true);
    return output;
}

float4 Frag(Varyings input) : SV_Target {
    float3 rayDirection = normalize(input.rayDirection);
    Light light = GetMainLight();
    float4 cloudData = SampleVolumetricCloud(rayDirection);
    float cloudViewTransmittance = 1 - cloudData.a;
    float cloudViewAmbientLuminanceTransfer = cloudData.b;
    float cloudLuminanceTransfer = cloudData.r;
    float cloudGroundAmbientTransfer = cloudData.g;
    float4 atmosphereLuminanceTransfer = SampleAtmosphereOctahedronPrt(rayDirection);
    float4 atmosphereZenithLuminanceTransfer  = SampleAtmosphereOctahedronPrt(WorldVerticalDir());
    float sunIntensity = FetchParam(SkyboxDesc).sunLightIntensity;
    float3 sunDiskTransmittance = LookupAtmosphereDirectionTransmittance(0,rayDirection);
    float3 sunTransmittance = 0;
    if (GetVertical(light.direction) > 0)
    {
       sunTransmittance = LookupAtmosphereDirectionTransmittance(0,light.direction);
    }

    float3 sunLightIntensity = sunIntensity * sunTransmittance * light.color * PI;
    float3 atmosphereLumin = atmosphereLuminanceTransfer * sunLightIntensity;
    float3 atmosphereZenithLumin = atmosphereZenithLuminanceTransfer * sunLightIntensity;
    float3 cloudLumin = cloudLuminanceTransfer * sunLightIntensity + cloudViewAmbientLuminanceTransfer * atmosphereLumin + cloudGroundAmbientTransfer * atmosphereZenithLumin;
    float3 sunDiskLumin = sunDiskTransmittance * IsDirectionInSunDisk(light.direction,rayDirection) * sunLightIntensity;
    float3 finalColor = (sunDiskLumin + atmosphereLumin) * cloudViewTransmittance + cloudLumin;
   
    //color = cloudColor.rgb;
    return float4( finalColor,1);
}

#endif
```
