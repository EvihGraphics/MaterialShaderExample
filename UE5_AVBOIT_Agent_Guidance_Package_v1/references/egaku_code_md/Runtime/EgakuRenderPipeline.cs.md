# Egaku source: `Runtime/EgakuRenderPipeline.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Collections.Generic;
using System.Linq;
using Egaku.Rendering.Pipeline.Common;
using UnityEngine;
using UnityEngine.Experimental.Rendering.RenderGraphModule;
using UnityEngine.Rendering;
using RenderPipeline = Egaku.Rendering.Pipeline.Common.RenderPipeline;
using Renderer = Egaku.Rendering.Pipeline.Common.Renderer;

namespace Egaku.Rendering.Pipeline.Runtime {
    public class EgakuRenderPipeline : RenderPipeline {
        
        public Renderer renderer;
        
        private EgakuRenderResourceManager _egakuRenderResourceManager;
        
        private int _renderContextCount;
        
        private bool _firstFrame = true;
        

        public EgakuRenderPipeline() {
            _egakuRenderResourceManager = new EgakuRenderResourceManager();
            renderer = new EgakuRenderer(_egakuRenderResourceManager);
        }

        public void Reset() {
            _firstFrame = true;
            renderer.Reset();
        }
        
        public void SetRenderContexts(ScriptableRenderContext context,IEnumerable<Camera> cameras) {
            int i = 0;
            foreach (var camera in cameras) {
                RenderContext ctx = new RenderContext(context, cameras,i++);
                _egakuRenderResourceManager.RegisterRenderContext(ctx);
            }
            _renderContextCount = i;
        }

        public void PerFrameCleanUp() {
            _egakuRenderResourceManager.ClearRenderContext();
        }


        protected override void Render(ScriptableRenderContext context, Camera[] cameras) {
            Render(context,cameras.ToList());
        }
        
        protected override void Render(ScriptableRenderContext context, List<Camera> cameras) {
    
            if (_firstFrame is true) {
                renderer.Init();
                renderer.CreatePasses();
                _firstFrame = false;
            }
            
            SetRenderContexts(context, cameras);
            BeginContextRendering(context, cameras);
            for(int i = 0; i < cameras.Count;i++) {
                _egakuRenderResourceManager.SetCurrentRenderContext(i);
                BeginCameraRendering(context,cameras[i]);
                renderer.Render();
                EndCameraRendering(context,cameras[i]);
            }
   
            EndContextRendering(context, cameras);
            PerFrameCleanUp();
        }
        
    }
}
```
