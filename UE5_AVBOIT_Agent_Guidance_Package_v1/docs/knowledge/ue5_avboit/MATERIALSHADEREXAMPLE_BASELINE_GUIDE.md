# MaterialShaderExample Baseline Guide

## 仓库

```text
https://github.com/EvihGraphics/MaterialShaderExample
```

## 定位

这是 UE5 AVBOIT 插件的唯一工程起始基线。README 将其定义为一个 `FMaterialShader` 小型示例，并展示了 Nanite Visibility Buffer 接入、插件 C++ 目录和 `.usf` Shader 位置。

## Agent 必须先完成

1. 从远端重新克隆；
2. 记录 default branch、HEAD、submodule 和 LFS；
3. 确认目标 UE 版本；
4. 原样构建；
5. 原样运行；
6. 验证 Actor、Material Shader、SceneViewExtension、RDG/Compute 路径；
7. 保存 GPU Marker、截图和日志；
8. 建立原始基线 checkpoint；
9. 才允许创建 AVBOIT 开发分支。

## 不允许

- 从空白插件开始；
- 未运行原示例就删除 Nanite 示例路径；
- 把示例的具体 Nanite 功能误当作 AVBOIT 设计；
- 在基线未归档前大规模重构。

## 迁移关系

```text
MaterialShaderExample
├─ Module / Shader Directory
├─ SceneViewExtension
├─ FMaterialShader
├─ View / Material Shader Map
├─ RDG Texture / UAV
└─ Compute Dispatch

UE AVBOIT
├─ Plugin lifecycle
├─ AVBOIT test content
├─ Clear / Build / Integrate / Shade / Resolve
├─ SceneDepth / SceneColor
└─ Validation readback
```
