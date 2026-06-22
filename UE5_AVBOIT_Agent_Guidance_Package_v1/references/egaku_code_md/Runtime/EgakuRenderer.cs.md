# Egaku source: `Runtime/EgakuRenderer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using Egaku.Rendering.Pipeline.Common;
using UnityEngine;
using UnityEngine.Assertions;
using Renderer = Egaku.Rendering.Pipeline.Common.Renderer;


namespace Egaku.Rendering.Pipeline.Runtime {
    public class EgakuRenderer : Renderer,IDisposable {

        private bool _initialized = true;
        
        private List<RenderFeature> _renderFeatures = new();

        private List<RenderPassNode> _renderPassNodes = new();

        private EgakuRenderResourceManager _renderResourceManager;
        
        private int _frameCount;
        
        public EgakuRenderer(EgakuRenderResourceManager renderResourceManager) {
            _renderResourceManager = renderResourceManager;
            AddRenderFeature(new BasicRenderFeature());
          //  AddRenderFeature(new SolidColorRenderFeature());
        }
        
        public void AddRenderFeature(RenderFeature feature) {
            _renderFeatures.Add(feature);
        }
        
        public void Dispose() {
            Debug.LogError("EgakuRenderer Disposed");
        }

        private void _InitResourceTransmissionPipes() {
            _renderResourceManager.AutoRegisterResourceTransmissions(_renderPassNodes);
            _renderResourceManager.CreateResourcePipes();
            _renderResourceManager.AutoSetResourcePipePorts(_renderPassNodes);
        }
        
        
        private void _InitComponents() {
            foreach (var n in _renderPassNodes) {
                n._InternalCreate(_renderResourceManager);
            }
        }
        
        private void _AddRenderPassNodes() { 
            foreach (var rf in _renderFeatures) {
                foreach (var passNode in rf.GetRenderPassNodes()) {
                    _renderPassNodes.Add(passNode);
                }
            }
        }
        
        private void _ClearAll() {
            _renderPassNodes.Clear();
            _renderResourceManager.ClearRenderContext();
            _renderResourceManager.ClearResourceTransmissionRegistry();
        }
        
        public override void Reset() {
            _ClearAll();
            _initialized = false;
        }

        public override void Init() {
            if (_initialized is false) {
                _ClearAll();
                _AddRenderPassNodes();
                _InitResourceTransmissionPipes();
                _initialized = true;
            }
        }


        public override void CreatePasses() {
            _InitComponents();
        }

        public override void SetupPasses() {
    
        }

        public override void Render() {
            CodeScope.Debug(() => { Assert.IsTrue(_initialized); });
            foreach (var pass in _renderPassNodes) {
                if (pass.Active) {
                    pass.Setup();
                    pass.Execute();
                    pass.Clean();
                } else {
                    pass.Bypass();
                }
            }
            _frameCount++;
        }

        public void SetupPerFrame() {

        }


        public void SetupPerCamera() {

        }

    }
}
```
