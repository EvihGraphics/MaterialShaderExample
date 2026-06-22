# Egaku source: `Scripts/Modules/OIT/Fourier/OpacityMap/Controller/FOMInstanceController.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Common.Rendering;
using Modules.OIT.Fourier.OpacityMap.Renderer;
using UnityEngine;
using UnityEngine.Rendering;

namespace Modules.OIT.Fourier.OpacityMap.Controller {
    
    [ExecuteAlways]
    public class FOMInstanceController : MonoBehaviour {
        
        [SerializeField]
        private UnityEngine.Material _material;
        private FOMRenderInstance _renderInstance = new();
        private FOMRenderer _renderer;
        private bool _loaded = false;
        private Mesh _mesh;
        
        public void SetMaterial(UnityEngine.Material material) {
            _material = material;
        }
        

        bool SetupInstance() {
            _mesh = GetComponent<MeshFilter>().sharedMesh;
            _renderInstance.SetMesh(_mesh);
            _renderInstance.SetModelTransform(GetComponent<Transform>());
            _renderInstance.SetCustomMaterial(_material);
            _renderInstance.InitInnerMaterial();
 
            if (_material!=null && _renderer!=null) {
                _renderInstance.SetBaseColor(_material.GetColor("_Translucency_baseColor"));
            }
            _renderInstance.Validate();
            _renderInstance.SetEnabled();
            if (_renderer!=null && _renderInstance.Valid && _renderInstance.Enabled) {
                _renderer.TryAddRenderInstance(_renderInstance);
                return true;
            }

            return false;
        }

        
        public void OnEnable() {
            _renderer = CustomRendererManager.TryGetCustomRenderer<FOMRenderer>();
            _renderInstance.SetEnabled();
        }
        
    
        private void Update() {
            if (_loaded == false) { 
                _loaded = SetupInstance();
            }
        }

        public void OnDisable() {
            _renderInstance.SetDisabled();
        }
        
    }
}
```
