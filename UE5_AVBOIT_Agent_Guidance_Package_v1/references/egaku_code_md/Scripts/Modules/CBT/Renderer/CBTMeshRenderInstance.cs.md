# Egaku source: `Scripts/Modules/CBT/Renderer/CBTMeshRenderInstance.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Common.Rendering;
using UnityEngine;
using UnityEngine.Rendering;

namespace Modules.CBT.Renderer {
    public class CBTMeshRenderInstance : CustomRenderInstance {
        private Material _material;
        private Transform _transform;

        private bool _valid = false;
        private bool _enabled = false;
        
        public override bool Enabled => _enabled;
        public override bool Valid => _valid;
        
        public void Validate() {
            if (_material != null && _transform != null) {
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
        
        public void SetMaterial(Material material) {
            _material = material;
        }

        public void SetTransform(Transform t) {
            _transform = t;
        }

        public Transform GetTransform() {
            return _transform;
        }

        public Material GetMaterial() {
            return _material;
         }
        
    }
}
```
