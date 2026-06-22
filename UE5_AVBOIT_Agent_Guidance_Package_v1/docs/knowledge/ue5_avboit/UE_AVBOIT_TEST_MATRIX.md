# UE AVBOIT Test Matrix

| Test | Expected |
|---|---|
| 单层白片 Alpha 0.5 | 线性 RGB `(0.5,0.5,0.5)` |
| 前红后蓝 Alpha 0.5 | `(0.5,0,0.25)` |
| 红绿蓝三层 | `(0.5,0.25,0.125)` |
| 六种 Draw Order | 输出不变 |
| 六种物理深度顺序 | 按深度改变 |
| Opaque occluder | SceneDepth 正确遮挡 |
| Camera motion | 无资源失效和闪烁 |
| Resize | RDG 资源重建正确 |
| PIE restart | 无悬空代理和历史污染 |
| Standalone | 与 PIE 契约一致 |

## 指标

- MAE、RMSE、Max Absolute Error、PSNR；
- Draw Order 方差；
- NaN/Inf 数量；
- 透明区域与背景区域分开统计；
- GPU 时间和中间显存。
