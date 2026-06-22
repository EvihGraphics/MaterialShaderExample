# Egaku source: `Common/RenderContext.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using Unity.Collections;
using UnityEngine;
using UnityEngine.Assertions;
using UnityEngine.Rendering;
using UnityEngine.Rendering.RendererUtils;

namespace Egaku.Rendering.Pipeline.Common{
    public class RenderContext {
        private ScriptableRenderContext _internalContext;
        private RenderCamera[] _cameras;
        private readonly int _currentCameraIndex;
 
        public RenderCamera Camera => _cameras[_currentCameraIndex];
        public ref RenderCamera[] Cameras => ref _cameras;
        public ref ScriptableRenderContext InternalRenderContext => ref _internalContext;

        public RenderContext() {
   
        }
        public RenderContext(in ScriptableRenderContext internalContext, IEnumerable<Camera> cameras,int currentCameraIndex) {
            _internalContext = internalContext;
            _cameras = cameras.Select(n => new RenderCamera(n)).ToArray();
            _currentCameraIndex = currentCameraIndex;
        }

        
        

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void SubmitCommands() {
            _internalContext.Submit();
        }


        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public CullingResults Cull(ref ScriptableCullingParameters p) {
            return _internalContext.Cull(ref p);
        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public RendererList CreateRendererList(in RendererListDesc rendererListDesc){
            return _internalContext.CreateRendererList(rendererListDesc);
        }
        
        
        
    }
}
```
