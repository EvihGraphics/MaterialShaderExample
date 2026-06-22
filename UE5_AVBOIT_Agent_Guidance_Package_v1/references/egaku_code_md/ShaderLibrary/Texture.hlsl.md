# Egaku source: `ShaderLibrary/Texture.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef TEXTURE_HLSL
#define TEXTURE_HLSL
#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/GlobalSamplers.hlsl"
#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/API/D3D11.hlsl"

float Remap(float x, float min0, float max0, float min1, float max1)
{
    return ((x - min0) / (max0 - min0)) * (max1 - min1) + min1;
}

float Decode01Range(float color01,float decodedMin,float decodedMax,bool inverse = false){
    if (inverse){
        return Remap(color01, 1.0, 0.0, decodedMin, decodedMax);
    }else{
        return Remap(color01, 0.0, 1.0, decodedMin, decodedMax);
    }
}

float Decode01Norm(float color01,bool inverse = false){
    if (inverse){
        return Remap(color01, 1.0, 0.0, -1.0,1.0);
    }else{
        return Remap(color01, 0.0, 1.0, -1.0,1.0);
    }
}

float Decode01NormScale(float color01,float scale,bool inverse = false)
{
    return Decode01Norm(color01, inverse)*scale;
}

float Decode01OffsetScale(float color01,float offset,float scale,bool inverse = false){
    float result = (color01 + offset) * scale;
    if (inverse){
        return -result;
    }else{
        return result;
    }
}

float4 SampleTexture(Texture2D tex, SamplerState samplerState, float2 uv)
{
    return SAMPLE_TEXTURE2D(tex,samplerState, uv);
}

float4 SampleTextureLevel(Texture2D tex, SamplerState samplerState, float2 uv, float lod)
{
    return SAMPLE_TEXTURE2D_LOD(tex, samplerState, uv, lod);
}

float4 SampleTextureBias(Texture2D tex, SamplerState samplerState, float2 uv,float bias)
{
    return SAMPLE_TEXTURE2D_BIAS(tex, samplerState, uv, bias);
}

float4 SampleTextureGrad(Texture2D tex, SamplerState samplerState, float2 uv, float2 dpdx, float2 dpdy)
{
    return SAMPLE_TEXTURE2D_GRAD(tex, samplerState, uv, dpdx, dpdy);
}

float4 SampleTextureArray(Texture2DArray tex, SamplerState samplerState, float2 uv, float slice)
{
    return SAMPLE_TEXTURE2D_ARRAY(tex, samplerState, uv, slice);
}

float4 SampleTextureArrayLevel(Texture2DArray tex, SamplerState samplerState, float2 uv, float slice, float lod)
{
    return SAMPLE_TEXTURE2D_ARRAY_LOD(tex, samplerState, uv, slice, lod);
}

 float4 SampleTextureArrayBias(Texture2DArray tex, SamplerState samplerState,  float2 uv, float slice, float bias)
{
    return SAMPLE_TEXTURE2D_ARRAY_BIAS(tex, samplerState, uv, slice, bias);
}

float4 SampleTextureArrayGrad(Texture2DArray tex, SamplerState samplerState, float2 uv,float slice, float2 dpdx, float2 dpdy)
{
    return SAMPLE_TEXTURE2D_ARRAY_GRAD(tex, samplerState, uv, slice, dpdx, dpdy);
}

float4 SampleTextureCube(TextureCube tex, SamplerState samplerState, float3 uv)
{
    return SAMPLE_TEXTURECUBE(tex, samplerState, uv);
}

float4 SampleTextureCubeLevel(TextureCube tex, SamplerState samplerState, float3 uv, float lod)
{
    return SAMPLE_TEXTURECUBE_LOD(tex, samplerState, uv, lod);
}

float4 SampleTextureCubeBias(TextureCube tex, SamplerState samplerState, float3 uvw, float bias)
{
    return SAMPLE_TEXTURECUBE_BIAS(tex, samplerState, uvw, bias);
}

float4 SampleTextureCubeArray(TextureCubeArray tex, SamplerState samplerState, float4 uv)
{
    return SAMPLE_TEXTURECUBE_ARRAY(tex, samplerState, uv.xyz, uv.w);
}

float4 SampleTextureCubeArrayLevel(TextureCubeArray tex, SamplerState samplerState, float4 uv, float lod)
{
    return SAMPLE_TEXTURECUBE_ARRAY_LOD(tex, samplerState, uv.xyz, uv.w, lod);
}

float4 SampleTextureCubeArrayBias(TextureCubeArray tex, SamplerState samplerState, float4 uv, float bias)
{
    return SAMPLE_TEXTURECUBE_ARRAY_BIAS(tex, samplerState, uv.xyz, uv.w, bias);
}

float4 SampleTexture3D(Texture3D tex, SamplerState samplerState, float3 uvw)
{
    return SAMPLE_TEXTURE3D(tex, samplerState, uvw);
}

float4 SampleTexture3DLevel(Texture3D tex, SamplerState samplerState, float3 uvw, float lod)
{
    return SAMPLE_TEXTURE3D_LOD(tex, samplerState, uvw, lod);
}



#endif
```
