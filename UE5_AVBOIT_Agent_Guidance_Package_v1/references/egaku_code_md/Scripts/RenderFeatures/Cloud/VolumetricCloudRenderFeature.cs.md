# Egaku source: `Scripts/RenderFeatures/Cloud/VolumetricCloudRenderFeature.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿

using RenderFeatures.Cloud.RenderPasses;
using Modules.Sky.Cloud.Volumetric.Renderer;
using UnityEngine;
using UnityEngine.Rendering.Universal;

namespace RenderFeatures.Cloud {
    
    [DisallowMultipleRendererFeature("Volumetric Cloud")]
    public class VolumetricCloudRenderFeature : ScriptableRendererFeature{
        static VolumetricCloudRenderer _renderer;
        private FourierShadowMapBuildingPass _fourierShadowMapBuildingPass;
        private VoxelMetaLutBuildingPass _voxelMetaLutBuildingPass;
        private VolumetricCloudPostProcessPass _postProcessPass;
        private VolumetricCloudOctahedronMarchingPass _octahedronMarchingPass;
        private DCTShadowMapBuildingPass _dctShadowMapBuildingPass;
        
        private bool _valid = false;
        
        public static void SetRenderer(VolumetricCloudRenderer renderer) {
            _renderer = renderer;
        }
        
        public override void AddRenderPasses(ScriptableRenderer renderer, ref RenderingData renderingData) {
            if (_renderer!=null) {
                _fourierShadowMapBuildingPass.SetRenderer(_renderer);
                _postProcessPass.SetRenderer(_renderer);
                _octahedronMarchingPass.SetRenderer(_renderer);
                _voxelMetaLutBuildingPass.SetRenderer(_renderer);
                _dctShadowMapBuildingPass.SetRenderer(_renderer);
                _valid = true;
            }
            if (_valid && _renderer.Enabled) {
                renderer.EnqueuePass(_voxelMetaLutBuildingPass);
                if (renderingData.cameraData.camera.cameraType == CameraType.SceneView ||
                    renderingData.cameraData.camera.cameraType == CameraType.Game) {
                    renderer.EnqueuePass(_dctShadowMapBuildingPass);
                    renderer.EnqueuePass(_fourierShadowMapBuildingPass);
                    renderer.EnqueuePass(_octahedronMarchingPass);
                }
            }
        }

        public override void Create() {
            _valid = false;
            _fourierShadowMapBuildingPass = new();
            _voxelMetaLutBuildingPass = new ();
            _postProcessPass = new ();
            _octahedronMarchingPass = new ();
            _dctShadowMapBuildingPass = new ();
            _dctShadowMapBuildingPass.renderPassEvent = RenderPassEvent.BeforeRenderingPrePasses;
            _fourierShadowMapBuildingPass.renderPassEvent = RenderPassEvent.BeforeRenderingPrePasses;
            _postProcessPass.renderPassEvent = RenderPassEvent.BeforeRenderingPostProcessing;
            _voxelMetaLutBuildingPass.renderPassEvent = RenderPassEvent.BeforeRenderingPrePasses;
            _octahedronMarchingPass.renderPassEvent = RenderPassEvent.BeforeRenderingPrePasses + 2;
            
        }
    }
}
```
