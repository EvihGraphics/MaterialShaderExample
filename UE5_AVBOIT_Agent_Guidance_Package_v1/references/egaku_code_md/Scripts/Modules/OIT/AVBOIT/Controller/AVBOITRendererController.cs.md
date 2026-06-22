# Egaku source: `Scripts/Modules/OIT/AVBOIT/Controller/AVBOITRendererController.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using Common.Rendering;
using Modules.OIT.AVBOIT.Renderer.VirtualBlockBased;
using UnityEngine;

namespace Modules.OIT.AVBOIT.Controller {
    
    [ExecuteAlways]
    public class AVBOITRendererController : MonoBehaviour {

        [SerializeField]
        private AVBOITRenderSettings _settings;

        private AVBOITRenderer _renderer;

        private bool _loaded = false;


        void Load() {
            _renderer = CustomRendererManager.GetOrCreateCustomRenderer<AVBOITRenderer>();
            _renderer.SetRenderSettings(_settings);
            _loaded = _renderer.Initialize();
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
