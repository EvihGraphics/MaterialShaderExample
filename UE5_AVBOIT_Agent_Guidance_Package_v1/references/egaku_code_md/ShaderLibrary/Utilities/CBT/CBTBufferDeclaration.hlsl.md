# Egaku source: `ShaderLibrary/Utilities/CBT/CBTBufferDeclaration.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CBT_BUFFER_DECLARATION_HLSL
#define CBT_BUFFER_DECLARATION_HLSL


#ifndef CBT_FLAG_WRITE

#ifndef CBT_BUFFER_NAME
StructuredBuffer<uint> __cbtBuffer;
#define CBT_BUFFER_NAME (__cbtBuffer)
#endif
#define CBT_BUFFER_GETTER ((StructuredBuffer<uint>)(CBT_BUFFER_NAME))

#else

#ifndef CBT_BUFFER_NAME
RWStructuredBuffer<uint> __cbtBuffer;
#define CBT_BUFFER_NAME  (__cbtBuffer)
#endif

#define CBT_BUFFER_GETTER ((RWStructuredBuffer<uint>)(CBT_BUFFER_NAME))

#endif

#define GetCBTBuffer() CBT_BUFFER_GETTER

#endif
```
