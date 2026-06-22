# Egaku source: `Scripts/Modules/CBT/Renderer/RenderResources/CBTMeshDrawIndirectBuffer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Unity.Collections;
using UnityEngine;

namespace Modules.CBT.Renderer.RenderResources {
    public struct CBTMeshDrawIndirectBuffer {
        private GraphicsBuffer _buffer;
        private const int MaxInstancePerDraw = 32768;
        
        public void Setup(uint instanceCount,uint meshletIndexCount) {
            /*_drawCallCount = (int)(instanceCount/MaxInstancePerDraw);
          _drawCallCount += instanceCount % MaxInstancePerDraw == 0 ? 0 : 1;*/
            int _drawCallCount = 1;
            _buffer =  new GraphicsBuffer(GraphicsBuffer.Target.IndirectArguments | GraphicsBuffer.Target.Structured,_drawCallCount,
                GraphicsBuffer.IndirectDrawIndexedArgs.size);
            uint instanceCountRemain = instanceCount;
            var buffer = new NativeArray<GraphicsBuffer.IndirectDrawIndexedArgs>(_drawCallCount, Allocator.Temp);
            for (int i = 0; i < _drawCallCount; i++) {
                buffer[i] = new GraphicsBuffer.IndirectDrawIndexedArgs{
                    indexCountPerInstance = meshletIndexCount,
                    instanceCount = Math.Min(instanceCountRemain, MaxInstancePerDraw),
                    baseVertexIndex = 0,
                    startIndex = 0,
                    startInstance = 0
                };
                instanceCountRemain -= buffer[i].instanceCount;
            }
            _buffer.SetData(buffer);
            _buffer.name = "CBT Mesh Draw Indirect Buffer";
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
