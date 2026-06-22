# Egaku source: `Shaders/Sky/Basic/RaySpace/OctahedronConcentric.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef OCTAHEDRON_CONCENTRIC_HLSL
#define OCTAHEDRON_CONCENTRIC_HLSL
#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/Macros.hlsl"

float2 OctahedronConcentric_SquareToConcentricDiskPolar(float2 s)
{
    float a = 2.0f * s.x - 1.0f;
    float b = 2.0f * s.y - 1.0f;
    float r, phi;

    const float PI_FOURTH = PI_DIV_FOUR;
    if (a > -b)
    {
        if (a > b)      // region 1
        {
            r = a;
            phi = (PI_FOURTH) * (b / a);
        }
        else            // region 2
        {
            r = b;
            phi = (PI_FOURTH) * (2.0f - (a / b));
        }
    }
    else
    {
        if (a < b)      // region 3
        {
            r = -a;
            phi = (PI_FOURTH) * (4.0f + (b / a));
        }
        else            // region 4
        {
            r = -b;
            phi = (b != 0.0f) ? (PI_FOURTH) * (6.0f - (a / b)) : 0.0f;
        }
    }
    return float2(r,phi);
}

float2 OctahedronConcentric_SquareToConcentricDisk(float2 s)
{
    float2 p = OctahedronConcentric_SquareToConcentricDiskPolar(s);
    float2 d = p.x*float2(cos(p.y), sin(p.y));
    return d;
}


float2 OctahedronConcentric_ConcentricDiskPolarToSquare(float2 p)
{
    float r = p.x;
    float phi = p.y;
    const float PI_HALF = HALF_PI;
    const float PI_FOURTH = PI_DIV_FOUR;
    if (phi < -PI_FOURTH)
    {
        phi += TWO_PI; // in range [-pi/4, 7pi/4] 
    }
    float a, b;
    if (phi < PI_FOURTH)          // region 1
    {
        a = r;
        b = phi * a / PI_FOURTH;
    }
    else if (phi < 3.0f * PI_FOURTH) // region 2
    {
        b = r;
        a = -(phi - PI_HALF) * b / PI_FOURTH;
    }
    else if (phi < 5.0f * PI_FOURTH) // region 3
    {
        a = -r;
        b = (phi - PI) * a / PI_FOURTH; 
    }
    else                              // region 4
    {
        b = -r;
        a = -(phi - 3.0f * PI_HALF) * b / PI_FOURTH;
    }

    //  [-1,1]^2 to [0,1]^2
    float2 s = 0.5f*(float2(a,b) + 1.0f);
    return s;
}

float2 OctahedronConcentric_ConcentricDiskToSquare(float2 d)
{
    float r = sqrt(d.x * d.x + d.y * d.y);
    float phi = atan2(d.y, d.x);
    return OctahedronConcentric_ConcentricDiskPolarToSquare(float2(r,phi));
}



// direction in world space
float2 OctahedronConcentric_GetHemiDirectionDiskPolar(float3 direction)
{
    float invScale = 1.0f / sqrt(1.0f + direction.y);
    float2 disk = direction.xz * invScale;
    float r = sqrt(disk.x * disk.x + disk.y * disk.y);
    float phi = atan2(disk.y, disk.x);
    return float2(r,phi);
}

// direction in world space
float2 OctahedronConcentric_GetHemiDirectionSampleCoords(float3 direction)
{
    float2 diskPolar = OctahedronConcentric_GetHemiDirectionDiskPolar(direction);
    return OctahedronConcentric_ConcentricDiskPolarToSquare(diskPolar);
}

float3 OctahedronConcentric_GetHemiDirectionFromDiskPolar(float2 diskPolar)
{
    float r = diskPolar.x;
    float r2 = r * r;
    float z = 1.0f - r2;
    float scale = sqrt(2.0f - r2);
    float2 disk = r*float2(cos(diskPolar.y), sin(diskPolar.y));
    
    return float3(disk * scale, z).xzy; // tangent space to  world space
}

float3 OctahedronConcentric_GetHemiDirectionFromSampleCoords(float2 uv)
{
    float2 diskPolar = OctahedronConcentric_SquareToConcentricDiskPolar(uv);
    return OctahedronConcentric_GetHemiDirectionFromDiskPolar(diskPolar);
}

float OctahedronConcentric_GetDirectionZenithAngleFromConcentricDiscPolar(float2 diskPolar)
{
    float r = diskPolar.x;
    float r2 = r * r;
    float z = 1.0f - r2;
    return acos(z);
}


#endif
```
