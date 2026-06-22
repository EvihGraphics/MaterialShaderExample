# Egaku source: `Editor/RenderGraph/Views/RenderPassNodeView.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using Egaku.Rendering.Pipeline.Common;
using UnityEditor;
using UnityEditor.Experimental.GraphView;
using UnityEngine;
using UnityEngine.UIElements;

namespace Egaku.Rendering.Pipeline.Editor {

    public class RenderPassNodeView : NodeView {
        
        public Toggle activeToggle;
        private bool _active;
        private readonly RenderPassNodeDesc _nodeDesc;
        
        private static readonly EdgeConnector _EdgeConnector = new EdgeConnector<Edge>(new NodeEdgeConnectorListener());
        

        public RenderPassNodeView(RenderPassNodeDesc nodeDesc,Vector2 position) {
            _nodeDesc = nodeDesc;
            title = ObjectNames.NicifyVariableName(nodeDesc.Name);
            SetPosition(new Rect(position, Vector2.zero));
            _CreatePorts();
            _CreateActiveToggle();
            RefreshExpandedState();
        }

        private void _CreatePorts() {
            foreach (var input in _nodeDesc.InputDesc) {
                CreatePort(input);
            }
            foreach (var output in _nodeDesc.OutputDesc) {
                CreatePort(output);
            }
            RefreshExpandedState();
        }
        



        private void _CreateActiveToggle() {
            var toggleContainer = new VisualElement();
            toggleContainer.style.flexDirection = FlexDirection.Row;
            toggleContainer.style.alignItems = Align.Center;
            toggleContainer.style.height = 20;
            toggleContainer.style.marginLeft = 10;
            toggleContainer.style.marginRight = 10;
            toggleContainer.style.marginTop = 4;

            activeToggle = new Toggle();
            activeToggle.label = "Active"; // 设置标签文本
            activeToggle.value = true;

// 手动控制 label 的样式：防止换行、设置宽度等
            activeToggle.labelElement.style.marginLeft = 4;           // 文字离复选框近一点
            activeToggle.labelElement.style.unityTextAlign = TextAnchor.MiddleLeft;
            activeToggle.labelElement.style.whiteSpace = WhiteSpace.NoWrap; // 禁止换行
            activeToggle.labelElement.style.minWidth = 60;            // 保证文字不被挤压

// 注册事件
            activeToggle.RegisterValueChangedCallback(evt => {
                _active = evt.newValue;
            });

// 将 Toggle 的 input（复选框）和 labelElement 添加到容器
            toggleContainer.Add(activeToggle);        // 复选框
            toggleContainer.Add(activeToggle.labelElement); // 文字标签

// 添加到标题容器下方
            
            Insert(1, toggleContainer);
        }

        private VisualElement _CreatePortContainer(string tooltip,Justify  align) {
            var portContainer = new VisualElement();
            portContainer.style.flexDirection = FlexDirection.Row; 
            portContainer.style.justifyContent = align;
        //    portContainer.style.alignItems = align;
            portContainer.style.height = 24;
            portContainer.tooltip = tooltip;
            // 添加小圆点（端口可视化）
            /*var cap = new VisualElement();
            cap.style.width = cap.style.height = 6;
            // cap.style.borderRadius = 6;
            cap.style.marginRight = 4;
            cap.style.backgroundColor = backgroundColor;*/
           // portContainer.Add(cap);
            return portContainer;
        }

        private Port CreatePort(RenderPassNodeInputDesc inputDesc) {
            Type inputType = inputDesc.ContentType;
            string portName = $"{ObjectNames.NicifyVariableName(inputDesc.Name)}";
            string typeName = inputType.Name;
            var port = InstantiatePort(Orientation.Horizontal, Direction.Input, Port.Capacity.Single, inputType);
          //  port.tooltip = $"Type: {typeName}";
            port.portName = portName;
            port.userData = inputDesc;
            port.portColor = new Color32(50, 93, 255, 255);
            //port.highlight = true;
            port.AddManipulator(_EdgeConnector);
            
            //  port.style.backgroundColor = GetDataTypeColor(dataType);

            var portContainer = _CreatePortContainer($"Type: {typeName}", Justify.FlexStart);
            portContainer.Add(port);
           // inputContainer.Add(port);
            inputContainer.Add(portContainer);
            portContainer.RegisterCallback((MouseDownEvent e) => {
                if (e.clickCount == 2) {
                    // 双击编辑？可扩展
                }
            });
            // 添加文字：name (type)
            //          var label = new Label($"{portName} ({type.Name})");
            //        label.style.unityTextAlign = TextAnchor.MiddleLeft;

            // 将小圆点和标签加入端口容器

            //   portContainer.Add(label);
            return port;
        }

        private Port CreatePort(RenderPassNodeOutputDesc outputDesc) {
            Type outputType = outputDesc.ContentType;
            string portName = $"{ObjectNames.NicifyVariableName(outputDesc.Name)}";
            string typeName = outputType.Name;
            var port = InstantiatePort(Orientation.Horizontal, Direction.Output, Port.Capacity.Multi, outputType);
            port.portName = portName;
        //    port.tooltip = $"Type: {typeName}";
            port.userData = outputDesc;
            port.highlight = true;
            port.portColor = new Color32(0, 255, 255, 255);
            port.AddManipulator(_EdgeConnector);
            var portContainer = _CreatePortContainer($"Type: {typeName}",Justify.FlexEnd);
            portContainer.Add(port);
            outputContainer.Add(portContainer);
            return port;
        }
    }

}
```
