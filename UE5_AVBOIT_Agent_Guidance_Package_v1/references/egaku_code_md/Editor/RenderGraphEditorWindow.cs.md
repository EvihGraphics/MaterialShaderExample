# Egaku source: `Editor/RenderGraphEditorWindow.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common;
using Egaku.Rendering.Pipeline.Utility;
using UnityEngine;
using UnityEditor;
using UnityEditor.UIElements;


namespace Egaku.Rendering.Pipeline.Editor {
    // RenderGraphEditorWindow.cs

    public class RenderGraphEditorWindow : EditorWindow
    {
   
        private RenderGraphEditorWindowContext _windowContext;
        
        [MenuItem("Window/Egaku Render Pipeline/Render Graph Editor")]
        public static void ShowWindow()
        {
            var window = GetWindow<RenderGraphEditorWindow>();
            window.titleContent = new GUIContent("Egaku Render Pipeline - Render Graph Editor");
            window.Show();
        }

        

        private void OnEnable()
        {
            // 清空并添加 GraphView
            rootVisualElement.Clear();
            if (_windowContext is null) {
                _windowContext = new RenderGraphEditorWindowContext();
            } else {
                _windowContext.Refresh();
            }
         
            var graphView = new RenderGraphView(this, _windowContext);
            graphView.style.flexGrow = 1; // 占满窗口
            rootVisualElement.Add(graphView);
        }
    }
}
```
