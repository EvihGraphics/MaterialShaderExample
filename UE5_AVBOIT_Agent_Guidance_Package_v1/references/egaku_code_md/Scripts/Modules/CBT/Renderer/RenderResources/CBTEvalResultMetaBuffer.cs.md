# Egaku source: `Scripts/Modules/CBT/Renderer/RenderResources/CBTEvalResultMetaBuffer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Runtime.InteropServices;
using Modules.CBT.Renderer.Shaders;
using Unity.Collections;
using UnityEngine;

namespace Modules.CBT.Renderer.RenderResources {
    public struct CBTEvalResultMetaBuffer {
        private GraphicsBuffer _buffer;

        public void Setup() {
            _buffer = new GraphicsBuffer(GraphicsBuffer.Target.Structured,1,
                Marshal.SizeOf<CBTEvalResultMeta>());
            var buffer = new NativeArray<CBTEvalResultMeta>(1, Allocator.Temp);
            buffer[0] = new CBTEvalResultMeta{
                resultCount = 0
            };
            _buffer.SetData(buffer);
            _buffer.name = "CBT Eval Result Meta Buffer";
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
