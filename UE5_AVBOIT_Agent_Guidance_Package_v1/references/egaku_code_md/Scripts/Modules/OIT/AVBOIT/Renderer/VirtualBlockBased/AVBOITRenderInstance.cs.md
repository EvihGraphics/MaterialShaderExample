# Egaku source: `Scripts/Modules/OIT/AVBOIT/Renderer/VirtualBlockBased/AVBOITRenderInstance.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Common.Rendering;
using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased.Shaders;
using UnityEngine;
using UnityEngine.Rendering;

namespace Modules.OIT.AVBOIT.Renderer.VirtualBlockBased {
    public class AVBOITRenderInstance : CustomRenderInstance{
        
        private UnityEngine.Material _material;
        private Mesh _mesh;
        private Transform _transform;
        private AVBOITShader _shader;

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

        public void RecordRenderCommandsTranslucencyForwardPassWithGPUBlending(CommandBuffer cmd) {
            _shader.CmdDrawMeshTranslucencyForwardPrePass(cmd, _mesh, _transform.localToWorldMatrix);
            _shader.CmdDrawMeshTranslucencyForwardPass(cmd,_mesh, _transform.localToWorldMatrix,_material);
        }
        
        public void RecordRenderCommandsTranslucencyForwardPass(CommandBuffer cmd) {
            _shader.CmdDrawMeshTranslucencyForwardPass(cmd, _mesh, _transform.localToWorldMatrix);
        }
        
        public void RecordRenderCommandsVolumeOccupancyCollectionPass(CommandBuffer cmd) {
            _shader.CmdDrawMeshVolumeOccupancyCollectionPass(cmd, _mesh, _transform.localToWorldMatrix);
        }
        
        public void RecordRenderCommandsSplattingPass(CommandBuffer cmd) {
            _shader.CmdDrawMeshSplattingPass(cmd, _mesh, _transform.localToWorldMatrix);
        }

    }
}
```
