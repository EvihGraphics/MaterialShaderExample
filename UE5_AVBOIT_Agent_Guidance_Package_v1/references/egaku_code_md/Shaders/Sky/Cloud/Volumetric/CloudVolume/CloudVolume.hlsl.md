# Egaku source: `Shaders/Sky/Cloud/Volumetric/CloudVolume/CloudVolume.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CLOUD_VOLUME_HLSL
#define CLOUD_VOLUME_HLSL
#include "Assets/Shaders/Sky/Basic/VolumeBase/Volume.hlsl"
#include "Assets/Shaders/Sky/Basic/Utils.hlsl"
#include "../VolumetricCloud.ParamStruct.hlsl"
#include "../CloudDensity/CloudVoxelDensity.hlsl"




float3 ScatteringColorToScatteringCoeff(float3 scatteringColor,float scale)
{
    return scatteringColor * scale;
}

float3 AbsorptionColorToAbsorptionCoeff(float3 absorptionColor,float scale)
{
    return rcp(absorptionColor) * scale;
}



VolumeDesc GetVolumeDescCloud()
{
    DeclareParamFetch(CloudVolumeDesc, volumeDesc);
    VolumeDesc context;
    context.scatteringCoeff = volumeDesc.scatteringCoeff;//ScatteringColorToScatteringCoeff(volumeDesc.scatteringColor,rcp(max(0.00000001,volumeDesc.scatteringDefinedDepth)));
    context.absorptionCoeff = volumeDesc.absorptionCoeff;// AbsorptionColorToAbsorptionCoeff(volumeDesc.absorptionColor,rcp(max(0.00000001,volumeDesc.absorptionDefinedDepth)));
    context.hgPhaseG1 = volumeDesc.phaseG1;
    context.hgPhaseG2 = volumeDesc.phaseG2;
    return context;
}

BoundingBox GetCloudVolumeBoundingBox()
{
    DeclareParamFetch(CloudVolumeDesc, volumeDesc);
    BoundingBox box;
    box.boundsMax = 0;
    box.boundsMin = 0;
    return box;
}

float GetEarthRadius()
{
    DeclareParamFetch(CloudVolumeDesc, volumeDesc);
    return volumeDesc.earthRadius;
}

float GetCloudVolumeUpperBound()
{
    DeclareParamFetch(CloudVolumeDesc, volumeDesc);
    return volumeDesc.heightUpperBound;
}

BoundingSphere GetCloudVolumeBoundingSphere()
{
    DeclareParamFetch(CloudVolumeDesc, volumeDesc);
    BoundingSphere sphere;
    const float earthRadius = volumeDesc.earthRadius;
    const float bottom = earthRadius + volumeDesc.heightLowerBound;
    const float top = earthRadius + volumeDesc.heightUpperBound;
    sphere.center = -ViewVerticalDir() * earthRadius;
    sphere.radiusMin = bottom;
    sphere.radiusMax = top;
    return sphere;
}

float GetPositionWSEarthAltitude(float3 positionWS)
{
    DeclareParamFetch(CloudVolumeDesc, volumeDesc);
    float3 earthCenter = -WorldVerticalDir() * volumeDesc.earthRadius;
    float3 raidusVec = positionWS - earthCenter;
    return length(raidusVec) - volumeDesc.earthRadius;
}

float ProcessCloudVoxelDensity_AltitudeGradient(float density,float cloudLayerHeight,float voxelPositionHeight)
{
    float factor = 0.125;
    float shape = density * smoothstep(0, cloudLayerHeight * factor,  voxelPositionHeight);
    shape = shape * smoothstep(0, cloudLayerHeight * factor, cloudLayerHeight -  voxelPositionHeight);
    
    float result = pow(shape, 1.5);
    result = lerp(result,saturate(result),0.5);
    return result;
}


float ProcessCloudVoxelDensity_HorizonGradient(float density,float viewZenithCosine,float gradientStartCosine,float gradientEndCosine)
{
    return density * smoothstep(gradientEndCosine,  gradientStartCosine,  viewZenithCosine);
}

float Sigmoid(float x,float halfPoint,float power)
{
    return  1.0f / (1.0f + exp(-power * (x - halfPoint)));
}



float ProcessVoxelDensity_Near(float density,float voxelPositionHeight,float viewZenithCosine,float gradientStartCosine,float gradientEndCosine)
{
    DeclareParamFetch(CloudVolumeDesc, volumeDesc);
    float cloudLayerHeight = volumeDesc.heightUpperBound - volumeDesc.heightLowerBound;
    float result = ProcessCloudVoxelDensity_AltitudeGradient(density,cloudLayerHeight,voxelPositionHeight - volumeDesc.heightLowerBound);
    result = ProcessCloudVoxelDensity_HorizonGradient(result,viewZenithCosine,gradientStartCosine,gradientEndCosine);
    return result;
}

float ProcessVoxelDensity_Distant(float density,float voxelPositionHeight,float viewZenithCosine,float gradientStartCosine,float gradientEndCosine)
{
    DeclareParamFetch(CloudVolumeDesc, volumeDesc);
    float cloudLayerHeight = volumeDesc.heightUpperBound - volumeDesc.heightLowerBound;
    float result = ProcessCloudVoxelDensity_AltitudeGradient(density,cloudLayerHeight,voxelPositionHeight - volumeDesc.heightLowerBound);
    float mid = (gradientStartCosine + gradientEndCosine) * 0.5;
    result = ProcessCloudVoxelDensity_HorizonGradient(result,viewZenithCosine,mid, gradientStartCosine);
    result = ProcessCloudVoxelDensity_HorizonGradient(result,viewZenithCosine,mid ,gradientEndCosine);
    return result;
}

float SampleCloudVoxelDensity_Near(SamplePoint samplePoint)
{
    DeclareParamFetch(CloudVolumeDesc, desc);
    float basicDensity = SampleCloudVoxelBasicDensity_Near(samplePoint.position);
    float detailDensity = SampleCloudVoxelDetailDensity_Near(samplePoint.position);
    float density = saturate(basicDensity - detailDensity) * desc.densityScale;
    return density;
}

float SampleCloudVoxelDensity_Far(SamplePoint samplePoint)
{
    DeclareParamFetch(CloudVolumeDesc, desc);
    float basicDensity = SampleCloudVoxelBasicDensity_Distant(samplePoint.position);
    float detailDensity = SampleCloudVoxelDetailDensity_Distant(samplePoint.position);
    float density = saturate(basicDensity - detailDensity) * desc.densityScale;
    return density;
}

float SampleVoxelDensity(SamplePoint samplePoint)
{
    DeclareParamFetch(CloudVolumeDesc, desc);
    DeclareParamFetch(WindDesc,windDesc);
    float viewZenithCosine = abs(dot(samplePoint.direction,WorldVerticalDir()));
    float voxelPositionHeight = GetPositionWSEarthAltitude(samplePoint.position);
    float weather = SampleWeatherMap(samplePoint.position);
    samplePoint.position += GetTimeContext().timeInSec*normalize(windDesc.direction.xyz)*windDesc.speed;
    float density1 = SampleCloudVoxelDensity_Near(samplePoint);
   // return density1;
    float result1 = ProcessVoxelDensity_Near(density1,voxelPositionHeight,viewZenithCosine,desc.viewZenithGradientStart,desc.viewZenithGradientEnd);
   // return result1;
    float density2 = SampleCloudVoxelDensity_Far(samplePoint);
    float result2 = ProcessVoxelDensity_Distant(density2,voxelPositionHeight,viewZenithCosine,desc.distantViewZenithGradientStart,desc.distantViewZenithGradientEnd);
    return max(result1,result2)*weather;
}

VoxelData Volume_SampleVoxelData(SamplePoint samplePoint)
{
    float density = SampleVoxelDensity(samplePoint );
    VolumeDesc context = GetVolumeDescCloud();
    VoxelData result;
    result.sigmaS = density * context.scatteringCoeff;
    result.sigmaA = density * context.absorptionCoeff;
    result.sigmaT = result.sigmaS + result.sigmaA;
    result.position = samplePoint.position;
    result.density = density;
    result.sigmaSPerUnit = context.scatteringCoeff;
    result.sigmaAPerUnit = context.absorptionCoeff;
    result.sigmaTPerUnit = result.sigmaSPerUnit + result.sigmaAPerUnit;
    return result;
}



#endif
```
