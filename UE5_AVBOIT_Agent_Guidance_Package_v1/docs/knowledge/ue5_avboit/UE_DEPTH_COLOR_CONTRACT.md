# UE AVBOIT Depth and Color Contract

## 深度

- 记录 UE Reversed-Z；
- 明确 Device Z、View-space Z、linear depth 和 normalized AVBOIT depth；
- Build 与 Resolve 必须使用同一函数；
- 深度范围不能用“看起来合适”的常量替代可解释策略；
- SceneDepth rejection 必须通过不透明遮挡测试。

## 颜色

- 明确 Straight Alpha 与 Premultiplied Alpha；
- 明确比较发生在线性 HDR 还是 sRGB PNG；
- Ground Truth：黑背景、前红中绿后蓝、Alpha 0.5，线性 RGB = `(0.5, 0.25, 0.125)`；
- 背景项：`C_out = C_transparent + T_total * C_background`；
- 禁止重复乘 Alpha 或把 Alpha 直接当 Optical Depth。
