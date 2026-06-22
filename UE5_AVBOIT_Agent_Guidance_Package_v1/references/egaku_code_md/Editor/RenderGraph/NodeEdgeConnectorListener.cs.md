# Egaku source: `Editor/RenderGraph/NodeEdgeConnectorListener.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Linq;
using UnityEditor.Experimental.GraphView;
using UnityEngine;

namespace Egaku.Rendering.Pipeline.Editor {
    public class NodeEdgeConnectorListener : IEdgeConnectorListener
    {
        public void OnDropOutsidePort(Edge edge, Vector2 position)
        {
            Debug.Log("OnDropOutsidePort called.");
            // 可以实现“断开连接”或“创建新节点”
        }
        
        public void OnDrop(GraphView graphView, Edge edge)
        {
            // ✅ 关键：必须手动将 edge 添加到 GraphView
            Debug.Log("OnDrop called.");
            if (!graphView.Contains(edge)) {
                graphView.AddElement(edge);
            }

            // 可选：触发自定义逻辑
         //   (graphView as RenderGraphView)?.ConnectPorts(edge);
        }

      
    }
}
```
