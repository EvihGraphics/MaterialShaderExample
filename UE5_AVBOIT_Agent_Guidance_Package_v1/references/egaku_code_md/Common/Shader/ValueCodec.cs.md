# Egaku source: `Common/Shader/ValueCodec.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿namespace Egaku.Rendering.Pipeline.Common.Shader {
    
    
    public interface IValueCodec<TEncoded,TDecoded> {
        public TDecoded Decode(TEncoded encodedValue);
        public TEncoded Encode(TDecoded decodedValue);
    }
}
```
