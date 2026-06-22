# Egaku source: `Shaders/Mesh/CBTTest.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿Shader "Custom/CBTTest"
{
    Properties
    {
		_Color ("Color", Color) = (1,1,1,1)
    }
    SubShader
    {
  
        Tags 
		{
			 "RenderPipeline"="UniversalPipeline"
		}

        Cull  Off
		ZWrite On

        HLSLINCLUDE
			#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"
			#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/CommonMaterial.hlsl"
			#include "CBT/CBTHelperStructs.hlsl"
			
			StructuredBuffer<uint> _CBTBuffer;
			StructuredBuffer<CBTPackedEvalResult> _CBTEvalResultBuffer;
			float4x4 _ModelMatrix;
			float4 _Color;
			int _InstanceIDOffset;

            /*
            UNITY_INSTANCING_BUFFER_START(Props)
			//	UNITY_DEFINE_INSTANCED_PROP(float4x4 ,_ModelMatrix)
                UNITY_DEFINE_INSTANCED_PROP(float ,_InstanceIDOffset)
            UNITY_INSTANCING_BUFFER_END(Props)
            */
			

		ENDHLSL
        
        Pass
        {
            HLSLPROGRAM
        	#pragma enable_d3d11_debug_symbols
			#pragma multi_compile_instancing
        	#pragma instancing_options procedural:Setup

        	
            #pragma vertex vert
            #pragma fragment frag
        	
            #define CBT_BUFFER_NAME _CBTBuffer
			#include "CBT/CBTMesh.hlsl"
            #include "Packages/com.unity.render-pipelines.core/ShaderLibrary/UnityInstancing.hlsl"
        	
            
            struct Attributes
            {
                float2 positionOS : POSITION;
            	uint instanceID : SV_InstanceID;
            };


            struct v2f
            {
                float2 uv : TEXCOORD0;
            	float3 positionWS : TEXCOORD1;
            	float3 positionOS :  TEXCOORD2;
                float4 positionCS : SV_POSITION;
            };

        	void Setup()
        	{
        		unity_ObjectToWorld = _ModelMatrix;
        	}

        	
            v2f vert(Attributes input)
            {
                v2f output;
            	uint nodeID = input.instanceID ;//+ _InstanceIDOffset;//GetInstanceID(input);
        		nodeID = UnpackEvalResult(_CBTEvalResultBuffer[nodeID]).nodeID;
            	float4 position = CBTMesh_GetPosition(nodeID,input.positionOS);
            //	position = float4(input.positionOS.x,0,input.positionOS.y,1);
				output.positionWS = mul(_ModelMatrix,position);//TransformObjectToWorld(position);
            	output.positionOS = position;
            	output.positionCS = TransformWorldToHClip(output.positionWS);
            	output.uv = input.positionOS.xy;
                return output;
            }

            
            float4 frag(v2f input) : SV_Target
            {
		//		UNITY_SETUP_INSTANCE_ID(input);
				float3 color = exp(-abs(input.positionWS)*0.1);
				return float4(input.uv,0,1);
			}
            
            ENDHLSL
        }
        
        
      
        
    }
}
```
