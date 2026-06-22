# Egaku source: `Common/CodeScope.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Diagnostics;
using System.Runtime.CompilerServices;
using Unity.Plastic.Antlr3.Runtime.Misc;

namespace Egaku.Rendering.Pipeline.Common {
    
    
    public static class CodeScope {

        [Conditional("DEBUG")]
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Debug(Action action) {
            action();
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Editor(Action action) {
#if UNITY_EDITOR
            action();
#endif
        }
        
    }
    
    
}
```
