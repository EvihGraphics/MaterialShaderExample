# Egaku source: `Editor/CodeGenerator/GenerateMaterialParamTableMenu.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common;
using Egaku.Rendering.Pipeline.Utility;
using UnityEditor;
using UnityEngine;

namespace Egaku.Rendering.Pipeline.Editor.CodeGenerator {
    
    
    public class GenerateMaterialParamTableMenu {
        [MenuItem("Tools/Generate All Material Basic Codes")]
        static void GenerateAllMaterialBasicCodes()
        {
            ShaderCodeGenerator.GenerateAllBasicCodes();
        }
        
        [MenuItem("Tools/Generate All Compute Shader Asset Files")]
        static void GenerateAllComputeShaderAssetFiles()
        {
            ShaderCodeGenerator.GenerateAllComputeShaderAssetFiles();
        }
    }
}
```
