# Egaku source: `Shaders/Mesh/CBT/CBTMeshUtils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CBT_MESH_UTILS_HLSL
#define CBT_MESH_UTILS_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Utilities/CBT/CBTLongestEdgeBisectionUtils.hlsl"


/*******************************************************************************
 * TriangleLevelOfDetail -- Computes the LoD assocaited to a triangle
 *
 * This function is used to garantee a user-specific pixel edge length in
 * screen space. The reference edge length is that of the longest edge of the
 * input triangle.In practice, we compute the LoD as:
 *      LoD = 2 * log2(EdgePixelLength / TargetPixelLength)
 * where the factor 2 is because the number of segments doubles every 2
 * subdivision level.
 */
float TriangleLevelOfDetail_Perspective(float3x4 patchVertices,float4x4 modelViewMatrix, float lodFactor)
{
    float3 v0 = mul(modelViewMatrix, patchVertices[0]).xyz;
    float3 v2 = mul(modelViewMatrix, patchVertices[2]).xyz;

    #if 0 //  human-readable version
    float3 edgeCenter = (v0 + v2); // division by 2 was moved to u_LodFactor
    float3 edgeVector = (v2 - v0);
    float distanceToEdgeSqr = dot(edgeCenter, edgeCenter);
    float edgeLengthSqr = dot(edgeVector, edgeVector);

    return u_LodFactor + log2(edgeLengthSqr / distanceToEdgeSqr);
    #else // optimized version
    float sqrMagSum = dot(v0, v0) + dot(v2, v2);
    float twoDotAC = 2.0f * dot(v0, v2);
    float distanceToEdgeSqr = sqrMagSum + twoDotAC;
    float edgeLengthSqr = sqrMagSum - twoDotAC;

    return lodFactor + log2(edgeLengthSqr / distanceToEdgeSqr);
    #endif
}


float TriangleLevelOfDetail_Perspective(float3x4 patchVertices,float lodFactor)
{
    float3 v0 = patchVertices[0].xyz;
    float3 v2 = patchVertices[2].xyz;

    #if 0 //  human-readable version
    float3 edgeCenter = (v0 + v2); // division by 2 was moved to u_LodFactor
    float3 edgeVector = (v2 - v0);
    float distanceToEdgeSqr = dot(edgeCenter, edgeCenter);
    float edgeLengthSqr = dot(edgeVector, edgeVector);

    return u_LodFactor + log2(edgeLengthSqr / distanceToEdgeSqr);
    #else // optimized version
    float sqrMagSum = dot(v0, v0) + dot(v2, v2);
    float twoDotAC = 2.0f * dot(v0, v2);
    float distanceToEdgeSqr = sqrMagSum + twoDotAC;
    float edgeLengthSqr = sqrMagSum - twoDotAC;

    return lodFactor + log2(edgeLengthSqr / distanceToEdgeSqr);
    #endif
}


float3x2 DecodeTriangleVerticesXZ(in const cbt_Node node)
{
    float3x2 vertices;
    // Since 'height' is always 0, one dimension is omitted.
    vertices[0] = float2(0,1);
    vertices[1] = float2(0,0);
    vertices[2] = float2(1,0);
    float3x2 pos = leb_DecodeNodeAttributeArray_Square(node, vertices);
    return pos;
}

float3x4 DecodeTriangleVertices(in const cbt_Node node)
{
    float3x2 triangleVerticesXZ = DecodeTriangleVerticesXZ(node);
    float3x4 vertices;
    vertices[0] = float4(triangleVerticesXZ[0].x,0,triangleVerticesXZ[0].y,1);
    vertices[1] = float4(triangleVerticesXZ[1].x,0,triangleVerticesXZ[1].y,1);
    vertices[2] = float4(triangleVerticesXZ[2].x,0,triangleVerticesXZ[2].y,1);
    return vertices;
}
/*******************************************************************************
 * BarycentricInterpolation -- Computes a barycentric interpolation
 *
 */
float2 BarycentricInterpolation(float2 v[3], float2 u)
{
    return v[1] + u.x * (v[2] - v[1]) + u.y * (v[0] - v[1]);
}

float4 BarycentricInterpolation(float4 v[3], float2 u)
{
    return v[1] + u.x * (v[2] - v[1]) + u.y * (v[0] - v[1]);
}


float2 TessellateTriangleVertex(float2 coords[3],float2 tessCoord) {
    return BarycentricInterpolation(coords, tessCoord);
}




#endif
```
