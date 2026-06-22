# Egaku source: `Shaders/Sky/Atmosphere/Atmosphere.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
Shader "MyCustom/Sky/Atmosphere"
{
	Properties
	{
		
		[Header(Atmosphere Volume Settings)]
		_AtmosphereVolume_atmosphereDensityScale("Atmosphere Density Scale", Float) = 0.0
		_AtmosphereVolume_dustDensityFraction("Dust Density Fraction", Range(0,1)) = 0
		_AtmosphereVolume_airDensityFraction("Air Density Fraction", Range(0,1)) = 0
		_AtmosphereVolume_ozoneDensityFraction("Ozone Density Fraction", Range(0,1)) = 0
		_AtmosphereVolume_mieScatteringAnisotropy("Mie Scattering Anisotropy", Range(-0.999,0.999)) = -0.999
	}
	
	HLSLINCLUDE
		#define UNIVERSAL_RENDER_PIPELINE 
		#include "Atmosphere.ParamTable.hlsl"
	ENDHLSL
	
	SubShader
	{
		Tags
		{
			"RenderPipeline" = "UniversalPipeline"
		}
		
		Pass
		{
			Name "AtmospherePrtPass"
			
			Cull Off
			ZWrite Off
			
			HLSLPROGRAM
				#include "Atmosphere.OctahedronPrtPass.hlsl"
				
				#pragma target 4.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "TransmittanceLutPass"
			
			Cull Off
			ZWrite Off
			
			HLSLPROGRAM
				#include "Atmosphere.TransmittanceLutPass.hlsl"
				
				#pragma target 4.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
	}
}
```
