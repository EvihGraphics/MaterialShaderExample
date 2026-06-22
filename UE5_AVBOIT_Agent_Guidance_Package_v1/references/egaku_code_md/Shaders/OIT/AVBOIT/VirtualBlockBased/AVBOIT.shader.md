# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/AVBOIT.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
Shader "MyCustom/OIT/AVBOIT/VirtualBlockBased"
{
	Properties
	{
		
		[Header(Translucency Settings)]
		_Translucency_baseColor("Base Color", Color) = (0, 0, 0, 0)
	}
	
	HLSLINCLUDE
		#define UNIVERSAL_RENDER_PIPELINE 
		#include "AVBOIT.ParamTable.hlsl"
	ENDHLSL
	
	SubShader
	{
		Tags
		{
			"RenderPipeline" = "UniversalPipeline"
		}
		
		Pass
		{
			Name "Splatting"
			
			Cull Back
			ZWrite Off
			ColorMask 0
			
			HLSLPROGRAM
				#include "Passes/SplattingPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "TranslucencyForwardPrePass"
			
			Cull Back
			ZWrite Off
			Blend DstAlpha One,One Zero
			ColorMask A
			
			HLSLPROGRAM
				#include "Passes/TranslucencyForwardPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "TranslucencyForward"
			
			Cull Back
			ZWrite Off
			Blend One One
			
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
			
			Cull Back
			ZWrite Off
			Blend One One, One Zero
			
			HLSLPROGRAM
				#include "Passes/BackgroundResolutionPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
		Pass
		{
			Name "VolumeOccupancyCollection"
			
			Cull Back
			ZWrite Off
			Conservative True
			ColorMask 0
			
			HLSLPROGRAM
				#include "Passes/VolumeOccupancyCollectionPass.hlsl"
				
				#pragma target 5.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
	}
}
```
