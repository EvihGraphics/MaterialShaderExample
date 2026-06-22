# Egaku source: `Shaders/Sky2/Cloud/HLSLInclude/cloud_LightingUtils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CLOUD_LIGHTING_UTILS_HLSL
#define CLOUD_LIGHTING_UTILS_HLSL


float3 BeerLambertTransmittance(float3 density) {
    return exp(-density);
}

float BeerLambertTransmittance(float density) {
    return exp(-density);
}

float3 ExpAttenuateIntensity(float3 intensity, float3 factor) {
    return intensity * exp(-factor);
}

float3 BeerPowderAttenuate(float3 factor, float a) {
    float3 t = exp(-factor * a) * (1 - exp(-factor * 2 * a));
    return t*2.5980762113533159402911695122588;
    return t;
}
  
float SchlickPhaseFunction(float cosTheta, float g) {
    /* Schlick phase function, an approximation of Hg phase function */
    float k = 1.55 * g - 0.55 * g * g * g;
    return (1.0 - k * k) / (12.56637 * pow(1 - k * cosTheta, 2));
}

float HgPhaseFunction(float cosTheta, float g) {
    /* Hg Phase Function */
                    
    float g2 = g * g;
    return (1 - g2) / (12.56637 * pow(1 + g2 - 2 * g * cosTheta, 1.5));
}

float GetPhaseAngle(float3 lightDir,float3 viewDir)
{
    return max(0,dot(-lightDir, viewDir));
}

// calculate phase function for cloud rendering.
// be careful about the theta angle's definition!
// be careful about the definition of light direction and view direction!
// view direction: vector that points to the camera
// light direction: vector that points to the light source
// theta angle: the angle between light shooting direction and the direction that pointing to the camera.
float CloudPhaseFunction(float3 lightDir,float3 viewDir,float phaseG1,float phaseG2)
{
    float cosTheta = GetPhaseAngle(lightDir, -viewDir);
    float phaseVal = lerp(HgPhaseFunction(cosTheta, phaseG1), HgPhaseFunction(cosTheta, phaseG2), 0.5);
    return phaseVal;
}

float CloudPhaseFunction(float cosTheta,float phaseG1,float phaseG2)
{
    float phaseVal = lerp(HgPhaseFunction(cosTheta, phaseG1), HgPhaseFunction(cosTheta, phaseG2), 0.5);
    return phaseVal;
}

float3 LightExtinctCoeff(float density,float3 sigmaExt)
{
  //  return density*sigmaExt;
    return max(density*sigmaExt, 1e-8f);
}

float3 LightScatterCoeff(float density,float3 sigmaScatter)
{
    return density*sigmaScatter;
}

// Exponential Integral 
// (http://en.wikipedia.org/wiki/Exponential_integral) 
float Ei(float z )
{
    return 0.5772156649015328606065 + log( 1e-4 + abs(z) ) + z * (1.0 + z * (0.25 + z * ( (1.0/18.0) + z * ( (1.0/96.0) + z * 
    (1.0/600.0) ) ) ) ); // For x!=0 
}

// Ambient Light Algorithm from Revision 2013 - Real-time Volumetric Rendering Course Notes
float3  ComputeAmbientColor(float volumeTopHeight,float volumeBtmHeight,float3 pos, float3 extCoeff,
    float3 isotropicLightTop, float3 isotropicLightBottom){
    float ht = volumeTopHeight - pos.y;   // Height to the top of the volume 
    float3 a = -extCoeff * ht; 
    float3 isotropicScatteringTop = isotropicLightTop * max(0.0, exp(a) - a*Ei(a)); 
    float hb = pos.y - volumeBtmHeight;   // Height to the bottom of the volume 
    a = -extCoeff * hb; 
    float3 isotropicScatteringBottom = isotropicLightBottom * max(0.0, exp(a) - a*Ei(a)); 
    return isotropicScatteringTop + isotropicScatteringBottom; 
}

float3 OctaveScatterLightExtinctCoeff(float density,float3 sigmaExt,float factor,int order){
    return LightExtinctCoeff(density,sigmaExt*pow(factor, order));
}

float3 OctaveScatterLightScatterCoeff(float density,float3 sigmaScatter,float factor,int order){
    return LightScatterCoeff(density,sigmaScatter*pow(factor, order));
}

float3 OctaveScatterLightSigmaScatter(float3 sigmaScatter,float factor,int order){
    return sigmaScatter*pow(factor, order);
}

float3 OctaveScatterLightSigmaExtinct(float3 sigmaExt,float factor,int order){
    return sigmaExt*pow(factor, order);
}

float OctaveMultiScatterCloudPhaseFunction(float cosTheta,float phaseG1,float phaseG2,float factor,int order)
{
    return CloudPhaseFunction(cosTheta*pow(factor,order), phaseG1, phaseG2);
}

float3 CalcLightEnergyOfPoint(float sunPhase,float3 sunColor,
    float ambientPhase,float3 ambientColor)
{
    float3 energy = (sunPhase * sunColor + ambientPhase * ambientColor);
    return energy;
}






#endif
```
