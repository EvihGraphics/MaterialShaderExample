# Egaku source: `Shaders/Mesh/CBT/CBTCulling.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CBT_CULLING_HLSL
#define CBT_CULLING_HLSL

struct FrustumPlanes
{
    float4 planes[6];
};



// *****************************************************************************
// Frustum Implementation

/**
 * Extract Frustum Planes from MVP Matrix
 *
 * Based on "Fast Extraction of Viewing Frustum Planes from the World-
 * View-Projection Matrix", by Gil Gribb and Klaus Hartmann.
 * This procedure computes the planes of the frustum and normalizes
 * them.
 */
FrustumPlanes LoadFrustum(float4x4 mvp)
{
    float4 planes[6];

    for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 2; ++j) {
        planes[i*2+j].x = mvp[0][3] + (j == 0 ? mvp[0][i] : -mvp[0][i]);
        planes[i*2+j].y = mvp[1][3] + (j == 0 ? mvp[1][i] : -mvp[1][i]);
        planes[i*2+j].z = mvp[2][3] + (j == 0 ? mvp[2][i] : -mvp[2][i]);
        planes[i*2+j].w = mvp[3][3] + (j == 0 ? mvp[3][i] : -mvp[3][i]);
        planes[i*2+j]*= length(planes[i*2+j].xyz);
    }
    FrustumPlanes frustumPlanes = { planes };
    return frustumPlanes;
}

/**
 * Negative Vertex of an AABB
 *
 * This procedure computes the negative vertex of an AABB
 * given a normal.
 * See the View Frustum Culling tutorial @ LightHouse3D.com
 * http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/
 */
float3 NegativeVertex(float3 bmin, float3 bmax, float3 n)
{
    bool3 b = n > 0;
    return lerp(bmin, bmax, b);
}

/**
 * Frustum-AABB Culling Test
 *
 * This procedure returns true if the AABB is either inside, or in
 * intersection with the frustum, and false otherwise.
 * The test is based on the View Frustum Culling tutorial @ LightHouse3D.com
 * http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/
 */
bool FrustumCullingTest(in const float4 planes[6], float3 bmin, float3 bmax,float bias = 0.f)
{
    float a = 1.0f;

    for (int i = 0; i < 6 && a >= 0.0f; ++i) {
        float3 n = NegativeVertex(bmin, bmax, planes[i].xyz);
        a = dot(float4(n, 1.0f), planes[i]);
        a += bias;
    }

    return (a >= 0.0);
}

bool FrustumCullingTest(float4x4 mvp, float3 bmin, float3 bmax)
{
    return FrustumCullingTest(LoadFrustum(mvp).planes, bmin, bmax);
}


bool FrustumCullingTest(float4x4 mvp,float3x4 patchVertices)
{
    float3 bmin = min(min(patchVertices[0], patchVertices[1]), patchVertices[2]).xyz;
    float3 bmax = max(max(patchVertices[0], patchVertices[1]), patchVertices[2]).xyz;

    return FrustumCullingTest(LoadFrustum(mvp).planes, bmin, bmax);
}

bool FrustumCullingTest(float4 frustumPlanes[6],float3x4 patchVertices,float bias = 0.f)
{
    //bmin.x = min{A.x,B.x,C.x}
    float3 bmin = min(min(patchVertices[0], patchVertices[1]), patchVertices[2]).xyz;
    float3 bmax = max(max(patchVertices[0], patchVertices[1]), patchVertices[2]).xyz;

    return FrustumCullingTest(frustumPlanes, bmin, bmax, bias);
}


#endif
```
