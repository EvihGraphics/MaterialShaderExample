# Egaku source: `Scripts/Modules/Utility/TilePixelSelector.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using System;
using UnityEngine;

namespace Modules.Utility {
    
    [ExecuteAlways]
    public class TilePixelSelector : MonoBehaviour {
        
        [Serializable]
        struct TilePixelSelectorConfig {
            public int tileSizeX;
            public int tileSizeY;
            public float fps;
        }
        
        private int _pixelTileSelected;
        
        [SerializeField]
        private TilePixelSelectorConfig _tilePixelSelectorConfig;
        

        private System.Random _rng = new();
        
        private void Update() {
            Shader.SetGlobalVector("_TilePixelSelector_TileSize", new Vector4(_tilePixelSelectorConfig.tileSizeX, _tilePixelSelectorConfig.tileSizeY, 0,0));
            Shader.SetGlobalFloat("_TilePixelSelector_FPS", _tilePixelSelectorConfig.fps);
            int randomInt = _rng.Next(0,_tilePixelSelectorConfig.tileSizeX * _tilePixelSelectorConfig.tileSizeY); 
            _pixelTileSelected = randomInt;
            Shader.SetGlobalInteger("_TilePixelSelector_SelectedIndex", _pixelTileSelected);
        }
    }
}
```
