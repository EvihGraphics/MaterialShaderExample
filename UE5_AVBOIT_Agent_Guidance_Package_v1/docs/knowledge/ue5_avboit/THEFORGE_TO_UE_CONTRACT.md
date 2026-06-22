# The Forge to UE Contract

## 冻结内容

- Alpha、Extinction、Optical Depth、Transmittance 定义；
- 深度空间和归一化范围；
- Clear / Build / Integration / Transparent Shade / Resolve 顺序；
- 中间资源的值域、格式和清理值；
- 前方透射率为 inclusive 还是 exclusive；
- 背景项合成公式；
- RGB Ground Truth；
- 六种提交顺序；
- Mode 0 / Mode 5 视觉基线；
- 已知误差和性能预算。

## 不冻结内容

- The Forge API；
- Descriptor/Root Signature 组织；
- 文件和类名；
- Command Buffer 结构；
- 平台宏；
- The Forge 的 UI 和资源加载方式。

## UE 对应

```text
The Forge Pass             UE5.7 目标抽象
Clear                      RDG Clear / Compute Pass
Build                      插件私有几何或 Material Shader Pass
Integration                Global/Compute Shader RDG Pass
Transparent Shade          Material Shader 或受控 Test Shader
Resolve                    Full-screen RDG Pass 到 SceneColor
Visual Capture             线性纹理读回 + 最终截图
```
