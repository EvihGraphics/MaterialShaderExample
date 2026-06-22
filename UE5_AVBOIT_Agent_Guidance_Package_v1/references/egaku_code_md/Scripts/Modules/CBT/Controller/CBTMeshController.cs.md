# Egaku source: `Scripts/Modules/CBT/Controller/CBTMeshController.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using System;
using Modules.CBT.Renderer;
using Common.Rendering;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.CBT.Controller {
    
    [ExecuteAlways]
    public class CBTMeshController : MonoBehaviour {
        [SerializeField]
        private Material _material;

        [Serializable]
        struct MeshSettings {
            public Vector2 dimension;
            public float altitude;
            public bool autoExtension;
        }
        
        [SerializeField]
        private MeshSettings _settings;
        
        
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
            if (_settings.autoExtension) {
                Camera cam = Camera.allCameras[0];
                Vector3 pos = cam.transform.position;
                float width = Math.Min(_settings.dimension.x,_settings.dimension.y);
                float camHeight = Math.Max(0.5f,pos.y)/(width*0.025f);
                float multiplier = Math.Max(1,(camHeight*Mathf.Tan(Mathf.Deg2Rad*cam.fieldOfView)));
                
                float x = _settings.dimension.x * multiplier;
                float z = _settings.dimension.y * multiplier;
                Vector3 offset = new Vector3(-0.5f*x,_settings.altitude,-0.5f*z);
                pos.y = 0;
                transform.position = pos + offset;
                transform.localScale = new Vector3(x, 1, z);
            }
            _material.SetMatrix("_ModelMatrix", _transform.localToWorldMatrix);
        }
        
        private void Update() {
            if (_loaded == false && enabled && _renderer != null) {
                SetupRenderInstance();
                if (_renderInstance.Valid) {
                    BuildAmbientSH();
                    _renderInstance.SetEnabled();
                    _renderer.SetRenderInstance(_renderInstance);
                }
            }
            if (_renderInstance.Valid) {
                UpdateTransform();
            }
        }

        private void BuildAmbientSH() {
            Vector3 pos = _transform.position;
            SphericalHarmonicsL2 sh;
            LightProbes.GetInterpolatedProbe(pos, null, out sh);

            _material.SetVector("_EnvLight_SHAr",new Vector4(sh[0,3],sh[0,1],sh[0,2],sh[0,0]));
            _material.SetVector("_EnvLight_SHBr",new Vector4(sh[0,4],sh[0,5],sh[0,6],sh[0,7]));
            _material.SetVector("_EnvLight_SHAg",new Vector4(sh[1,3],sh[1,1],sh[1,2],sh[1,0]));
            _material.SetVector("_EnvLight_SHBg",new Vector4(sh[1,4],sh[1,5],sh[1,6],sh[1,7]));
            _material.SetVector("_EnvLight_SHAb",new Vector4(sh[2,3],sh[2,1],sh[2,2],sh[2,0]));
            _material.SetVector("_EnvLight_SHBb",new Vector4(sh[2,4],sh[2,5],sh[2,6],sh[2,7]));
            _material.SetVector("_EnvLight_SHC",new Vector4(sh[0,8],sh[1,8],sh[2,8],1));
        }

    }
}
```
