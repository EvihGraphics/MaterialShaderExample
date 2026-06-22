# Egaku source: `Scripts/Modules/CBT/Renderer/RenderResources/CBTEvalContextCBuffer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Runtime.InteropServices;
using Modules.CBT.Renderer.Shaders;
using UnityEngine;

namespace Modules.CBT.Renderer.RenderResources {
    public struct CBTEvalContextCBuffer {
        private GraphicsBuffer _buffer;

        public void Setup() {
            _buffer = new GraphicsBuffer(GraphicsBuffer.Target.Constant, 
                GraphicsBuffer.UsageFlags.LockBufferForWrite,
                1, Marshal.SizeOf<CBTEvalContextConstants>());
            _buffer.name = "CBT Eval Context Constants";
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
