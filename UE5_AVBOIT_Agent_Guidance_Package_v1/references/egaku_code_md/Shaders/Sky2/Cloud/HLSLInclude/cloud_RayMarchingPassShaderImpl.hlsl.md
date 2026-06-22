# Egaku source: `Shaders/Sky2/Cloud/HLSLInclude/cloud_RayMarchingPassShaderImpl.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CLOUD_RAY_MARCHING_PASS_SHADER_IMPL_HLSL
#define CLOUD_RAY_MARCHING_PASS_SHADER_IMPL_HLSL
#include "cloud_Common.hlsl"
#include "cloud_RayMarching.hlsl"
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"

struct Attributes {
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};


                    
Varyings vert_screenspace_blit(Attributes a2v) {
    Varyings v2f;
    v2f.position = TransformObjectToHClip(a2v.vertex);
    v2f.uv = a2v.uv;
    return v2f;
}

float4 frag(Varyings v2f) : SV_TARGET
{
    Ray ray = GetWorldSpaceRay(v2f.uv);
    BoundingBox box = GetBoundingBox();
    CloudRayMarchingConfig config = GetCloudRayMarchingConfig();
    CloudVolumeDesc cloudDesc = GetCloudVolumeDesc();
    Light light = GetMainLight();
    SunLightDesc lightDesc;
    lightDesc.direction = light.direction;
    lightDesc.color = light.color;
    AmbientLightDesc ambientLightDesc = GetAmbientLightDesc();
    float4 color = ShadeCloud(ray,box,config,cloudDesc,lightDesc,ambientLightDesc);
    float4 background = GetBackgroundColor(v2f.uv);
    color = float4(color.rgb + background.rgb*color.a,1);
    return color;
}

#endif
```
