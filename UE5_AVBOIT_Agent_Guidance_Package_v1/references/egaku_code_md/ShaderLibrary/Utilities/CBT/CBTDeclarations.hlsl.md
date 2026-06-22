# Egaku source: `ShaderLibrary/Utilities/CBT/CBTDeclarations.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CBTDECLARATIONS_HLSL
#define CBTDECLARATIONS_HLSL

#ifndef CBT_HEAP_BUFFER_BINDING
#ifdef CBT_FLAG_WRITE
#define CBT_HEAP_BUFFER_BINDING register(u0)
#else
#define CBT_HEAP_BUFFER_BINDING register(t0)
#endif
#endif

#ifdef CBT_FLAG_WRITE
RWStructuredBuffer<uint> u_CbtBuffer : CBT_HEAP_BUFFER_BINDING;
#else
StructuredBuffer<uint> u_CbtBuffer : CBT_HEAP_BUFFER_BINDING;
#endif

#include "CBTTypedef.hlsl"

#endif
```
