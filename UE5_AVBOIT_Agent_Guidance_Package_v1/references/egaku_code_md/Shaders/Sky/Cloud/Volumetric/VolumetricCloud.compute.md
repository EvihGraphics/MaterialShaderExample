# Egaku source: `Shaders/Sky/Cloud/Volumetric/VolumetricCloud.compute`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#define UNIVERSAL_RENDER_PIPELINE

#include "Passes/VoxelMetaLutBuildingPass.CSImpl.hlsl"

#pragma kernel Kernel_VoxelMetaLutBuilding
```
