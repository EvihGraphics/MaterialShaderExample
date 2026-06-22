# Egaku source: `ShaderLibrary/Modules/SolidColor.shader`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿Shader "Egaku Render Pipeline/SolidColor"
{
    Properties
    {
        _SolidColor ("Color", Color) = (1,1,1,1)
    }
    SubShader
    {
        HLSLINCLUDE
        #include "SolidColor.hlsl"
     
        ENDHLSL

        Pass{
            Tags { "LightMode" = "Basic"}
              

            HLSLPROGRAM
            #pragma multi_compile_instancing
            #pragma vertex Vertex
            #pragma fragment Fragment
            #pragma target 3.0
            ENDHLSL
        }
        
    

    }

}
```
