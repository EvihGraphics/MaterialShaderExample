# Egaku source: `Shaders/Ocean/Ocean.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#pragma once
#ifndef OCEAN_HLSL
#define OCEAN_HLSL
#include "Modules/OceanSurface.hlsl"
#include "Modules/WaterShading/WaterLightingUtils.hlsl"
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/MaterialModel/SingleLayerWater.hlsl"
#include "Modules/Terrain/OpaqueScene.hlsl"
#include "Modules/Reflection/ReflectionTexture.hlsl"
#include "Modules/WaterShading/WaterSurfaceShading.hlsl"
#include "Ocean.Mesh.hlsl"
#include "Ocean.SH.hlsl"
#include "Ocean.AtmosphereTransmittanceLut.hlsl"
#include "Ocean.ParamStruct.hlsl"

//#pragma enable_d3d11_debug_symbols


struct Attributes
{
    float2 positionOS : POSITION;
    uint instanceID : SV_InstanceID;
};

struct OceanAttributes
{
    float4 positionOS : POSITION;
    float2 uv : TEXCOORD0;
};

struct Varyings{
    float4 position : SV_POSITION;
    float4 varyingData0 : TEXCOORD0;
    float4 varyingData1 : TEXCOORD1;
    float4 varyingData2 : TEXCOORD2;
    float4 varyingData3 : TEXCOORD3;
};

struct OceanVaryings
{
    float2 uv;
    float2 velocity;
    float3 positionWS;
    float3 normalWS;
    float4 position;
    float3 color;
    float foamIntensity;
    float nearFarBlendFactor;
};

OceanVaryings UnpackVaryings(Varyings input)
{
    OceanVaryings output;
    output.position = input.position;
    output.uv = input.varyingData0.xy;
    output.velocity = input.varyingData0.zw;
    output.positionWS = input.varyingData1.xyz;
    output.foamIntensity = input.varyingData1.w;
    output.normalWS = input.varyingData2.xyz;
    output.nearFarBlendFactor = input.varyingData2.w;
    output.color = input.varyingData3.xyz;
    return output;
}

Varyings PackVaryings(OceanVaryings input)
{
    Varyings output;
    output.position = input.position;
    output.varyingData0 = float4(input.uv,input.velocity);
    output.varyingData1 = float4(input.positionWS,input.foamIntensity);
    output.varyingData2 = float4(input.normalWS,input.nearFarBlendFactor);
    output.varyingData3 = float4(input.color,0.f);
    return output;
}

OceanAttributes SetupAttributes(Attributes a)
{
    OceanAttributes oceanAttr;
    oceanAttr.positionOS = OceanMesh_GetVertexPositionOS(a.positionOS, a.instanceID);
    oceanAttr.uv = a.positionOS.xy;
    SetupModelMatrix();
    return oceanAttr;
}


Varyings Vert(Attributes input)
{
    OceanAttributes oceanAttr = SetupAttributes(input);
    
    float3 positionWS = TransformObjectToWorld(oceanAttr.positionOS);
    float2 positionWS_Horizontal = GetHorizontal(positionWS);
    OceanTerrainContext terrainContext = SampleOceanTerrainMap(positionWS_Horizontal);
    OceanSurfaceData surfaceData = CalcOceanSurfaceData(positionWS_Horizontal,terrainContext);
    float3 displacedPositionWS = positionWS + surfaceData.displacementVec;
    OceanVaryings output;
    //GetCameraPositionWS()
    output.positionWS = displacedPositionWS;
    output.uv = positionWS_Horizontal;
    output.velocity = surfaceData.velocity;
    output.normalWS = surfaceData.normal;
    output.foamIntensity = surfaceData.foam;
    output.position = TransformWorldToHClip(displacedPositionWS);
    output.color = surfaceData.debug;
    output.nearFarBlendFactor = CalcWaterColorBlendFactor(terrainContext);
    Varyings packedOutput = PackVaryings(output);

  //  output.color = terrainContext.debugTerrain;
    return packedOutput;
}



float3 Shade(WaterShadingContext shadingContext,Light light,float2 sceneUV,float underwaterRayDistance)
{
    DeclareParamFetch(WaterSurfaceShadingDesc, surfaceDesc);
    WaterShadingData shadingData = CalcWaterSurfaceShadingData(shadingContext);
   // return shadingContext.pixelNormalWS;
   // return shadingData.overlayColor;
    //return shadingData.specular;
    //FoamShadingData foamShadingData = CalcFoamShadingData(shadingContext);
  //  return shadingData.overlayColor;
    // modulate the behind-water color to create wet color of underwater terrain.
    float3 waterBehind = SampleOpaqueSceneColor(sceneUV) * shadingData.behindColor;

    float3 sunTransmittance = 0;
    if (GetVertical(light.direction) > 0)
    {
        sunTransmittance = LookupAtmosphereDirectionTransmittance(0,light.direction);
    }
    SingleLayerWaterDesc waterDesc;
    waterDesc.absorptionCoeff = shadingData.absorption;
    waterDesc.scatterCoeff = shadingData.scattering;
    waterDesc.behindWaterSceneLuminance = waterBehind;
    waterDesc.phaseG = shadingData.phaseG;
    waterDesc.waterVisibility = 1.f - shadingData.overlayOpacity;
    waterDesc.viewDirection = shadingContext.viewDir;
    waterDesc.mainLightIntensity = sunTransmittance * LightColorToLightIntensity(light.color);
    waterDesc.mainLightDirection = light.direction;
    waterDesc.underwaterRayDistance = underwaterRayDistance;
    waterDesc.environLightIntensity = 1;
    SingleLayerWaterOutput output = SingleLayerWater(waterDesc);
    float ior = 1.33;
    float3 f0 = SchlickFresnelF0(ior);
    float3 reflection = SampleReflectionTexture(sceneUV);
    BRDFContext brdf = GetBRDFContext(shadingData.roughness,shadingData.specular,
        0,shadingData.pixelNormalWS,light.direction,
        shadingContext.viewDir,f0,shadingData.overlayColor);
    
    float3 directLighting = Lighting_CookTorrance_Direct(brdf,light.color);
  //  brdf.ks = surfaceDesc.reflectionSpecular;
    BRDFContext brdfReflect = brdf;
    brdfReflect.ks = surfaceDesc.reflectionSpecular;
    float3 envLighting = Lighting_Reflect(brdfReflect,reflection);
    float3 envDiffSHColor = SH_DiffuseIndirect(shadingData.pixelNormalWS);
    float3 envDiff = Lighting_EnvSH(brdf,envDiffSHColor);
    float3 volumeLighting = output.luminance;
   // return shadingData.overlayColor;
  //  return shadingData.pixelNormalWS;
    return saturate(shadingData.overlayOpacity/0.5) * (directLighting + envLighting + envDiff) + volumeLighting;
}

float3 VisualizeNormal(float3 normal)
{
    return normal.xzy * 0.5 + 0.5;
}

float4 Frag(Varyings packedInput) : SV_Target
{
    OceanVaryings input = UnpackVaryings(packedInput);
   // return float4(input.color,1);
    WaterShadingContext shadingContext;
    shadingContext.vertexNormalWS = WorldVerticalDir();
    float visualDepth = GetLinearEyeDepthFromRawZ(input.position.z);
    float normalAttenuate =  Remap(min(visualDepth,500),0,500,0,1);
    normalAttenuate = 0;
    shadingContext.pixelNormalWS = normalize(lerp(normalize(input.normalWS), WorldVerticalDir(),normalAttenuate));
    shadingContext.viewDir = normalize(GetWorldSpaceViewDir(input.positionWS));
    Light light = GetMainLight();
    shadingContext.mainLightDir = light.direction;
    shadingContext.foamIntensity = input.foamIntensity;
    shadingContext.nearFarBlendFactor = input.nearFarBlendFactor;
    shadingContext.positionWS = input.positionWS;
    shadingContext.velocity = input.velocity;
    shadingContext.uv = input.uv;
    float2 sceneUV = GetScreenUV(input.position) + GetUVDistortion(WorldVerticalDir(), input.normalWS, 1.33f)/20;
    float dist = GetOpaqueSceneLinearEyeDepth(sceneUV) - visualDepth;
    dist = max(0.01,dist);
    float3 color = Shade(shadingContext,light,sceneUV,dist);
//    color = input.nearFarBlendFactor;
   // color = VisualizeNormal(shadingContext.pixelNormalWS);
 //   color = input.foamIntensity;
    return float4(color,1.f);

}
#endif
```
