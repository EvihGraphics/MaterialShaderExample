# Egaku source: `Scripts/RenderFeatures/Atmosphere/AtmosphereRenderFeature.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using RenderFeatures.Atmosphere.RenderPasses;
using Modules.Sky.Atmosphere.Renderer;
using UnityEngine;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.Atmosphere {
    
    [DisallowMultipleRendererFeature("Atmosphere")]
    public class AtmosphereRenderFeature : ScriptableRendererFeature{
        static AtmosphereRenderer _renderer;
        
        private AtmospherePrtPass _prtPass;
        private AtmosphereTransmittanceLutPass _transmittanceLutPass;
        
        private bool _valid = false;
        
        public static void SetRenderer(AtmosphereRenderer renderer) {
            _renderer = renderer;
        }
        
        public override void AddRenderPasses(ScriptableRenderer renderer, ref RenderingData renderingData) {
            if (_renderer!=null) {
                _prtPass.SetRenderer(_renderer);
                _transmittanceLutPass.SetRenderer(_renderer);
                _renderer.SetupRenderTargets();
                _renderer.SetupShaderParameters();
                
                // for unity skybox
                Shader.SetGlobalTexture("_AtmospherePrt", _renderer.GetAtmospherePrtRenderTarget());
                Shader.SetGlobalTexture("_AtmosphereTransmittanceLut", _renderer.GetAtmosphereTransmittanceLutRenderTarget());
                _valid = true;
            }
            if (_valid && _renderer.Enabled) {
                if (renderingData.cameraData.camera.cameraType == CameraType.SceneView ||
                    renderingData.cameraData.camera.cameraType == CameraType.Game) {
                    renderer.EnqueuePass(_transmittanceLutPass);
                    renderer.EnqueuePass(_prtPass);
                }
            }
        }

        public override void Create() {
            _valid = false;
            _transmittanceLutPass = new ();
            _transmittanceLutPass.renderPassEvent = RenderPassEvent.BeforeRendering;
            _prtPass = new ();
            _prtPass.renderPassEvent = RenderPassEvent.BeforeRenderingSkybox;
        }
    }
}
```
