# Egaku source: `Scripts/Modules/OIT/Fourier/OpacityMap/Renderer/FOMRenderer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using Common.Rendering;
using Modules.OIT.Fourier.OpacityMap.Renderer.RenderResources;
using Modules.OIT.Fourier.OpacityMap.Renderer.Shaders;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.OIT.Fourier.OpacityMap.Renderer {
    
    
    public class FOMRenderer : CustomRenderer<FOMRenderInstance>{
        private readonly HashSet<FOMRenderInstance> _renderInstances = new ();
        private static FOMRenderSettings _settings;
        private static FourierCoeffsLut _fourierCoeffsLut;
        private static TranslucencyBuffer _translucencyBuffer;
        private bool _loaded = false;
        private bool _enabled = false;

        public override bool Enabled => _enabled;
        public override bool Valid => _loaded;
        
        public override bool TryAddRenderInstance(FOMRenderInstance instance) {
            return _renderInstances.Add(instance);
        }

        public override bool TryRemoveRenderInstance(FOMRenderInstance instance) {
            return _renderInstances.Remove(instance);
        }
        
        private void LoadResources() {
            if (_loaded == false) {
                try {
                    _fourierCoeffsLut.Setup(_settings.width,_settings.height,4);
                    FOMShader.GlobalInit();
                    FOMShader.SetFourierCoeffsLut(_fourierCoeffsLut.GetSliceHandles());
                    _loaded = true;
                }catch (Exception e) {
                    Debug.LogError(e);
                    _enabled = false;
                    _loaded = false;
                }
            }
        }

        public void Reset() {
            _loaded = false;
        }
        
        public override bool Initialize() {
            LoadResources();
            return _loaded;
        }
        
        public void SetRenderSettings(FOMRenderSettings settings) {
            _settings = settings;
        } 
        
        public override bool Destroy() {
            return true;
        }
        
        public void SetEnabled() {
            _enabled = true;
        }
        
        public void SetDisabled() {
            _enabled = false;
        }
        
        public void SetupTranslucencyBuffer(int width, int height) {
            _translucencyBuffer.Setup(width, height);
        }

        private RTHandle _tempRT;
        public void RecordRenderCommandsFourierCoefficientsBuildingPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("FOM - Fourier Coefficients Building Pass");
            var depthTarget = new RenderTargetIdentifier(_fourierCoeffsLut.GetSliceHandles()[0].rt.depthBuffer);
            CoreUtils.SetRenderTarget(cmd,_fourierCoeffsLut.GetSliceTargets(), depthTarget, ClearFlag.Color,Color.clear);
            foreach (var rendererInstance in _renderInstances) {
                if (rendererInstance.Enabled && rendererInstance.Valid) {
                    rendererInstance.RecordRenderCommandsFourierCoefficientsBuildingPass(cmd);
                }
            }
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }

        public void RecordRenderCommandsTranslucencyForwardPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("FOM - Translucency Forward Pass");
            CoreUtils.SetRenderTarget(cmd,_translucencyBuffer.GetHandle(),ClearFlag.Color, Color.clear);
       
            foreach (var rendererInstance in _renderInstances) {
                if (rendererInstance.Enabled && rendererInstance.Valid) {
                    rendererInstance.RecordRenderCommandsTranslucencyForwardPass(cmd);
                }
            }
            
            FOMShader.CmdResolveBackground(cmd,_translucencyBuffer.GetHandle());
            RTHandle rt = data.cameraData.renderer.cameraColorTargetHandle;
            cmd.Blit(_translucencyBuffer.GetHandle(),rt);
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }

        
        

        
    }
}
```
