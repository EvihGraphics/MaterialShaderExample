# Egaku source: `Shaders/Test/TestSH.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
Shader "Custom/TestSH"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _BaseColor("Base Color", Color) = (1,1,1,1)
        [HideInInspector] _SrcBlend("src", Float) = 1.0
        [HideInInspector] _DstBlend("dst", Float) = 0.0

    }
    SubShader
    {
        Tags {  }
 

        Pass
        {
            Blend DstAlpha One,Zero Zero //[_SrcBlend] [_DstBlend]
            ZWrite Off
            
            HLSLPROGRAM
            #pragma vertex vert
            #pragma fragment frag
		    #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"
            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"
            
            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
                float3 normal : NORMAL;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
                float3 normalWS : TEXCOORD1;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;
            float4 _BaseColor;
            

            float3 SH_IndirectionDiff(float3 normalWS)
            {
                float4 SHCoefficients[7];
                SHCoefficients[0]=unity_SHAr;
                SHCoefficients[1]=unity_SHAg;
                SHCoefficients[2]=unity_SHAb;
                SHCoefficients[3]=unity_SHBr;
                SHCoefficients[4]=unity_SHBg;
                SHCoefficients[5]=unity_SHBb;
                SHCoefficients[6]=unity_SHC;
                float3 Color = SampleSH9(SHCoefficients,normalWS);
                return max(0,Color);
            }
            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = TransformObjectToHClip(v.vertex);
                o.normalWS = TransformObjectToWorldNormal(v.normal);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                return o;
            }

            float4 frag (v2f i) : SV_Target
            {
                float3 env = SH_IndirectionDiff(i.normalWS);
                return float4(_BaseColor.rgb,1);
            }
            ENDHLSL
        }
    }
}
```
