# Egaku source: `ShaderLibrary/Modules/Blit/Blit.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿Shader "Egaku Render Pipeline/Blit"
{
    Properties
    {
        _MainTex ("Main Texture", 2D) = "white" {}
    }
    SubShader
    {
        HLSLINCLUDE
        #include "Blit.hlsl"
     
        ENDHLSL

        Pass{
            Tags { "LightMode" = "Basic"}
              

            HLSLPROGRAM
            #pragma enable_d3d11_debug_symbols
            #pragma vertex Vertex
            #pragma fragment Fragment
            #pragma target 3.0
            ENDHLSL
        }
        
    

    }

}
```
