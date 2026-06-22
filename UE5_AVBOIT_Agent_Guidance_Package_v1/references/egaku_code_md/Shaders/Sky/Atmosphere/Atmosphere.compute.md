# Egaku source: `Shaders/Sky/Atmosphere/Atmosphere.compute`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#include "Atmosphere.TransmittanceLutPass.hlsl"



#pragma kernel RenderAtmosphereTransmittanceLUTKernel
[numthreads(8, 4, 1)]
void RenderAtmosphereTransmittanceLUTKernel(uint2 dispatchThreadId : SV_DispatchThreadID)
{
    //RenderAtmosphereTransmittanceLUT()
}
```
