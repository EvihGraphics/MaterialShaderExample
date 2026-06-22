# Egaku source: `Scripts/Modules/OIT/AVBOIT/Controller/AVBOITInstanceController.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Common.Rendering;
using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased;
using UnityEngine;
using UnityEngine.Rendering;

namespace Modules.OIT.AVBOIT.Controller {
    
    [ExecuteAlways]
    public class AVBOITInstanceController : MonoBehaviour{
        [SerializeField]
        private UnityEngine.Material _material;
        private AVBOITRenderInstance _renderInstance = new();
        private AVBOITRenderer _renderer;
        private bool _loaded = false;
        
        public void SetMaterial(UnityEngine.Material material) {
            _material = material;
        }
        

        bool SetupInstance() {
            _renderInstance.SetMesh(GetComponent<MeshFilter>().sharedMesh);
            _renderInstance.SetModelTransform(transform);
            _renderInstance.InitInnerMaterial();
            
            if (_material!=null && _renderer!=null) {
                if (_renderer.UseGPUBlending) {
                    _material.SetInt("_SrcBlend", (int)BlendMode.DstAlpha);
                    _material.SetInt("_DstBlend", (int)BlendMode.One);
                    _renderInstance.SetBaseColor(_material.GetColor("_BaseColor"));
                    _renderInstance.SetCustomMaterial(_material);
                } else {
                    _renderInstance.SetBaseColor(_material.GetColor("_Translucency_baseColor"));
                }
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
            _renderer = CustomRendererManager.TryGetCustomRenderer<AVBOITRenderer>();
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
