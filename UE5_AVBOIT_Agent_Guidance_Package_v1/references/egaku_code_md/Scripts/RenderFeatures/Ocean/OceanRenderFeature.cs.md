# Egaku source: `Scripts/RenderFeatures/Ocean/OceanRenderFeature.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
using Modules.CBT.Renderer;
using Common.Rendering;
using RenderFeatures.Ocean.RenderPasses;
using UnityEngine;
using UnityEngine.Rendering.Universal;


namespace RenderFeatures.VolumetricCloud {


	[DisallowMultipleRendererFeature("Ocean")]
	public class OceanRenderFeature : ScriptableRendererFeature {
		private CBTEvalPass _evalPass;
		private CBTUpdatePass _updatePass;
		private OceanSurfacePass _surfacePass;
		private CBTMeshRenderer _renderer;
		
		public override void AddRenderPasses(ScriptableRenderer renderer, ref RenderingData renderingData) {
			if (_renderer!=null && _renderer.Enabled && _renderer.Valid) {
				_updatePass.SetRenderer(_renderer);
				_surfacePass.SetRenderer(_renderer);
				_evalPass.SetRenderer(_renderer);
				if (renderingData.cameraData.camera.cameraType == CameraType.SceneView || renderingData.cameraData.camera.cameraType == CameraType.Game) {
					renderer.EnqueuePass(_evalPass);
					renderer.EnqueuePass(_updatePass);
					renderer.EnqueuePass(_surfacePass);
				}
			}
		}

		public override void Create() {
			_renderer = CustomRendererManager.GetOrCreateCustomRenderer<CBTMeshRenderer>();
			_updatePass = new CBTUpdatePass();
			_surfacePass = new OceanSurfacePass();
			_evalPass = new CBTEvalPass();
			_evalPass.renderPassEvent = RenderPassEvent.BeforeRendering;
			_surfacePass.renderPassEvent = RenderPassEvent.BeforeRenderingTransparents;
			_updatePass.renderPassEvent = RenderPassEvent.AfterRendering;
		}

	}

}
```
