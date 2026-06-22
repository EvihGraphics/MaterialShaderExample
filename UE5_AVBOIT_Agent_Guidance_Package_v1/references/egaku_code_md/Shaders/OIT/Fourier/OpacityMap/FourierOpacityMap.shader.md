# Egaku source: `Shaders/OIT/Fourier/OpacityMap/FourierOpacityMap.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
Shader "MyCustom/OIT/Fourier/OpacityMap"
{
	Properties
	{
		
		[Header(Translucency Settings)]
		_Translucency_baseColor("Base Color", Color) = (0, 0, 0, 0)
	}
	
	HLSLINCLUDE
		#define UNIVERSAL_RENDER_PIPELINE 
		#include "FourierOpacityMap.ParamTable.hlsl"
	ENDHLSL
	
	SubShader
	{
		Tags
		{
			"RenderPipeline" = "UniversalPipeline"
		}
		
		Pass
		{
			Name "FourierCoeffsBuilding"
			
			Cull Off
			ZWrite Off
			ZTest Off
			Blend One One
			
			HLSLPROGRAM
				#include "Passes/FourierCoeffsBuildingPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "TranslucencyForward"
			
			Cull Off
			ZWrite Off
			Blend One One,One Zero
			
			HLSLPROGRAM
				#include "Passes/TranslucencyForwardPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "BackgroundResolution"
			
			Cull Off
			ZWrite Off
			Blend One One,One Zero
			
			HLSLPROGRAM
				#include "Passes/BackgroundResolutionPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
	}
}
```
