# Egaku source: `Common/RenderPipeline.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {
    public abstract class RenderPipeline : UnityEngine.Rendering.RenderPipeline {

        private static void GetAllDerivedRenderPipeline() {
            
        }
        
        public virtual void Create() {
            
        }

        public virtual void OnPipelineSetup() {
            
        }
        
        public virtual void OnCameraSetup(ScriptableRenderContext ctx) {
            
        }


        public virtual void RecordRenderCommands(ScriptableRenderContext ctx) {
            
        }
        
        public virtual void OnCameraCleanup(ScriptableRenderContext ctx) {
            
        }
        
        public virtual void OnPipelineExit(ScriptableRenderContext ctx) {
            
        }
        

        public virtual void Destroy() {
            
        }
        
    }
}
```
