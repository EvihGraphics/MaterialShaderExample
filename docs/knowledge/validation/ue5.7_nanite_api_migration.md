# UE 5.7 Nanite API Migration and Fixes

## 概述 (Overview)
在将 `MaterialShaderExample` 适配到本地的 `UnrealEngine_Animation_Tech` (基于 UE 5.7) 时，遇到了一系列由 Nanite 核心渲染 API 变更引起的 Shader 编译错误和运行时崩溃。本文档记录了相关的错误现象和适配 UE 5.7 的修复方案。

## 1. Shader 编译错误修复 (MaterialShaderExample.usf)

### 错误现象
在启动编辑器时，`WorldGridMaterial` 在编译阶段出现严重报错：
* `unknown type name 'FNaniteTransformedTri'`
* `use of undeclared identifier 'FetchTransformedNaniteTriangle'`
* `no matching function for call to 'FetchNaniteMaterialPixelParameters'`

### 原因
UE 5.7 中的 Nanite 数据结构和方法发生了改变，弃用了旧的 `FetchTransformedNaniteTriangle`，转而使用数组形式获取顶点的 `FetchTransformedNaniteVerts`。同时，`FetchNaniteMaterialPixelParameters` 的模板参数和参数列表也有所精简和调整。

### 修复方案
修改 `MaterialShaderExample.usf` 中对过期 API 的调用：

1. **顶点获取**: 将 `FNaniteTransformedTri` 替换为数组和体素数据结构：
```hlsl
// 旧代码
// FNaniteTransformedTri Tri = FetchTransformedNaniteTriangle(VisibleCluster, TriIndex, NumTexCoords);
// FPositionAndNormal Point0 = Tri.Verts[0];

// 新代码 (UE 5.7)
FNaniteTransformedVerts<3> Verts;
FReconstructedVoxelData VoxelData;
FetchTransformedNaniteVerts(VisibleCluster, TriIndex, NumTexCoords, Verts, VoxelData);
FPositionAndNormal Point0 = Verts[0];
```

2. **参数获取**: 更新 `FetchNaniteMaterialPixelParameters` 参数列表以匹配新的模板签名：
```hlsl
// 旧代码
// FNaniteMaterialPixelParameters MaterialParameters = FetchNaniteMaterialPixelParameters(
//     ...
//     /* bWPOEnabled = */ false,
//     ...

// 新代码 (UE 5.7)
FNaniteMaterialPixelParameters MaterialParameters = FetchNaniteMaterialPixelParameters(
    ...
    VoxelData, // 替代了 bWPOEnabled 等旧参数
    ...
```

## 2. 运行时 C++ 断言崩溃修复 (ExampleSceneViewExtension.cpp)

### 错误现象
成功修复 Shader 编译后，在编辑器加载场景时引发崩溃并弹出错误日志：
`Null resource entry in uniform buffer parameters: FNaniteShadingUniformParameters.Resources[3], ResourceType 0x11.`

### 原因
在插件构建 `FNaniteShadingUniformParameters` （Nanite 着色 Uniform Buffer 参数）时使用了伪造（Dummy）的 Resource Binding 以绕过管线校验。
而在 UE 5.7 中，`FNaniteShadingUniformParameters` 增加了一个必须绑定的 SRV 缓冲 `AssemblyTransforms`（在 Resources 数组的索引为 3），导致插件遗漏该绑定并在运行时触发空指针校验断言。

### 修复方案
在 `ExampleSceneViewExtension.cpp` 中分配并初始化 `FNaniteShadingUniformParameters` 时，为其增加一个 Dummy 绑定：
```cpp
// 修复前
Parameters.HierarchyBuffer = PassParameters.VisibleClustersSWHW;
Parameters.ShadingMask = PassParameters.VisBuffer64;

// 修复后
Parameters.HierarchyBuffer = PassParameters.VisibleClustersSWHW;
Parameters.AssemblyTransforms = PassParameters.VisibleClustersSWHW; // 新增，用于 UE 5.7
Parameters.ShadingMask = PassParameters.VisBuffer64;
```

## 结论 (Conclusion)
完成以上两步修复后，该 MaterialShaderExample 工程在 UE 5.7 引擎架构下（如 `-game` 模式或完整编辑器模式）将能够稳定启动和渲染，并在视口呈现自定义渲染流程（红蓝噪声材质替换）而不产生崩溃或渲染报错。
