# Egaku source: `Editor/EgakuRenderPipelineEditor.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Runtime;
using UnityEditor;
using UnityEngine;

namespace Egaku.Rendering.Pipeline.Editor {

    
    [CustomEditor(typeof(EgakuRenderPipelineAsset)),CanEditMultipleObjects]
    public class EgakuRenderPipelineAssetEditor : UnityEditor.Editor
    {
        public override void OnInspectorGUI()
        {
            DrawDefaultInspector();
            
            EditorGUILayout.Space();
            if (GUILayout.Button("Refresh Pipeline"))
            {
                HandleRefreshButtonPress();
                Debug.Log("Pipeline Refreshed");
            }
        }

        private void HandleRefreshButtonPress() {
            EgakuRenderPipelineAsset.ResetPipeline();
        }
    }


}
```
