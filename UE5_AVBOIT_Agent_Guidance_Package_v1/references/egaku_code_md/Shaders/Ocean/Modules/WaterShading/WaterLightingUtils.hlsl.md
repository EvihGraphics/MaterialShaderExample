# Egaku source: `Shaders/Ocean/Modules/WaterShading/WaterLightingUtils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef WATER_LIGHTING_UTLIS_HLSL
#define WATER_LIGHTING_UTLIS_HLSL

#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/DeclareOpaqueTexture.hlsl"
#include "Assets/Shaders/HLSLInclude/CommonShaderUtils.hlsl"

float3 TransmissionColorToAbsorption(float3 color,float depth)
{
	return -log(color) * (1/depth);
}
	

float3 Transmittance(float depth,float3 extinctionCoeff)
{
	return exp(-extinctionCoeff * depth);
}

float3 Refraction(float3 background,float depth, float depthScale,float3 absorption)
{
	float3 tr = Transmittance(clamp( depthScale* depth,0,1000),absorption);
	//	return opaqueDepthLinearEye;
	return tr * background;
}

float3 SchlickFresnel(float3 normal,float3 direction,float indexOfRefraction)
{
	float3 f90 = float3(1,1,1);
	float f0 = (indexOfRefraction - 1) /(indexOfRefraction + 1);
	f0 *= f0;
	float factor = pow(1.0 - clamp(dot(normal, direction),0,1), 5);
	return lerp(float3(f0,f0,f0),f90, factor);
}

float3 SchlickFresnelF0(float indexOfRefraction)
{
	float f0 = (indexOfRefraction - 1) /(indexOfRefraction + 1);
	f0 *= f0;
	return float3(f0,f0,f0);
}

float3 GetApproximateDeflectedRefractionRayHitPoint(float3 refractionPoint,float3 rayHitPoint,float3 geometryNormal,float deflectionAngle)
{
		float3 rayVec = rayHitPoint - refractionPoint;
		float3 hitPointShiftDir = normalize(rayVec - dot(rayVec,geometryNormal) * geometryNormal);
		float shiftDist = length(rayVec) * tan(deflectionAngle);
		return rayHitPoint + shiftDist * hitPointShiftDir;
}



	float3 GetRefractionRayHitPoint(float3 refractionPoint,float3 viewDir,float3 normal,float indexOfRefraction,float maxRayMarchDistance)
	{

		float3 refractDir = refract(viewDir, normal, indexOfRefraction);
		if (all(abs(refractDir) < 0.0001))
		{
			return float3(0,0,0);
		}
		refractDir = normalize(refractDir);
		float3 rayOrigin = refractionPoint;
	
		float3 hitPointUB = rayOrigin;// + refractDir * maxRayMarchDistance;
		float3 hitPointLB = rayOrigin;
		float epsilon = 0.15;
		float3 hitPoint = hitPointLB  + (hitPointUB - hitPointLB)*0.5;
		for (int i = 0; i < 25;i++)
		{
			float4 hitPointSS = TransformWorldToScreenspace(hitPoint);
			float opaqueDepth = GetLinearEyeDepthFromScene(hitPointSS.xy);
			float hitPointDepth = GetLinearEyeDepthFromRawZ(hitPointSS.z);
			float depthDelta =  opaqueDepth - hitPointDepth;
			if (depthDelta > 0 + epsilon){
				hitPointLB = hitPoint;
				hitPoint = hitPointLB + depthDelta*refractDir;
				hitPointUB = hitPoint;
			}else if (depthDelta < 0 - epsilon){
				break;
			}else{
				//return float3(1,0,0);
				return hitPoint;
			}
		}
		for (int i = 0; i < 25;i++)
		{
			hitPoint = hitPointLB + (hitPointUB - hitPointLB)*0.5;
			float4 hitPointSS = TransformWorldToScreenspace(hitPoint);
			float opaqueDepth = GetLinearEyeDepthFromScene(hitPointSS.xy);
			float hitPointDepth = GetLinearEyeDepthFromRawZ(hitPointSS.z);
			float depthDelta =  opaqueDepth - hitPointDepth;
			if (all(abs(hitPointUB - hitPointLB))<0.05)
			{
				return hitPoint;
			}
			if (depthDelta > 0 + epsilon){
				hitPointLB = hitPoint;
			}else if (depthDelta < 0 - epsilon){
				hitPointUB = hitPoint;
			}else{
				return hitPoint;
			}
		}
		//return hitPoint;
		return  float3(0,1,0);
	}

	float GetDeflectionAngle(float3 viewDir,float3 originalNormal,float3 deflectedNormal,float indexOfRefraction)
	{
		float3 originalReflection = refract(viewDir, originalNormal, 1.0/indexOfRefraction);
		float3 deflectedReflection = refract(viewDir, deflectedNormal, 1.0/indexOfRefraction);
		float deflectionAngle = acos(clamp(-1.0,1.0,dot(originalReflection, deflectedReflection)));
		if (dot(viewDir,originalNormal) < dot(viewDir,deflectedNormal))
		{
			return -deflectionAngle;
		}
		return deflectionAngle;
	}

	float GetReflectDeflectionAngle(float3 viewDir,float3 originalNormal,float3 deflectedNormal)
	{
		float3 originalReflection = reflect(viewDir, originalNormal);
		float3 deflectedReflection = reflect(viewDir, deflectedNormal);
		float originalReflectAngle = acos(clamp(-1.0,1.0,dot(originalNormal, originalReflection)));
		float deflectedReflectAngle = acos(clamp(-1.0,1.0,dot(originalNormal, deflectedReflection)));
		float deflectionAngle = deflectedReflectAngle - originalReflectAngle;
		return deflectionAngle;
	}


	float3 GetApproximateDeflectedReflectionRayHitPoint(float3 reflectionPoint,float3 rayVec,float3 normal,float deflectionAngle,float intensity)
	{
		float3 hitPointShiftDir = normalize(rayVec - dot(rayVec , normal) * normal);
		float shiftDist = intensity * tan(deflectionAngle);
		return reflectionPoint + shiftDist * hitPointShiftDir;
	}

half BRDF_DTerm(float NdotH, float roughness) {
	//DGGX =  a^2 / π((a^2 – 1) (n · h)^2 + 1)^2
	float a2 = roughness * roughness;
	float val = ((a2 - 1) * (NdotH * NdotH) + 1);
	return a2 / (PI * (val * val));
}

half BRDF_GTerm(float NdotL, float NdotV, float roughness) {
	//G(l,v,h)=1/(((n·l)(1-k)+k)*((n·v)(1-k)+k))
	float a = 1 + roughness * roughness;
	float k = a*a / 8;
	return NdotL * NdotV / ((NdotL * (1 - k) + k) + (NdotV * (1 - k) + k));
}





half3 BRDF_FresnelTerm(half3 F0, float HdotV) {
	//F(l,h) = F0+(1-F0)(1-l·h)^5
	return F0 + (1 - F0) * pow(1 - HdotV, 5);
}

half3 BRDF_FresnelTerm_Lerp(half3 f0,half3 f90, float HdotV) {
	float factor = pow(1 - HdotV, 5);
	return lerp(f0,f90, factor);
}

half3 custom_FresnelLerp(half3 F0, half3 F90, half cosA, float fresnelPow)
{
	half t = pow(1 - cosA, fresnelPow);   // ala Schlick interpoliation
	return lerp(F0, F90, t);
}

struct BRDFContext
{
	float3 f0;
	float3 albedo;
	float ks;
	float metallic;
	float roughness;
	float roughness2;
	float nl;
	float nv;
	float nh;
	float hv;
};

BRDFContext GetBRDFContext(float roughness,float specular,float metallic,float3 normal,float3 lightDir,float3 viewDir,float3 f0,float3 albedo)
{
	BRDFContext context;
	float3 halfVec = normalize(lightDir + viewDir);
	context.roughness = roughness;
	context.roughness2 = roughness * roughness;
	context.f0 = f0;
	context.albedo = albedo;
	context.nl = max(0.001f,saturate(dot(lightDir, normal)));
	context.nv = max(0.001f,saturate(dot(viewDir, normal)));
	context.nh = max(0.001f,saturate(dot(halfVec, normal)));
	context.hv = max(0.001f,saturate(dot(halfVec, viewDir)));
	context.metallic = metallic;
	context.ks = specular;
	return context;
}

float3 BRDF_Specular(BRDFContext context)
{
	// D
	half d = BRDF_DTerm(context.nh, context.roughness);  
	// G
	half g = BRDF_GTerm(context.nl, context.nv, context.roughness);  
	// F 
	half3 f = BRDF_FresnelTerm(context.f0, context.hv);
	float specularTerm = f * d * g / (4 * context.nl * context.nv); 
	
	specularTerm = max(0, specularTerm);  
	return specularTerm;
}


float3 BRDF_Specular_Env(BRDFContext context)
{
	// D
	half d = BRDF_DTerm(context.nh, context.roughness);  
	// G
	half g = BRDF_GTerm(context.nv, context.nv, context.roughness);  
	// F 
	half3 f = BRDF_FresnelTerm(context.f0, context.hv);
	float specularTerm = f * d * g / (4 * context.nv * context.nv); 
	
	specularTerm = max(0, specularTerm);  
	return specularTerm;
}

float3 LightColorToLightIntensity(float3 lightColor)
{
	return PI * lightColor;
}

float3 BRDF_Diffuse(float3 albedo)
{
	return albedo / PI;
}

float3 Lighting_CookTorrance_Direct(BRDFContext context,float3 lightColor)
{
	float3 ks = context.ks;
	float3 kd = max(0,(1 - ks) * (1 - context.metallic));
	return (kd * BRDF_Diffuse(context.albedo) + ks * BRDF_Specular(context)) * PI * lightColor * context.nl;
}

float3 Lighting_Reflect(BRDFContext context,float3 reflectColor)
{
	context.hv = max(0.05,context.nv); // prevent unexpected gray color caused by minimal value
	context.nl = max(0.05,context.nv);
	context.nh = 1;
	context.albedo = 0;
	return Lighting_CookTorrance_Direct(context,reflectColor);
}

float3 Lighting_EnvSH(BRDFContext context,float3 shColor)
{
	float3 ks = context.ks;
	float3 kd = (1 - ks) * (1 - context.metallic);
	return kd * context.albedo * shColor;
}


float3 Lighting_LambertEnv(BRDFContext context,float3 envColor)
{
	return BRDF_Diffuse(context.albedo) * PI * envColor;
}

	
float4 MixedSample(sampler2D map,float2 uv,float2 velocity,float time)
{
	float t = time*4.0f;
	float loop1 = cos(t)*0.5f+0.5f;
	float loop2 = cos(t+1/3.0f*TWO_PI)*0.5f+0.5f;
	float loop3 = cos(t+2/3.0f*TWO_PI)*0.5f+0.5f;
	float2 uv1 = uv - velocity*loop1;
	float2 uv2 = uv - velocity*loop2 + float2(0.1,0.1);
	float2 uv3 = uv - velocity*loop3 - float2(0.1,0.1);
	float4 sample1 = tex2Dlod(map,float4(uv1,0,0));
	float4 sample2 = tex2Dlod(map,float4(uv2,0,0));
	float4 sample3 = tex2Dlod(map,float4(uv3,0,0));
	float weight = 1.0f/3.0f;
	float4 result = sample1*weight + sample2*weight + sample3*weight;
	return result;
}

float4 FlowSample(sampler2D map,float2 uv,float2 velocity,float time)
{
	float t = time*4.0f;
	float4 sample1 = tex2Dlod(map,float4(uv - t*velocity,0,0));
	return sample1;
}


#endif
```
