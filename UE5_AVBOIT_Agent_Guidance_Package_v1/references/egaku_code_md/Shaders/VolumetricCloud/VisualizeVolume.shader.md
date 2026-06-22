# Egaku source: `Shaders/VolumetricCloud/VisualizeVolume.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿Shader "Custom/VolumetricCloud/VisualizeVolume"
{
    Properties
    {
        [NoScaleOffset]
        _VolumeTex("Texture", 3D)  = "" {}
        _VolumeTex_ValueScale("Value Scale", Vector) = (1,1,1,1)
        _VolumeTex_ValueOffset("Value Offset", Vector) = (0,0,0,0)
        _VolumeTex_CoordsScale("Coords Scale", Vector) = (1,1,1)
        _ChannelWeights("Channel Weights", Vector) = (1,1,1,1)
    }
    
    SubShader
    {
        Tags {
            "RenderPipeline" = "UniversalRenderPipeline"
        }
        
        Pass
        {
            HLSLPROGRAM

            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"
            #include "Packages/com.unity.render-pipelines.core/ShaderLibrary/SpaceTransforms.hlsl"
            #include "HLSLInclude/ScreenSpaceUtils.hlsl"
            
            #pragma vertex vert
            #pragma fragment frag

            sampler3D _VolumeTex;
            float4 _VolumeTex_ValueScale;
            float4 _VolumeTex_ValueOffset;
            float3 _VolumeTex_CoordsScale;
            float4 _ChannelWeights;
            
           struct Attributes {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
           };

            struct Varyings {
                float4 position : SV_POSITION;
                float2 uv : TEXCOORD0;
                float3 positionWS : TEXCOORD1;
            };


            sampler2D _MainTex;
            float4 _MainTex_ST;

            
            Varyings vert(Attributes a2v) {
                Varyings v2f;
                v2f.positionWS = TransformObjectToWorld(a2v.vertex);
                v2f.position = TransformObjectToHClip(a2v.vertex);
                v2f.uv = a2v.uv;
                return v2f;
            }


            float remap(float x, float L0, float H0, float Ln, float Hn){
	            return Ln + (x - L0) * (Hn - Ln) / (H0 - L0);
            }
                        
            float4 frag(Varyings v2f) : SV_TARGET {
                float3 uvw = v2f.positionWS;
              //  return float4(uvw*_VolumeTex_CoordsScale,1);
                float4 value = tex3D(_VolumeTex, uvw*_VolumeTex_CoordsScale)*_VolumeTex_ValueScale + _VolumeTex_ValueOffset;
                float fbm = value.gba*_ChannelWeights.gba;
                float color = remap(value.r,-(1-fbm),1,0,1);
                return float4(color, color, color, 1);
            }
            ENDHLSL
        }
    }
}
```
