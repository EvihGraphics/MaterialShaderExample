# Egaku source: `Scripts/Common/DispatchIndirectArgs.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Runtime.InteropServices;

namespace Common {
    [StructLayout(LayoutKind.Sequential)]
    struct DispatchIndirectArgs {
        public uint threadGroupsX;
        public uint threadGroupsY;
        public uint threadGroupsZ;
    }

}
```
