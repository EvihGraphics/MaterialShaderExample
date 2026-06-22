# Egaku source: `Shaders/Ocean/Ocean.Mesh.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef OCEAN_MESH_HLSL
#define OCEAN_MESH_HLSL
#include "Assets/Shaders/Mesh/CBT/CBTHelperStructs.hlsl"

StructuredBuffer<uint> _CBTBuffer;
StructuredBuffer<CBTPackedEvalResult> _CBTEvalResultBuffer;
float4x4 _ModelMatrix;
#include "Assets/Shaders/Mesh/CBT/CBTMesh.hlsl"

float4 OceanMesh_GetVertexPositionOS(float2 meshletPositionOS,uint instanceId)
{
    uint nodeID = UnpackEvalResult(_CBTEvalResultBuffer[instanceId]).nodeID;
    float4 positionOS = CBTMesh_GetPosition(nodeID,meshletPositionOS);
    return positionOS;
}

void SetupModelMatrix()
{
    unity_ObjectToWorld = _ModelMatrix;
}

#endif
```
