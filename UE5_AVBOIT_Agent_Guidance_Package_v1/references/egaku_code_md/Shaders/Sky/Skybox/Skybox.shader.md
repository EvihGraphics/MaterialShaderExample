# Egaku source: `Shaders/Sky/Skybox/Skybox.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
Shader "MyCustom/Sky/Skybox"
{
	Properties
	{
		
		[Header(Skybox Settings)]
		_Skybox_sunLightIntensity("Sun Light Intensity", Float) = 0.0
	}
	
	HLSLINCLUDE
		#define UNIVERSAL_RENDER_PIPELINE 
		#include "Skybox.ParamTable.hlsl"
	ENDHLSL
	
	SubShader
	{
		Tags
		{
			"RenderPipeline" = "UniversalPipeline"
		}
		
		Pass
		{
			
			Cull Off
			ZWrite Off
			
			HLSLPROGRAM
				#include "Skybox.hlsl"
				
				#pragma target 4.0
				#pragma vertex Vert
				#pragma fragment Frag
			ENDHLSL
		}
		
	}
}
```
