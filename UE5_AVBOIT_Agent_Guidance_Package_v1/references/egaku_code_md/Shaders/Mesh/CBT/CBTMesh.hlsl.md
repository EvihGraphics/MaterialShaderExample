# Egaku source: `Shaders/Mesh/CBT/CBTMesh.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CBT_MESH_HLSL
#define CBT_MESH_HLSL



#define CBT_BUFFER_NAME _CBTBuffer
#include "CBTMeshUtils.hlsl"



float4 CBTMesh_GetPosition(uint meshletInstanceID, float2 meshletVertex)
{
    cbt_Node node = cbt_DecodeNode(meshletInstanceID);
    float3x2 triangleVertices = DecodeTriangleVerticesXZ(node);
    float2 triangleTexCoords[3] = {
        triangleVertices[2],
        triangleVertices[1],
        triangleVertices[0]
    };

    float2 tessellatedVertex = TessellateTriangleVertex(triangleTexCoords,meshletVertex);
    return float4(tessellatedVertex.x,0,tessellatedVertex.y,1);
}


#endif
```
