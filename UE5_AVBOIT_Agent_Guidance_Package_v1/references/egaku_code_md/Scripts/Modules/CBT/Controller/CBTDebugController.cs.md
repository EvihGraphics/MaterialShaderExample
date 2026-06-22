# Egaku source: `Scripts/Modules/CBT/Controller/CBTDebugController.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Modules.CBT.Renderer;
using Common.Rendering;
using UnityEngine;

namespace Modules.CBT.Controller {
    [ExecuteAlways]
    public class CBTDebugController : MonoBehaviour{

        private Material _material;
        
        private CBTMeshRenderInstance _renderInstance = new();
        private CBTMeshRenderer _renderer;
        private Transform _transform;
        private bool _loaded = false;

        private void SetupRenderInstance() {
            _renderInstance.SetMaterial(_material);
            _renderInstance.SetTransform(_transform);
            _renderInstance.Validate();
        }

        private void OnEnable() {
            _transform = GetComponent<Transform>();
            _renderer = CustomRendererManager.TryGetCustomRenderer<CBTMeshRenderer>();
            _renderInstance.SetEnabled();
        }

        private void OnDisable() {
            _renderInstance.SetDisabled();
        }


        private void UpdateTransform() {
            _material.SetMatrix("_ModelMatrix", _transform.localToWorldMatrix);
        }
        
        private void Update() {
            if (_loaded == false && enabled && _renderer != null) {
                SetupRenderInstance();
                _renderInstance.SetEnabled();
                _renderer.SetRenderInstance(_renderInstance);
            }
            UpdateTransform();
        }

    }
}
```
