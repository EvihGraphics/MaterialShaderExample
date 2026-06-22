# Egaku source: `Scripts/Modules/CBT/Renderer/RenderResources/CBTUpdateIndirectBuffer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Runtime.InteropServices;
using Modules.CBT.Renderer.Common;
using Common;
using Unity.Collections;
using UnityEngine;

namespace Modules.CBT.Renderer.RenderResources {
    public struct CBTUpdateIndirectBuffer {
        private GraphicsBuffer _buffer;
        
        public void Setup() { 
            _buffer = new GraphicsBuffer(GraphicsBuffer.Target.IndirectArguments | GraphicsBuffer.Target.Structured,1,
                Marshal.SizeOf<DispatchIndirectArgs>());
            var args = new NativeArray<DispatchIndirectArgs>(1,Allocator.Temp);
            args[0] = new DispatchIndirectArgs{
                threadGroupsX = 1,
                threadGroupsY = 1,
                threadGroupsZ = 1
            };
            _buffer.SetData(args);
            _buffer.name = "CBT Update Indirect Buffer";
            
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
