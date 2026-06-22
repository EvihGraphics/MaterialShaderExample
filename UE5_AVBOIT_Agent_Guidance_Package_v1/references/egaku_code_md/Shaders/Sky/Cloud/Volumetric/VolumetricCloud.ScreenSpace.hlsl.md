# Egaku source: `Shaders/Sky/Cloud/Volumetric/VolumetricCloud.ScreenSpace.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#pragma once
#ifndef VOLUMETRIC_CLOUD_SCREEN_SPACE_HLSL
#define VOLUMETRIC_CLOUD_SCREEN_SPACE_HLSL
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"
#include "VolumetricCloud.RayMarchingShading.hlsl"


Texture2D _BackgroundTex;

float4 GetBackgroundColor(float2 screenUV)
{
    return _BackgroundTex.SampleLevel(sampler_PointClamp,screenUV,0);
}


struct Attributes {
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = TransformObjectToHClip(input.vertex);
    output.uv = input.uv;
    return output;
}

float4 Frag(Varyings input) : SV_Target {
    float3 viewHit = GetWorldPosition(input.uv);
    float3 cameraPosWS = GetCameraPositionWS();
    float4 cloudColor = ShadeVolumetricCloud(cameraPosWS, viewHit);
    float4 background = GetBackgroundColor(input.uv);
    float4 color;
    color.rgb = cloudColor.rgb + (1 - cloudColor.a) * background.rgb;
    color.a = cloudColor.a + background.a - cloudColor.a * background.a;
    return color;
}

#endif
```
