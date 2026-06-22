# Egaku source: `Shaders/TestSamples/TestTexArray.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
Shader "Unlit/TestTexArray"
{
    Properties
    {
        _MainTex2 ("Texture", 2DArray) = "white" {}
        _Slice ("Slice", Float) = 0
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            HLSLPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"
            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/UnityInput.hlsl"
            #include "Packages/com.unity.render-pipelines.core/ShaderLibrary/GlobalSamplers.hlsl"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            Texture2DArray _MainTex2;
            SamplerState sampler_MainTex2;
            float _Slice;
            
            float4 _MainTex2_ST;

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = TransformObjectToHClip(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex2);
                return o;
            }

            float4 frag (v2f i) : SV_Target
            {
                float slice = fmod(_Time.y,64.0f);
                float4 col = _MainTex2.Sample(sampler_MainTex2,float3(i.uv,slice));
                return float4(col.rgb,1);
            }
            ENDHLSL
        }
    }
}
```
