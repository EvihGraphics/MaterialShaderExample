# Egaku source: `Scripts/Modules/CBT/Renderer/CBTMeshRenderSettings.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;

namespace Modules.CBT.Renderer {
    
    [Serializable]
    public struct CBTMeshRenderSettings {
        public int cbtMaxDepth;
        public int meshletSubdivisionLevel;
        public int targetPrimitiveViewportPixelCount;
    }
}
```
