# Egaku source: `Shaders/Skybox2/Skybox.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿Shader "Custom/Shaders/Skybox"
{
    Properties
    {
	    _SunSize ("Sun Size", Range(0,1)) = 0.04
        _AtmosphereThickness ("Atmoshpere Thickness", Range(0,5)) = 1.0
        _SkyTint ("Sky Tint", Color) = (.5, .5, .5, 1)
        _GroundColor ("Ground", Color) = (.369, .349, .341, 1)
        _Exposure("Exposure", Range(0, 8)) = 1.3
    }


    HLSLINCLUDE

	sampler2D _Cloud;
	sampler2D _AtmospherePRT;
	float _HemiOctahedron;
    
	struct a2v
	{
		float4 vertex : POSITION;
	};

	struct v2f
	{
		float4 pos : SV_POSITION;
		float4 screenPos : TEXCOORD5;
		float3 rayDir : TEXCOORD1;
		float3 groundColor : TEXCOORD2;
		float3 skyColor : TEXCOORD3;
		float3 sunColor : TEXCOORD4;
		float3 vertex : TEXCOORD6;
	};


	#include "ProceduralSky.cginc"
	

	float2 VectorToHemiOctahedron(float3 N)
	{
		N.xz /= dot(1.0, abs(N));
		return float2(N.x + N.z, N.x - N.z) * 0.5 + 0.5;
	}


	v2f vert(a2v v)
	{
		v2f o;

		o.pos = UnityObjectToClipPos(v.vertex);
		o.screenPos = ComputeScreenPos(o.pos);
		o.rayDir = normalize(mul((float3x3)unity_ObjectToWorld, v.vertex.xyz));//世界坐标当方向
		o.vertex = v.vertex;
		vert_sky(o);

		return o;
	}

    float4 frag(v2f i) : SV_Target
    {
    	
    	float4 cloud;
    	float4 atmosphere;
    	//return float4(1,1,0,1);
    	if(_HemiOctahedron > 0)
    	{
    		float2 uv = VectorToHemiOctahedron(normalize(i.vertex));
    		cloud = tex2D(_Cloud, uv);
			atmosphere = tex2D(_AtmospherePRT, uv);
    	//	return float4(cloud.rgba);
		//	return float4(cloud.rgb,1);
    		if(i.vertex.y < 0)
    		{
    		
    			cloud = 0;
    			atmosphere = 0;
    		}
    		
    	}
    	else
    	{
    		float4 screenPos = float4(i.screenPos.xyz , i.screenPos.w + 0.00000000001);
			float2 uv = screenPos.xy / screenPos.w;
    	//	cloud = tex2Dlod(_Cloud, float4(uv,0,0));
			cloud.rgb = 0; 
    		cloud.a = 0;
    	}

		half3 sky = saturate(frag_sky(i));//渲染天空
		half3 final;// = lerp(atmosphere, cloud.rgb, 1 - cloud.a);
		final = cloud.rgb + atmosphere * (1 - cloud.a);
        return float4(final,1);
    }

    ENDHLSL

    SubShader
    {
        Tags { "Queue"="Background" "RenderType"="Background" "PreviewType"="Skybox" }
        Cull Off ZWrite Off
        Pass
        {
            HLSLPROGRAM
        	#pragma enable_d3d11_debug_symbols

            #pragma vertex vert
            #pragma fragment frag
            #pragma target 3.0
            ENDHLSL
        }
    }
}
```
