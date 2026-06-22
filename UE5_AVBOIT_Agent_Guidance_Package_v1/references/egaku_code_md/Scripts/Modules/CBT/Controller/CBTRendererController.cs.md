# Egaku source: `Scripts/Modules/CBT/Controller/CBTRendererController.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Modules.CBT.Renderer;
using Common.Rendering;
using UnityEngine;

namespace Modules.CBT.Controller {
    
    [ExecuteAlways]
    public class CBTRendererController : MonoBehaviour{
        [SerializeField]
        private CBTMeshRenderSettings _settings;
        
        private CBTMeshRenderer _renderer;
        private bool _loaded = false;

        private void Load() {
            _renderer = CustomRendererManager.GetOrCreateCustomRenderer<CBTMeshRenderer>();
            _renderer.SetRenderSettings(_settings);
            _loaded = _renderer.Initialize();
         
        }
        
        private void OnEnable() {
            Load();
            _renderer.SetEnabled();
        }

        private void Update() {
            if (_loaded == false && enabled) {
                Load();
                _renderer.SetEnabled();
            }

            if (_renderer.Enabled && _renderer.Valid) {
                _renderer.UpdatePingPong();
            }
        }

        private void OnDisable() {
            _renderer.SetDisabled();
        }
        
        private void OnDestroy() {
            _renderer.Destroy();
        }
    }
}
```
