# Egaku source: `ShaderLibrary/Transformation.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef TRANSFORMATION_HLSL
#define TRANSFORMATION_HLSL
#include "Input.hlsl"

// model matrix and its alias
// abbreviation, full name, semantic name, action function

// M, V, P, MV, VP, MVP
// MInv, VInv, PInv, MVInv, VPInv, MVPInv

/*
inline float4x4 M()
{
    return ;
}

inline float4x4 ModelMatrix()
{
    return M();
}

inline float4x4 ObjectToWorldMatrix()
{
    return M();
}

inline float4 ObjectToWorld(float4 positionOS)
{
    return mul(M(),positionOS);
}

// view matrix and its alias
// abbreviation, full name, semantic name, action function

inline float4x4 V()
{
    return UNITY_MATRIX_V;
}

inline float4x4 ViewMatrix()
{
    return V();
}

inline float4x4 WorldToCameraMatrix()
{
    return V();
}

inline float4 WorldToCamera(float4 positionWS)
{
    return mul(V(),positionWS);
}

// projection matrix and its alias
// abbreviation, full name, semantic name, action function

inline float4x4 P()
{
    return UNITY_MATRIX_P;
}

inline float4x4 ProjectionMatrix()
{
    return P();
}

inline float4x4 CameraToClipMatrix()
{
    return P();
}

inline float4 CameraToClip(float4 positionVS)
{
    return mul(P(),positionVS);
}


// MV matrix and its alias
// abbreviation, full name, semantic name, action function

inline float4x4 MV()
{
    return UNITY_MATRIX_MV;
}

inline float4x4 ModelViewMatrix()
{
    return MV();
}

inline float4x4 ObjectToCameraMatrix()
{
    return MV();
}

inline float4 ObjectToCamera(float4 positionVS)
{
    return mul(MV(),positionVS);
}

// VP matrix and its alias
// abbreviation, full name, semantic name, action function

inline float4x4 VP()
{
    return UNITY_MATRIX_VP;
}

inline float4x4 ViewProjectionMatrix()
{
    return VP();
}

inline float4x4 WorldToClipMatrix()
{
    return VP();
}

inline float4 WorldToClip(float4 positionWS)
{
    return mul(VP(), positionWS);
}

// MVP matrix and its alias
// abbreviation, full name, semantic name, action function

inline float4x4 MVP()
{
    return UNITY_MATRIX_MVP;
}

inline float4x4 ModelViewProjectionMatrix()
{
    return MVP();
}

inline float4x4 ObjectToClipMatrix()
{
    return MVP();
}

inline float4 ObjectToClip(float3 positionOS)
{
    
    return mul(MVP(), float4(positionOS,1));
}


inline float4 ObjectToClip(float4 positionOS)
{
    return mul(MVP(), positionOS);
}

// MInv matrix and its alias
// abbreviation, full name, semantic name, action function


inline float4x4 MInv()
{
    return unity_WorldToObject;
}

inline float4x4 ModelMatrixInverse()
{
    return MInv();
}

inline float4x4 WorldToObjectMatrix()
{
    return MInv();
}

inline float4 WorldToObject(float4 positionWS)
{
    return mul(MInv(), positionWS);
}


// VInv matrix and its alias
// abbreviation, full name, semantic name, action function

inline float4x4 VInv()
{
    return UNITY_MATRIX_I_V; 
}

inline float4x4 ViewMatrixInverse()
{
    return VInv();
}

inline float4x4 CameraToWorldMatrix()
{
    return VInv();
}

inline float4 CameraToWorld(float4 positionVS)
{
    return mul(VInv(), positionVS);
}

// PInv matrix and its alias
// abbreviation, full name, semantic name, action function

inline float4x4 PInv()
{
    return unity_CameraInvProjection;
}

inline float4x4 ProjectionMatrixInverse()
{
    return PInv();
}

inline float4x4 ClipToCameraMatrix()
{
    return PInv();
}

inline float4 ClipToCamera(float4 positionVS)
{
    return mul(PInv(), positionVS);
}
*/

#endif
```
