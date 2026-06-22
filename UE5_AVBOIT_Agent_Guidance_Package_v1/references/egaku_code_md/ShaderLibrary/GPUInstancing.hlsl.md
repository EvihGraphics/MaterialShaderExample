# Egaku source: `ShaderLibrary/GPUInstancing.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef GPU_INSTANCING_HLSL
#define GPU_INSTANCING_HLSL

#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/UnityInstancing.hlsl"

#define DECLARE_VERTEX_INPUT_INSTANCE_ID UNITY_VERTEX_INPUT_INSTANCE_ID
#define SETUP_INSTANCE_ID(var) UNITY_SETUP_INSTANCE_ID(var)
#define TRANSFER_INSTANCE_ID(input,output) UNITY_TRANSFER_INSTANCE_ID(input,output)

#define INSTANCING_BUFFER_BEGIN(name) UNITY_INSTANCING_BUFFER_START(name)
#define INSTANCING_BUFFER_END(name) UNITY_INSTANCING_BUFFER_END(name)

#define DEFINE_INSTANCED_PROP(type, name) UNITY_DEFINE_INSTANCED_PROP(type, name)
#define ACCESS_INSTANCED_PROP(arr,var) UNITY_ACCESS_INSTANCED_PROP(arr,var)


#endif
```
