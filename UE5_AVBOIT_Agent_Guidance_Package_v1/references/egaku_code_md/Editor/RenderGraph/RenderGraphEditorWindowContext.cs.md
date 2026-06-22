# Egaku source: `Editor/RenderGraph/RenderGraphEditorWindowContext.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common;
using Egaku.Rendering.Pipeline.Utility;
using PlasticGui.WorkspaceWindow;

namespace Egaku.Rendering.Pipeline.Editor {
    public class RenderGraphEditorWindowContext {
        private RenderFeatureDesc[] _renderFeaturesDesc;
        
        public RenderFeatureDesc[] RenderFeaturesDesc { get => _renderFeaturesDesc; }

        public RenderGraphEditorWindowContext() {
            Refresh();
        }

        public void Refresh() {
            _renderFeaturesDesc = PipelineMetaUtils.GetAllRenderFeatureDesc().ToArray();
        }
    }
}
```
