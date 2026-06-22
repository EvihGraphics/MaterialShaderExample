# Egaku source: `Scripts/Modules/CBT/Renderer/RenderResources/CBTEvalResultBuffer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;

namespace Modules.CBT.Renderer.RenderResources {
    public struct CBTEvalResultBuffer {
        private GraphicsBuffer _buffer;

        public void Setup(int cbtMaxNodeCount) {
            _buffer = new GraphicsBuffer( GraphicsBuffer.Target.Structured,cbtMaxNodeCount,
                sizeof(uint));
            _buffer.SetCounterValue(0);
            _buffer.name = "CBT Eval Result Buffer";
        }
        
        public GraphicsBuffer GetBuffer() {
            return _buffer;
        }
        
        public void Release() {
            if (_buffer != null) {
                _buffer.Release();
            }
        }
    }
}
```
