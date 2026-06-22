# Egaku source: `Shaders/Sky2/Cloud/HLSLInclude/cloud_RayMarching.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CLOUD_RAY_MARCHING_HLSL
#define CLOUD_RAY_MARCHING_HLSL
#include "cloud_LightingUtils.hlsl"
#include "cloud_BoundsUtils.hlsl"
#include "cloud_TextureSampler.hlsl"
#include "cloud_RayJitterUtils.hlsl"


float CalcDensityOfSunLightDir(float3 pos,BoundingBox box,CloudRayMarchingConfig marchConfig,CloudVolumeDesc cloudVolDesc,SunLightDesc sunLightDesc)
{
    Ray ray;
    ray.direction = sunLightDesc.direction; // points to the light source
    ray.origin = pos;
    ray.hitPoint = sunLightDesc.direction * 10000;
    BoundsHitResult boundsHitResult = CalcBoundsHit(box,ray);

    float maxDistance = boundsHitResult.distInsideBox;
    float stepSize = maxDistance/marchConfig.lightMarchStepCnt;
    
    float sumDistance = 0;
    float sumDensity = 0;
    float3 samplePos = pos;
    
    // ray marching of sunlight direction
    [loop]
    for (int i = 0; i < 50; i++)
    {
        if (sumDistance >= maxDistance)
        {
            break;
        }
        float density = SampleCloudVolume(cloudVolDesc,box,samplePos);
        float deltaDensity = density * stepSize;
        sumDensity += deltaDensity;
        samplePos += ray.direction * stepSize;
        sumDistance += stepSize;
    }
    return sumDensity;
}



float3 CalcReceivedLightEnergy(float3 viewDir,float densityOfPoint,float densityOfSunLightDir,CloudVolumeDesc cloudVolDesc,SunLightDesc sunLightDesc,AmbientLightDesc ambientLightDesc)
{
    VolumeLightingDesc lightingDesc = cloudVolDesc.lightingDesc;
    OctaveScatterDesc scatterDesc = lightingDesc.octaveScatterDesc;
    float sunPhaseAngle = GetPhaseAngle(sunLightDesc.direction,viewDir);

    float3 multiScatterEnergy = 0;

    //octave multi scattering method from 
    [loop]
    for (int i = 0; i < scatterDesc.count; i++)
    {
        float sunPhaseVal = OctaveMultiScatterCloudPhaseFunction(sunPhaseAngle,lightingDesc.hgPhaseG1Factor,
            lightingDesc.hgPhaseG2Factor,scatterDesc.phaseFactor,i);
        float3 extCoeff = OctaveScatterLightExtinctCoeff(densityOfSunLightDir,lightingDesc.sigmaExt,
            scatterDesc.extFactor, i);
        float3 sigmaExt = OctaveScatterLightSigmaExtinct(lightingDesc.sigmaExt,scatterDesc.extFactor, i);
        float3 scatterCoeff = OctaveScatterLightScatterCoeff(densityOfPoint,lightingDesc.sigmaScatter,
            scatterDesc.scatterFactor, i);
        float3 scatterEnergy = CalcLightEnergyOfPoint(sunPhaseVal,sunLightDesc.color,
            ambientLightDesc.phase,ambientLightDesc.color);
        float transmittance = BeerLambertTransmittance(extCoeff);
        scatterEnergy = scatterCoeff * scatterEnergy * transmittance;
        scatterEnergy = scatterEnergy*(1-transmittance)/sigmaExt;
        multiScatterEnergy += scatterEnergy;
    }
    return multiScatterEnergy;
}

float4 ShadeCloud(Ray ray,BoundingBox box,CloudRayMarchingConfig marchConfig,CloudVolumeDesc cloudVolDesc,SunLightDesc sunLightDesc,AmbientLightDesc ambientLightDesc)
{
    BoundsHitResult boundsHitResult = CalcBoundsHit(box,ray);
    float maxDistance = boundsHitResult.distInsideBox;
    float3 entryPos = ray.origin + ray.direction * boundsHitResult.distToBox;
    float stepSize = marchConfig.viewMarchStepSize;
    VolumeLightingDesc lightingDesc = cloudVolDesc.lightingDesc;
    
    JitterDesc jitterDesc = (JitterDesc)0;
    float viewDistance = stepSize*0.5;
    float3 viewLightEnergy = 0;
    float3 viewDensity = 0;
    float3 viewTransmittance = 1;
    float3 samplePos = JitterPoint(entryPos + ray.direction*stepSize*0.5,jitterDesc);
    
    // ray marching
    [loop]
    for (int i = 0; i < 50; i++)
    {
        if (viewDistance > maxDistance)
        {
            break;
        }
        float density = SampleCloudVolume(cloudVolDesc,box,samplePos);
        float densityOfPoint = density * stepSize;
        float densityOfSunLightDir = CalcDensityOfSunLightDir(samplePos,box,marchConfig,cloudVolDesc,sunLightDesc);
        float3 deltaLightEnergy = CalcReceivedLightEnergy(-ray.direction,densityOfPoint,densityOfSunLightDir,cloudVolDesc,sunLightDesc,ambientLightDesc);
        viewDensity += density;
        viewLightEnergy +=  deltaLightEnergy * viewTransmittance;
        viewTransmittance *= BeerLambertTransmittance(LightExtinctCoeff(densityOfPoint,lightingDesc.sigmaExt));
        viewDistance += stepSize;
        samplePos += ray.direction * stepSize;
    }
    float alpha = viewTransmittance;
    return float4(viewLightEnergy,alpha);
}


#endif
```
