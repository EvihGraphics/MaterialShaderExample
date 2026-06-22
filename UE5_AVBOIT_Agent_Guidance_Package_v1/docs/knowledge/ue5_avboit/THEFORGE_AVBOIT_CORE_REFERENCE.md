# The Forge AVBOIT Core Reference

## 仓库与参考分支

```text
Repository:
https://github.com/EvihGraphics/The-Forge

Reference branch URL:
https://github.com/EvihGraphics/The-Forge/tree/baseline/theforge-1.58-windows-vs-dx12
```

## 已知历史锚点

- The Forge Release 1.58 原始基线：`2f47c1445ca0062998b4b4aa81e5346a4c3cdf2d`
- 最近一次已知 AVBOIT 视觉修正提交：`717c3c3b310b493919424d9c6d451ca39c877f46`
- 修正主题：把 AVBOIT 深度切片聚焦到透明几何所在的近距离范围，以改善透射率混合精度。

Agent 不得只相信上述 SHA。首次执行必须 fetch 远端并归档当前精确 HEAD。

## 核心检查对象

- `15_Transparency` Unit Test；
- Mode 0 标准 Alpha Blend；
- Mode 5 AVBOIT；
- AVBOIT Clear / Build / Integration / Shade / Resolve；
- 深度线性化、slice 映射、`volumeMaxZ`；
- Extinction / Transmittance 资源；
- `LocalVisualResults/HIVE_4090x2/VisualResults/15_Transparency/Screenshots/`；
- `UT_15_Transparency_DX12_Mode_0.png`；
- `UT_15_Transparency_DX12_Mode_5.png`；
- RGB Ground Truth 与 Draw Order 测试。

## 迁移原则

迁移算法与契约，不复制 The Forge 的框架 API、资源类或目录结构。
