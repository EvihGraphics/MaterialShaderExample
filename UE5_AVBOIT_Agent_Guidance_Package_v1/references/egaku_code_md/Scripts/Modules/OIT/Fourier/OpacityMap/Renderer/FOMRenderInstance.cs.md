# Egaku source: `Scripts/Modules/OIT/Fourier/OpacityMap/Renderer/FOMRenderInstance.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Common.Rendering;
using Modules.OIT.Fourier.OpacityMap.Renderer.Shaders;
using UnityEngine;
using UnityEngine.Rendering;

namespace Modules.OIT.Fourier.OpacityMap.Renderer {
    public class FOMRenderInstance : CustomRenderInstance{
        
        private UnityEngine.Material _material;
        private Mesh _mesh;
        private Transform _transform;
        private FOMShader _shader;
        
        public override bool Enabled => _enabled;
        
        public override bool Valid => _valid;

        private bool _valid = false;
        private bool _enabled = false;

        
        public void SetModelTransform(Transform t) {
            _transform = t;
        }

        public void SetMesh(Mesh mesh) {
            _mesh = mesh;
        }
        
        public void SetCustomMaterial(UnityEngine.Material material) {
            _material = material;
        }

        public void SetBaseColor(in Color color) {
            _shader.SetBaseColor(color);
        }
        
        public void InitInnerMaterial() {
            _shader.InitInnerMaterial();
        }

        public void Validate() {
            if (_material != null && _mesh != null && _transform != null) {
                _valid = true;
            } else {
                _valid = false;
            }
        }

        public void SetEnabled() {
            _enabled = true;
        }

        public void SetDisabled() {
            _enabled = false;
        }
        
        public void RecordRenderCommandsFourierCoefficientsBuildingPass(CommandBuffer cmd) {
            _shader.CmdDrawMeshFourierCoefficientsBuildingPass(cmd, _mesh, _transform.localToWorldMatrix);
        }
        
        public void RecordRenderCommandsTranslucencyForwardPass(CommandBuffer cmd) {
            _shader.CmdDrawMeshTranslucencyForwardPass(cmd, _mesh, _transform.localToWorldMatrix);
        }
    }
}
```
