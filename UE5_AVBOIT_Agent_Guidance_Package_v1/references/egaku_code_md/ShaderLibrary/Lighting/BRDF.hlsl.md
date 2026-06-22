# Egaku source: `ShaderLibrary/Lighting/BRDF.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#pragma once
#ifndef BRDF_HLSL
#define BRDF_HLSL
#include "../Typedef.hlsl"
#include "../MathUtils.hlsl"
// Physically based shading model
// parameterized with the below options
// [ Karis 2013, "Real Shading in Unreal Engine 4" slide 11 ]

// E = Random sample for BRDF.
// N = Normal of the macro surface.
// H = Normal of the micro surface.
// V = View vector going from surface's position towards the view's origin.
// L = Light ray direction

// D = Microfacet NDF
// G = Shadowing and masking
// F = Fresnel

// Vis = G / (4*NoL*NoV)
// f = Microfacet specular BRDF = D*G*F / (4*NoL*NoV) = D*Vis*F

// Diffuse

real3 Diffuse_Lambertian(real3 albedo)
{
	return albedo * INV_PI;
}

// Microfacet specular BRDF

// [Blinn 1977, "Models of light reflection for computer synthesized pictures"]
real D_Blinn( real a2, real NoH )
{
	real n = 2 / a2 - 2;
	return (n + 2) / TWO_PI * SafePow( NoH, n );		// 1 mad, 1 exp, 1 mul, 1 log
}

// [Beckmann 1963, "The scattering of electromagnetic waves from rough surfaces"]
real D_Beckmann( real a2, real NoH )
{
	real NoH2 = NoH * NoH;
	return exp( (NoH2 - 1) / (a2 * NoH2) ) / ( PI * a2 * NoH2 * NoH2 );
}

// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough surfaces"]
real D_GGX( real a2, real NoH )
{
	real d = ( NoH * a2 - NoH ) * NoH + 1;	// 2 mad
	return a2 / ( PI * Sq(d));					// 4 mul, 1 rcp
}

// Anisotropic GGX
// [Burley 2012, "Physically-Based Shading at Disney"]
real D_GGX_Aniso( real ax, real ay, real NoH, real XoH, real YoH )
{
	real a2 = ax * ay;
	real3 v = real3(ay * XoH, ax * YoH, a2 * NoH);
	real s = dot(v, v);
	return INV_PI * a2 * Square(a2 / s);
}


real3 F_None( real3 f0 )
{
	return f0;
}

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
real3 F_FastSchlick( real3 f0, real VoH )
{
	real Fc = Pow5( 1 - VoH );					// 1 sub, 3 mul
	//return Fc + (1 - Fc) * SpecularColor;		// 1 add, 3 mad
	
	// Anything less than 2% is physically impossible and is instead considered to be shadowing
	return saturate( 50.0 * f0.g ) * Fc + (1 - Fc) * f0;
}

real3 F_Schlick(real3 f0, real3 f90, real VoH)
{
	real c = Pow5(1 - VoH);
	return f90 * c + (1 - c) * f0;
}

real3 F_Schlick(real3 f0, real VoH)
{
	return F_Schlick(f0,1.0, VoH);
}

real F_F0(real ior1, real ior2)
{
	real f = (ior1 - ior2) / (ior1 + ior2);
	return Sq(f);
}

real3 F_F0(real ior)
{
	return F_F0(ior,1);
}


half G_GGX(float NdotL, float NdotV, float roughness) {
	//G(l,v,h)=1/(((n·l)(1-k)+k)*((n·v)(1-k)+k))
	float a = 1 + roughness * roughness;
	float k = a*a / 8;
	return NdotL * NdotV / ((NdotL * (1 - k) + k) + (NdotV * (1 - k) + k));
}

// Tuned to match behavior of Vis_Smith
// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
float Vis_Schlick(float roughness2, float NoV, float NoL )
{
	float k = sqrt(roughness2) * 0.5;
	float Vis_SchlickV = NoV * (1 - k) + k;
	float Vis_SchlickL = NoL * (1 - k) + k;
	return 0.25 / ( Vis_SchlickV * Vis_SchlickL );
}

real3 SpecularColor(real ior)
{
	return F_F0(ior);
}





#endif
```
