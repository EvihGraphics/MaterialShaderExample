# UE5.7 Rendering Pipeline Map for AVBOIT

## 最小数据流

```text
Game Thread test parameters
        ↓
Render-thread proxy / immutable snapshot
        ↓
SceneViewExtension or selected renderer hook
        ↓
RDG private resources
        ↓
AVBOIT Clear
        ↓
Test geometry Build
        ↓
Transmittance Integration
        ↓
Transparent Color
        ↓
Resolve with SceneColor / SceneDepth
        ↓
Linear readback and final screenshot
```

## 必须核验

- 插入点是否在 opaque SceneColor / SceneDepth 可用之后；
- 是否在需要的 Tone Mapping 之前；
- Reversed-Z 和线性深度公式；
- ViewRect、dynamic resolution 和 texture extent；
- PIE、Standalone 和多次进入 PIE 的生命周期；
- RDG 资源是否每帧正确清理；
- UAV/SRV/RTV 状态与 Barrier；
- Shader permutation 和 Cook。

## 原型与产品边界

SceneViewExtension 可以承担 Unit Test 和早期插件原型。是否能接管普通 UE Translucent Mesh，需要在 RGB Unit Test 通过后重新评估。
