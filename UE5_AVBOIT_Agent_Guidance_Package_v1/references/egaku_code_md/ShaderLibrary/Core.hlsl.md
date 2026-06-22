# Egaku source: `ShaderLibrary/Core.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CORE_HLSL
#define CORE_HLSL

#ifdef UNIVERSAL_RENDER_PIPELINE
#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"
#else
#include "Input.hlsl"
#endif

#include "Common.hlsl"
#include "Texture.hlsl"
#include "Context.hlsl"
#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/SpaceTransforms.hlsl"

#endif
```
