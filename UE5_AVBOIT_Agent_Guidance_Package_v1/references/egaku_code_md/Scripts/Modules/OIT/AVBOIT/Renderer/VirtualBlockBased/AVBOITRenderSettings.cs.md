# Egaku source: `Scripts/Modules/OIT/AVBOIT/Renderer/VirtualBlockBased/AVBOITRenderSettings.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;

namespace Modules.OIT.AVBOIT.Renderer.VirtualBlockBased {
    
    [Serializable]
    public struct AVBOITRenderSettings {
        [Serializable]
        public struct VirtualVolumeConfig {
            public int width;
            public int height;
            public int sliceCount;
        }
        
        [Serializable]
        public struct VolumeBlockConfig {
            public int widthDivisor;
            public int heightDivisor;
            public int sliceDivisor;
        }
        
        [Serializable]
        public struct PhysicalVolumeConfig {
            public int blockCount;
        }
        
        public VirtualVolumeConfig virtualVolumeConfig;
        public VolumeBlockConfig volumeBlockConfig;
        public PhysicalVolumeConfig physicalVolumeConfig;
        
        public bool useGPUBlending;

    }
}
```
