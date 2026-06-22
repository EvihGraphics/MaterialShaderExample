# Egaku source: `Runtime/EgakuRenderPipelineAsset.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Runtime {
    
    [CreateAssetMenu(menuName = "Rendering/Egaku Render Pipeline")]
    public class EgakuRenderPipelineAsset : RenderPipelineAsset {
        
        
        private static EgakuRenderPipeline _currentPipeline;


        public static void ResetPipeline() {
            _currentPipeline.Reset();
        }

        protected override RenderPipeline CreatePipeline() {
            _currentPipeline = new();
            return _currentPipeline;
        }
        
    }
}
```
