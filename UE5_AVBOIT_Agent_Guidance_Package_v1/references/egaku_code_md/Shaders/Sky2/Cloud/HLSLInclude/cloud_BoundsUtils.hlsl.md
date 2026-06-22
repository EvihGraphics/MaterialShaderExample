# Egaku source: `Shaders/Sky2/Cloud/HLSLInclude/cloud_BoundsUtils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CLOUD_BOUNDS_UTILS_HLSL
#define CLOUD_BOUNDS_UTILS_HLSL
#include "cloud_DeclareStructs.hlsl"

BoundsHitResult CalcBoundsHit(BoundingBox box,Ray ray) {
    // 1. separate ray movement and box volume to x,y,z
    // 2. calculate ray arrival of box separately: (bmin.x/d.x,bmin.y/d.y,bmin.z/d.z),(bmax.x/d.x,bmax.y/d.y,bmax.z/d.z)
    // 3. put results in proper order: for each axis's value, the larger value is the 'ray out', the smaller value is the 'ray in'
    // 4. find out when the ray hits and enters the box: max value of the 'ray in' timepoint which separates into x,y,z, and min value of 'ray out'
    // 5. because the ray direction vector is a unit vector, time of ray enter is just the distance between ray origin and the hitpoint on the box.

    float3 t0 = (box.boundsMin - ray.origin) / ray.direction;
    float3 t1 = (box.boundsMax - ray.origin) / ray.direction;
    float3 tmin = min(t0, t1);
    float3 tmax = max(t0, t1);

    float distA = max(max(tmin.x, tmin.y), tmin.z);
    float distB = min(tmax.x, min(tmax.y, tmax.z));
    BoundsHitResult result;
    // also works for the case that ray origin being inside the bounding box.
    result.distToBox = max(0, distA);
    result.distInsideBox = max(0, distB - result.distToBox);
    return result;
}


BoundsHitResult CalcBoundsHit(Sphere sphere,Ray ray)
{
    
}

#endif
```
