# Egaku source: `Shaders/Sky/Basic/VolumeBase/Ray.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef RAY_HLSL
#define RAY_HLSL
#include "Volume.hlsl"

struct Ray
{
    float3 origin;
    float3 direction;
    float3 hitPoint;
};

struct SphereIntersectionResult
{
    bool intersected;
    float t0;
    float t1;
};

struct BoundsHitResult {
    float distToBounds;
    float distInsideBounds;
};




float CalcPlaneIntersection(float4 plane,Ray ray)
{
    float div = dot(plane,ray.direction);
    if (div == 0)
    {
        return FLT_INF;
    }
    return -dot(plane,float4(ray.origin,1))*rcp(div);
}

BoundsHitResult CalcFrustum4PlanesBoundsHit(float4x4 frustumPlanes4,Ray ray) {
    float tEnter = FLT_INF;
    float tExit = 0;
    float epsilon = 0.0001;
    BoundsHitResult result = (BoundsHitResult)0;
    // assume plane normal is pointing outwards
    for (int i = 0; i < 4; i++)
    {
        float d = dot(frustumPlanes4[i],ray.direction);
        float nom = dot(frustumPlanes4[i],float4(ray.origin,1));
        // plane to enter
        if (d < -epsilon) 
        {

            tEnter = min(tEnter,-nom*rcp(d));
        }else if (d > epsilon)
        {
            tExit = max(tExit,-nom*rcp(d));
        }else
        {
            if (nom > 0)
            {
                return result;
            }
        }
    }
    result.distInsideBounds = max(0,tExit - tEnter);
    result.distToBounds = tEnter;
    return result;
}

SphereIntersectionResult CalcSphereIntersection(Sphere sphere, Ray ray)
{
    float a = dot(ray.direction, ray.direction);
    float3 cr = ray.origin - sphere.center;
    float b = 2.0 * dot(ray.direction, cr);
    float c = dot(cr, cr) - sphere.radius * sphere.radius;
    float delta = b * b - 4 * a * c;
    SphereIntersectionResult result;
    if (delta < 0)
    {
        result.intersected = false;
        result.t0 = -FLT_INF;
        result.t1 = -FLT_INF;
        return result;
    }
    result.intersected = true;
    result.t0 = (-b - sqrt(delta)) / (2 * a);
    result.t1 = (-b + sqrt(delta)) / (2 * a);
    return result;
}


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
    result.distToBounds = max(0, distA);
    result.distInsideBounds = max(0, distB - result.distToBounds);
    return result;
}


BoundsHitResult CalcBoundsHitInsideBoundingSphere(BoundingSphere sphere,Ray ray) {
    Sphere lower = {sphere.center,sphere.radiusMin};
    Sphere upper = {sphere.center,sphere.radiusMax};
    SphereIntersectionResult lowerResult = CalcSphereIntersection(lower, ray);
    SphereIntersectionResult upperResult = CalcSphereIntersection(upper, ray);
    BoundsHitResult result;
    float entryDist = max(lowerResult.t1, upperResult.t0);
    float exitDist = max(lowerResult.t0, upperResult.t1);
    result.distToBounds = entryDist; // negative value indicates when the ray is inside the bounding sphere
    result.distInsideBounds = exitDist - 0;
    return result;
}

BoundsHitResult CalcBoundsHit(BoundingSphere sphere,Ray ray,bool ignoreSecondEntry = true) {
    Sphere lower = {sphere.center,sphere.radiusMin};
    Sphere upper = {sphere.center,sphere.radiusMax};
    SphereIntersectionResult lowerResult = CalcSphereIntersection(lower, ray);
    SphereIntersectionResult upperResult = CalcSphereIntersection(upper, ray);
    BoundsHitResult result;
    lowerResult.t0 = max(lowerResult.t0, 0);
    lowerResult.t1 = max(lowerResult.t1, 0);
    upperResult.t0 = max(upperResult.t0, 0);
    upperResult.t1 = max(upperResult.t1, 0);
    float entryDist = max(lowerResult.t1, upperResult.t0);
    float exitDist = max(lowerResult.t0, upperResult.t1);
    result.distToBounds = entryDist;
    result.distInsideBounds = exitDist - entryDist;
    return result;
}
#endif
```
