# Source Priority and Boundaries

## 权威顺序

1. UE5.7 真实源码、实际编译与运行结果；
2. AVBOIT 正式 PDF 的 Markdown 转译；
3. The Forge 已验证 AVBOIT 源码、视觉结果和测试；
4. 数学 Ground Truth；
5. MaterialShaderExample 的真实工程；
6. Single Volume PDF 转译；
7. UE 渲染管线官方课程、源码教程和博客；
8. EgakuRenderPipeline；
9. 论坛与零散文章。

## 职责边界

- MaterialShaderExample：UE 插件工程起点。
- The Forge：算法、资源语义、视觉与验证核心参考。
- AVBOIT PDF：算法规范。
- Single Volume PDF：复杂 UE 插件渲染管线的工程指导。
- Egaku：Unity 行为与替代实现参考，不决定 UE 架构。

## 明确排除

`SingleVolume.zip` 完全排除。指导包中的任何文档不得由该 ZIP 派生。
