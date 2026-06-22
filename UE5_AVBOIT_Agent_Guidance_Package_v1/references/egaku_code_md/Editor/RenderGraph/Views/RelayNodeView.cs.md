# Egaku source: `Editor/RenderGraph/Views/RelayNodeView.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using UnityEditor.Experimental.GraphView;
using UnityEngine;
using UnityEngine.UIElements;
using Node = UnityEditor.Experimental.GraphView.Node;

namespace Egaku.Rendering.Pipeline.Editor {
    
    
    public class RelayNodeView : NodeView {

        private Type _relayType;
        
        private static readonly EdgeConnector _EdgeConnector = new EdgeConnector<Edge>(new NodeEdgeConnectorListener());


        public RelayNodeView(Vector2  position)  {
            this.Q("title").RemoveFromHierarchy();
            this.Q("divider").RemoveFromHierarchy();
            SetPosition(new Rect(position,Vector2.zero));
            CreateInputPort();
            CreateOutputPort();
            RefreshExpandedState();
        }
        
        private Port CreateInputPort() {
            var port = InstantiatePort(Orientation.Horizontal, Direction.Input, Port.Capacity.Single, typeof(object));
            port.portName = "";
            port.portColor = new Color32(50, 93, 255, 255);
            port.AddManipulator(_EdgeConnector);
            inputContainer.Add(port);
            return port;
        }
        
        private Port CreateOutputPort() {
            var port = InstantiatePort(Orientation.Horizontal, Direction.Output, Port.Capacity.Multi, typeof(object));
            port.portName = "";
            port.portColor = new Color32(0, 255, 255, 255);
            port.AddManipulator(_EdgeConnector);
            outputContainer.Add(port);
            return port;
        }
        
        public void RefreshRelayType(Type type) {
            _relayType = type;
            foreach (var port in GetPorts()) {
                port.portType = type;
                port.tooltip = $"{type.Name}";
            }
        }
        
    }
}
```
