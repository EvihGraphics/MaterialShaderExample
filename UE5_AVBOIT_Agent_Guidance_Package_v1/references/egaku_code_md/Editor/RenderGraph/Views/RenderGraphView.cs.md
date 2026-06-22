# Egaku source: `Editor/RenderGraph/Views/RenderGraphView.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;
using Egaku.Rendering.Pipeline.Common;
using UnityEditor.Experimental.GraphView;
using UnityEngine;
using UnityEngine.UIElements;


namespace Egaku.Rendering.Pipeline.Editor {

    public class RenderGraphView : GraphView {

        private readonly RenderGraphEditorWindow _editorWindow;

        private readonly RenderGraphEditorWindowContext _editorContext;


        public RenderGraphView(RenderGraphEditorWindow editorWindow, RenderGraphEditorWindowContext editorContext) {
            _editorWindow = editorWindow;
            _editorContext = editorContext;
            // 设置缩放范围
            SetupZoom(ContentZoomer.DefaultMinScale, ContentZoomer.DefaultMaxScale);
            this.StretchToParentSize();
            // 添加拖拽、选择等操作器
            this.AddManipulator(new SelectionDragger());
            this.AddManipulator(new ContentDragger());
            this.AddManipulator(new RectangleSelector());
            this.AddManipulator(new ClickSelector());

            // this.AddManipulator(new DeleteSelectionDelegate());

            _InitNodeSearchMenuWindow();

            // 背景网格
            var grid = new GridBackground();
            Insert(0, grid);
            grid.StretchToParentSize();
            graphViewChanged += OnGraphViewChanged;
        }
        

   

        private void _InitNodeSearchMenuWindow() {
            var menuWindowProvider = ScriptableObject.CreateInstance<NodeSearchMenuWindowProvider>();
            menuWindowProvider.OnSelectEntryHandler = OnCreateNodeEntrySelected;
            var renderFeaturesDesc = _editorContext.RenderFeaturesDesc;
            foreach (var feature in renderFeaturesDesc) {
                menuWindowProvider.AddFeature(feature);
                foreach (var passNode in feature.PassNodesDesc) {
                    menuWindowProvider.AddPassNode(passNode);
                }
            }
            
            nodeCreationRequest += context => {
                SearchWindow.Open(new SearchWindowContext(context.screenMousePosition), menuWindowProvider);
            };
        }

        
        private Vector2 _GetGraphMousePosition(in SearchWindowContext context) {
            var windowRoot = _editorWindow.rootVisualElement;
            var windowMousePosition = windowRoot.ChangeCoordinatesTo(windowRoot.parent, context.screenMousePosition - _editorWindow.position.position);
            var graphMousePosition = contentViewContainer.WorldToLocal(windowMousePosition);
            return graphMousePosition;
        }
        
        private bool OnCreateNodeEntrySelected(SearchTreeEntry searchTreeEntry, SearchWindowContext context) {
            var passNodeDesc = searchTreeEntry.userData as RenderPassNodeDesc;
            RenderPassNodeView node = new RenderPassNodeView(passNodeDesc,_GetGraphMousePosition(context));
            AddElement(node);
            return true;
        }

        public override void BuildContextualMenu(ContextualMenuPopulateEvent evt) {
            base.BuildContextualMenu(evt);
            BuildContextualMenu(evt,"Put Relay",OnPutRelayMenuEntryClicked,1);
        }

        public void OnPutRelayMenuEntryClicked(DropdownMenuAction e) {
            RelayNodeView node = new RelayNodeView(e.eventInfo.localMousePosition);
            AddElement(node);
        }
        
        protected void BuildContextualMenu(ContextualMenuPopulateEvent evt, string actionName,Action<DropdownMenuAction> action, int menuPosition = -1)
        {
            if (menuPosition == -1) {
                menuPosition = evt.menu.MenuItems().Count;
            }
            Vector2 position = (evt.currentTarget as VisualElement).ChangeCoordinatesTo(contentViewContainer, evt.localMousePosition);
            evt.menu.InsertAction(menuPosition, actionName, action, DropdownMenuAction.AlwaysEnabled);
        }


        private GraphViewChange OnGraphViewChanged(GraphViewChange graphViewChange) {
            if (graphViewChange.edgesToCreate != null) {
                foreach (var edge in graphViewChange.edgesToCreate) {
                    ConnectPorts(edge);
                }
            }
            
            if (graphViewChange.elementsToRemove != null) {
                foreach (var element in graphViewChange.elementsToRemove) {
                    switch (element) {
                        case Edge edge:
                            DisconnectPorts(edge);
                            break;
                        case NodeView node:
                            foreach (var port in node.GetPorts()) {
                                List<Edge> portEdges = new List<Edge>(port.connections);
                                foreach (var edge in portEdges) {
                                    DisconnectPorts(edge);
                                    RemoveElement(edge);
                                }
                            }
                            break;
                    }
                }
            }
            

            return graphViewChange;
        }

        public void ConnectPorts(Edge edge) {
            var outputPort = edge.output;
            var inputPort = edge.input;
            var fromNode = outputPort.node;
            var toNode = inputPort.node;
            inputPort.Connect(edge);
            outputPort.Connect(edge);
            if (fromNode != null && toNode != null) {
                var fromFieldName = outputPort.portName;
                var toFieldName = inputPort.portName;
                Debug.Log($"Connected: {fromNode.title}.{fromFieldName} → {toNode.title}.{toFieldName}");
            }
        }
        
        private void DisconnectPorts(Edge edge) {
            var outputPort = edge.output;
            var inputPort = edge.input;
            var fromNode = outputPort?.node;
            var toNode = inputPort?.node;
            outputPort?.Disconnect(edge);
            inputPort?.Disconnect(edge);
            if (fromNode != null && toNode != null) {
                var fromFieldName = outputPort.portName;
                var toFieldName = inputPort.portName;
                Debug.Log($"Disconnected: {fromNode.title}.{fromFieldName} → {toNode.title}.{toFieldName}");
            }
        }
        

        public override List<Port> GetCompatiblePorts(Port startPort, NodeAdapter nodeAdapter) {
            var compatiblePorts = new List<Port>();
            foreach (var port in ports.ToList()) {
                if (startPort.node != port.node &&
                    startPort.direction != port.direction &&
                    startPort.portType == port.portType) {
                    compatiblePorts.Add(port);
                }
            }

            return compatiblePorts;
        }
    }
}
```
