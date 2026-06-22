# Egaku source: `Shaders/Sky/Basic/VolumeBase/VolumeLighting.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef VOLUME_LIGHTING_HLSL
#define VOLUME_LIGHTING_HLSL
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Lighting/Volumetric.hlsl"

float3 BeerPowderAttenuate(float3 factor, float a) {
    float3 t = exp(-factor * a) * (1 - exp(-factor * 2 * a));
   // return t*2.5980762113533159402911695122588;
    return t;
}

// calculate phase function for cloud rendering.
// be careful about the theta angle's definition!
// be careful about the definition of light direction and view direction!
// view direction: vector that points to the camera
// light direction: vector that points to the light source
// theta angle: the angle between light shooting direction and the direction that pointing to the camera.
float GetPhaseAngleCosine(float3 lightDir,float3 viewDir)
{
    return dot(-lightDir, viewDir);
}


float CloudPhaseFunction(float cosTheta,float phaseG1,float phaseG2)
{
    float phaseVal = lerp(HGPhase(phaseG1,cosTheta), HGPhase(phaseG2,cosTheta), 0.5);
    return phaseVal;
}


float CloudPhaseFunction(float3 lightDir,float3 viewDir,float phaseG1,float phaseG2)
{
    float cosTheta = GetPhaseAngleCosine(lightDir, -viewDir);
    return CloudPhaseFunction(cosTheta, phaseG1, phaseG2);
}




#endif
```
