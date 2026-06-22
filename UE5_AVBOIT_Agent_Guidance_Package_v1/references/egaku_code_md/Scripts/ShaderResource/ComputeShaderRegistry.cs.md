# Egaku source: `Scripts/ShaderResource/ComputeShaderRegistry.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;


namespace ShaderResource{
    
    [CreateAssetMenu(fileName = "ComputeShaderRegistry", menuName = "Compute Shader Registry")]
    public class ComputeShaderRegistry  : ScriptableObject {

        [Serializable]
        public struct Entry {
            public string name;
            public ComputeShader shader;
        }
        
        [Serializable]
        public struct Folder {
            public string path;
            public List<Entry> shaders;
        }

        public List<Folder> registry = new();

        private Dictionary<string, Entry> _map = new();
        
        private void OnEnable() {
            _map.Clear();
            foreach (var folder in registry) {
                foreach (var entry in folder.shaders) {
                    _map.Add($"{folder.path}/{entry.name}", entry);
                }
            }
        }

        public Entry GetEntry(string key) {
            return _map.GetValueOrDefault(key);
        }
     
    }
}
```
