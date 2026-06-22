# Egaku source: `Editor/RenderGraph/NodeSerchWindowProvider.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Collections.Generic;
using Egaku.Rendering.Pipeline.Common;
using UnityEditor.Experimental.GraphView;
using UnityEngine;

namespace Egaku.Rendering.Pipeline.Editor {
    
    public class NodeSearchMenuWindowProvider : ScriptableObject, ISearchWindowProvider
    {
        public delegate bool OnSelectEntryDelegate(SearchTreeEntry searchTreeEntry,SearchWindowContext context);
        
        public OnSelectEntryDelegate OnSelectEntryHandler;   
        
        private List<SearchTreeEntry> _entries;
        
        public NodeSearchMenuWindowProvider() { }
        
        public void Reset() {
            var entries = new List<SearchTreeEntry>();
            entries.Add(new SearchTreeGroupEntry(new GUIContent("Render Features")));    
            _entries = entries;
        }

        public void AddFeature(RenderFeatureDesc featureDesc) {
            var groupEntry = new SearchTreeGroupEntry(new GUIContent(featureDesc.Name)) {
                level = 1,
                userData = featureDesc
            };
            _entries.Add(groupEntry);
        }
        
        public void AddPassNode(RenderPassNodeDesc nodeDesc) {
            var entry = new SearchTreeEntry(new GUIContent(nodeDesc.Name)) {
                level = 2,
                userData = nodeDesc
            };
            _entries.Add(entry);
        }
        
        public List<SearchTreeEntry> CreateSearchTree(SearchWindowContext context) {
            return _entries;
        }

        public bool OnSelectEntry(SearchTreeEntry searchTreeEntry, SearchWindowContext context) {
            if (OnSelectEntryHandler is null) {
                return false;
            }
            return OnSelectEntryHandler(searchTreeEntry, context);
        }
    }
}
```
