# Egaku source: `Scripts/Modules/OIT/Fourier/OpacityMap/Controller/FOMRendererController.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Common.Rendering;
using Modules.OIT.Fourier.OpacityMap.Renderer;
using UnityEngine;

namespace Modules.OIT.Fourier.OpacityMap.Controller {
    
    [ExecuteAlways]
    public class FOMRendererController : MonoBehaviour{
        
        [SerializeField]
        private FOMRenderSettings _settings;

        private FOMRenderer _renderer;

        private bool _loaded = false;


        void Load() {
            _renderer = CustomRendererManager.GetOrCreateCustomRenderer<FOMRenderer>();
            _renderer.SetRenderSettings(_settings);
            _loaded = _renderer.Initialize();
        }

        [ContextMenu("Reload")]
        public void GenerateTestCases() {
            _renderer = CustomRendererManager.GetOrCreateCustomRenderer<FOMRenderer>();
            _renderer.Reset();
            Load();
        }

        public void OnEnable() {
            Load();
            _renderer.SetEnabled();
        }

        public void Update() {
            if (_loaded == false && enabled) {
                Load();
                _renderer.SetEnabled();
            }
        }
    
        
        public void OnDisable() {
            _renderer.SetDisabled();
        }

        public void OnDestroy() {
            // _renderer.Destroy();
        }

    }
}
```
