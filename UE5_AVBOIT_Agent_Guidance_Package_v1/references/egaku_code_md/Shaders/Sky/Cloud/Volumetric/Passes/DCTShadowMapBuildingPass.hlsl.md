# Egaku source: `Shaders/Sky/Cloud/Volumetric/Passes/DCTShadowMapBuildingPass.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef DCT_SHADOW_MAP_BUILDING_PASS_HLSL
#define DCT_SHADOW_MAP_BUILDING_PASS_HLSL

#include "Assets/Shaders/Sky/Basic/VolumeBase/VolumeRayMarching.hlsl"
#include "../VolumetricCloud.ParamStruct.hlsl"
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"
#include "Assets/Shaders/Sky/Basic/RaySpace/OctahedronConcentric.hlsl"
#include "../CloudVolume/CloudVolume.hlsl"
#include "../Buffers/DCTShadowMap.hlsl"
#include "../Utils/TilePixelSelector.hlsl"
#include "Assets/Shaders/Sky/Basic/VolumeBase/PerspectiveFrustum.hlsl"

#pragma enable_d3d11_debug_symbols

uint _DCTShadowMapBuild_CoeffIndexStart;
float4x4 _ViewProjectionMatrix;

struct Attributes {
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct PixelOutput
{
    float4 output0 : SV_Target0;
    float4 output1 : SV_Target1;
    float4 output2 : SV_Target2;
    float4 output3 : SV_Target3;
};

Varyings Vert(Attributes input) {
    Varyings output;
    output.position = input.vertex;
    output.uv = input.uv;
    return output;
}



DCTCoeffs RayMarchingCalcTransmittanceDCT(RayMarchingContext rayMarchingContext)
{
    SamplePoint samplePoint;
    samplePoint.position = rayMarchingContext.entryPosition;
    samplePoint.stepSize = rayMarchingContext.stepSize;
    samplePoint.direction = rayMarchingContext.direction;
    
    float transmittance = 1;

    // We step n times to get the volume extinction, but we actually evaluate transmittance function n+1 times,
    // since transmittance is always 1 at the beginning.
    // Also, n times extinction sampling uses n intervals (for i=0,1,2,...,n-1 represents [i*step, i*step+step] to cover
    // the whole ray marching range, but the transmittance function is defined on [0, n*step], and the divided intervals
    // generates n+1 endpoints (0, 1*step, 2*step, ..., n*stepSize), which actually stands for n+1 evaluation for
    // the transmittance function.
    // So we need to save transmittance(0)=1 first and then save each evaluation of transmittance function after
    // each volume extinction sampling ray step. 
    uint transmittanceEvalCount = rayMarchingContext.iterationCount + 1;
    

    // sample the center of each interval as the height of 'quad'.
    // each step evaluates the quad's area as 'sampleVal*step'.
    // the quad is [i*step, i*step+step] with height f(i*step + 0.5*step)
    samplePoint.position += samplePoint.direction*samplePoint.stepSize * 0.5f;

    /*
    float baseAngle = DCT_CalcBaseAngle(0,transmittanceEvalCount);
    DCTCoeffs sumCoeffs = DCT_CreateDCTCoeffs(transmittance, baseAngle);

    [loop]
    for (uint i = 1; i < transmittanceEvalCount; i++)
    {
        
        VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
        transmittance*= BeerTransmittance(voxelData.sigmaT * samplePoint.stepSize);
        baseAngle = DCT_CalcBaseAngle(i,transmittanceEvalCount);
        
        DCTCoeffs c = DCT_CreateDCTCoeffs(transmittance, baseAngle);
        sumCoeffs = DCT_CombineDCTCoeffs(sumCoeffs,c);

        samplePoint.position += samplePoint.direction * samplePoint.stepSize;
    }
    */
    
    // Vectorized version
    // Batch DCT every 4 samples

    
    float4 baseAngle4 = 0;
    float4 transmittance4 = 0;
    baseAngle4[0] = DCT_CalcBaseAngle(0,transmittanceEvalCount);
    DCTCoeffs sumCoeffs = (DCTCoeffs)0;
    transmittance4[0] = 1;
    
    VoxelData voxelData = Volume_SampleVoxelData(samplePoint);
    transmittance*= BeerTransmittance(voxelData.sigmaT * samplePoint.stepSize);
    transmittance4[1] = transmittance;
    baseAngle4[1] = DCT_CalcBaseAngle(1,transmittanceEvalCount);
    samplePoint.position += samplePoint.direction * samplePoint.stepSize;
    
    voxelData = Volume_SampleVoxelData(samplePoint);
    transmittance*= BeerTransmittance(voxelData.sigmaT * samplePoint.stepSize);
    transmittance4[2] = transmittance;
    baseAngle4[2] = DCT_CalcBaseAngle(2,transmittanceEvalCount);
    samplePoint.position += samplePoint.direction * samplePoint.stepSize;
    
    voxelData = Volume_SampleVoxelData(samplePoint);
    transmittance*= BeerTransmittance(voxelData.sigmaT * samplePoint.stepSize);
    transmittance4[3] = transmittance;
    baseAngle4[3] = DCT_CalcBaseAngle(3,transmittanceEvalCount);
    samplePoint.position += samplePoint.direction * samplePoint.stepSize;
    
    DCTCoeffs c = DCT_CreateDCTCoeffs(transmittance4, baseAngle4);
    sumCoeffs = DCT_CombineDCTCoeffs(sumCoeffs,c);
    
    [loop]
    for (uint i = 1; i < transmittanceEvalCount/4; i++)
    {
        [unroll]
        for (uint j = 0; j < 4; j++)
        {
            voxelData = Volume_SampleVoxelData(samplePoint);
            transmittance*= BeerTransmittance(voxelData.sigmaT * samplePoint.stepSize);
            transmittance4[j] = transmittance;
            baseAngle4[j] = DCT_CalcBaseAngle(4*i + j,transmittanceEvalCount);
            samplePoint.position += samplePoint.direction * samplePoint.stepSize;
        }
        c = DCT_CreateDCTCoeffs(transmittance4, baseAngle4);
        sumCoeffs = DCT_CombineDCTCoeffs(sumCoeffs,c);
    }
    
    sumCoeffs = DCT_NormalizeCoeffs(sumCoeffs, transmittanceEvalCount);
    return sumCoeffs;
}



PixelOutput Frag(Varyings input)  {

    float3 viewHit = GetWorldPosition(input.uv);
    BoundingSphere sphere = GetCloudVolumeBoundingSphere();

    
    float remappingFactor = 0.92;
    float volumeFactor = GetCloudVolumeUpperBound() * rcp(GetEarthRadius());
    float3 remappedCenter = sphere.center + remappingFactor * GetEarthRadius() * WorldVerticalDir();
    
    float polarRadiusFactor = sqrt(1 + (1 - remappingFactor*remappingFactor) * rcp(volumeFactor * (2 + volumeFactor)));
    float2 dirDiskPolar = OctahedronConcentric_SquareToConcentricDiskPolar(input.uv);
    
    dirDiskPolar.x *= rcp(polarRadiusFactor);
    float3 dirToVolume = OctahedronConcentric_GetHemiDirectionFromDiskPolar(dirDiskPolar);
    
    Ray rayToVolume;
    rayToVolume.direction = dirToVolume;//OctahedronConcentric_GetHemiDirectionFromSampleCoords(input.uv);
    rayToVolume.origin = remappedCenter;//sphere.center;// 0;
    rayToVolume.hitPoint = viewHit;
    Sphere lower = {sphere.center,sphere.radiusMin};
    Sphere upper = {sphere.center,sphere.radiusMax};
    SphereIntersectionResult result = CalcSphereIntersection(upper, rayToVolume);

    float3 mainLightDir = GetMainLight().direction;
    /*if (mainLightDir.y < 0){
        discard;
    }*/
    Ray rayFromSun;
    rayFromSun.direction = -mainLightDir;
    rayFromSun.origin = rayToVolume.origin + rayToVolume.direction * result.t1;
    rayFromSun.hitPoint = rayFromSun.direction*1000000;
 

    float4x4 planes = GetFrustumPlanesWSNoNearFar(_ViewProjectionMatrix);
    BoundsHitResult frustumPlanesHit = CalcFrustum4PlanesBoundsHit(planes, rayFromSun);
    float3 frustumPlanesHitPoint = rayFromSun.origin + rayFromSun.direction*frustumPlanesHit.distInsideBounds;
    
    DeclareParamFetch(CloudVolumeDesc, volumeDesc);
    
    bool insideVisibleVolume = (GetVertical(frustumPlanesHitPoint) > volumeDesc.heightLowerBound - 10
                    && GetVertical(frustumPlanesHitPoint) < volumeDesc.heightUpperBound + 10)
                    || false == FrustumCullVector(_ViewProjectionMatrix,normalize(rayFromSun.origin));
    if (insideVisibleVolume == false)
    {
        /*if (IsPixelSelectedInTile(input.position,uint2(2,1)) == false){
            discard;
        }*/
    }
    if (IsPixelSelectedInTile(input.position,uint2(2,2)) == false){
      //  discard;
    }
    SphereIntersectionResult result2 = CalcSphereIntersection(lower, rayFromSun);
    BoundsHitResult boundsHit;
    boundsHit.distInsideBounds = max(0,result2.t0);
    boundsHit.distToBounds = 0;
    
    
    uint iterationNormal = FetchParam(VolumeRayMarchingDesc).mainLightIteration;
    uint maxSampleCount = FetchParam(VolumeRayMarchingDesc).mainLightMaxSampleCount;
    SphereVolumeSampleConfig sampleConfig = GetSphereVolumeSampleConfig(boundsHit,sphere,iterationNormal,maxSampleCount);
    uint viewSampleCount = sampleConfig.directionSampleCount;
    RayMarchingContext rayMarchingContext = GetRayMarchingContext(rayFromSun,boundsHit,viewSampleCount);
    
    DCTCoeffs coeffs = RayMarchingCalcTransmittanceDCT(rayMarchingContext);

    DCTCoeffsLutEntry entry0 = DCTShadowMap_PackToDCTCoeffsLutEntry(coeffs, 0);
    DCTCoeffsLutEntry entry1 = DCTShadowMap_PackToDCTCoeffsLutEntry(coeffs, 1);
    DCTCoeffsLutEntry entry2 = DCTShadowMap_PackToDCTCoeffsLutEntry(coeffs, 2);
    DCTCoeffsLutEntry entry3 = DCTShadowMap_PackToDCTCoeffsLutEntry(coeffs, 3);
    
    PixelOutput output;
    output.output0 = DCTShadowMap_GetPackedValue(entry0);
    output.output1 = DCTShadowMap_GetPackedValue(entry1);
    output.output2 = DCTShadowMap_GetPackedValue(entry2);
    output.output3 = DCTShadowMap_GetPackedValue(entry3);
    return output;
}



#endif
```
