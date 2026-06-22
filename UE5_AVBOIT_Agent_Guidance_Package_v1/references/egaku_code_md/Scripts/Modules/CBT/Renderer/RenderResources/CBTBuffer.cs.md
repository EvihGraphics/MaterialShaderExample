# Egaku source: `Scripts/Modules/CBT/Renderer/RenderResources/CBTBuffer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Runtime.InteropServices;
using Modules.CBT.Renderer.Common;
using Unity.Collections;
using UnityEngine;
using UnityEngine.Assertions;

namespace Modules.CBT.Renderer.RenderResources {
    public struct CBTBuffer {
        private GraphicsBuffer _buffer;

        private GraphicsBuffer _ConstructCBTBuffer(int cbtMaxDepth) {
                        int cbtBufferSize = CBTUtils.GetCBTBufferSizeInBytes(cbtMaxDepth);
            int stride = sizeof(uint);
            Assert.IsTrue(cbtBufferSize % stride == 0);
            GraphicsBuffer cbtBuffer = new GraphicsBuffer(GraphicsBuffer.Target.Structured, cbtBufferSize / stride, stride);
            var cbtBufferUint64 = new NativeArray<ulong>(cbtBufferSize / sizeof(ulong), Allocator.Temp);
       
            cbtBufferUint64[0] = (ulong)CBTUtils.GetCBTLevelNodeCount(cbtMaxDepth);
            for (int i = 1; i < cbtBufferUint64.Length; i++) {
                cbtBufferUint64[i] = 0;
            }
            CBTNode root = new CBTNode { id = 1, depth = 0 };
            CBTNode initLeafNodeLeft = new CBTNode{ id = root.id<<1, depth = 1}; 
            CBTNode initLeafNodeRight = new CBTNode{ id = (root.id<<1) | 1, depth = 1};
   
            var cbtBufferView = cbtBufferUint64.AsSpan();
            CBTUtils.SetLeafNodeBitfield(cbtBufferView, initLeafNodeLeft, cbtMaxDepth, true);
            CBTUtils.SetLeafNodeBitfield(cbtBufferView, initLeafNodeRight, cbtMaxDepth, true);
            
            // sum-reduction for initial CBT two leaf nodes at depth 2.
            CBTNode node1 = CBTUtils.GetCeilNode(initLeafNodeLeft, cbtMaxDepth);
            CBTNode node2 = CBTUtils.GetCeilNode(initLeafNodeRight, cbtMaxDepth);
            for (int i = cbtMaxDepth; i >= 1; i--) {
                CBTUtils.SetNodeBits(cbtBufferView, node1, cbtMaxDepth, 1);
                CBTUtils.SetNodeBits(cbtBufferView, node2, cbtMaxDepth, 1);
                node1.id >>= 1;
                node1.depth -= 1;
                node2.id >>= 1;
                node2.depth -= 1;
                var bytes2 = MemoryMarshal.AsBytes(cbtBufferUint64.AsReadOnlySpan());
            }
            CBTUtils.SetNodeBits(cbtBufferView, new CBTNode{ id = 1, depth = 0}, cbtMaxDepth, 2);
            cbtBuffer.SetData(cbtBufferUint64);
            var bytes = MemoryMarshal.AsBytes(cbtBufferUint64.AsReadOnlySpan());
            return cbtBuffer;
        }

        public void Setup(int cbtMaxDepth) {
            _buffer = _ConstructCBTBuffer(cbtMaxDepth);
            _buffer.name = "CBT Buffer";
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
