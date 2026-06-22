# Egaku source: `Scripts/Modules/OIT/AVBOIT/Renderer/VirtualBlockBased/RenderResources/ParallelScanAuxiliaryArray.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.OIT.AVBOIT.Renderer.VirtualBlockBased.RenderResources {
    
    public struct ParallelScanAuxiliaryArray  {
        private GraphicsBuffer _buffer;
        
        public void Setup(int length) { 
            _buffer = new GraphicsBuffer(GraphicsBuffer.Target.Structured,length,sizeof(float));
            _buffer.name = "AVBOIT - Parallel Scan Auxiliary Array";
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
