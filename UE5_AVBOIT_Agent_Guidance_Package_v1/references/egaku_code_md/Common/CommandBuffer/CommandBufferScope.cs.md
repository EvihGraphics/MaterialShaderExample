# Egaku source: `Common/CommandBuffer/CommandBufferScope.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {
    
    public struct CommandScope : IDisposable{
        private RenderContext _context;
        private CommandBuffer _cmdBuf;
        private ProfilingSampler _profSampler;
        private bool _disposed;

        public CommandScope(RenderContext context, CommandBuffer cmdBuf, ProfilingSampler profSampler = null) {
            _context = context;
            _cmdBuf = cmdBuf;
            _profSampler = profSampler;
            _disposed = false;
            RenderCommand.BeginCommandScope(context, cmdBuf, profSampler);
        }

        public void Dispose() {
            Dispose(true);
        }
        
        // Protected implementation of Dispose pattern.
        void Dispose(bool disposing) {
            if (_disposed) {
                return;
            }

            if (disposing) {
                RenderCommand.EndCommandScope(_context, _cmdBuf, _profSampler);
            }

            _disposed = true;
        }
    }
}
```
