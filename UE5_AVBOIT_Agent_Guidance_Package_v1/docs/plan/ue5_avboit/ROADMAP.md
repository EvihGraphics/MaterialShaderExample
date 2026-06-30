# UE5 AVBOIT Roadmap

| Stage | Goal | Exit Gate |
|---|---|---|
| UE-0 | 冻结源与跨引擎契约 | 两仓库 HEAD、参考视觉与算法契约归档 |
| UE-1 | MaterialShaderExample 基线 | 原样构建运行、GPU 证据 |
| UE-2 | RDG/Scene smoke test | SceneColor/Depth 读写闭环 |
| UE-3 | RGB test content | 单层、双层、三层和六顺序可控 |
| UE-4 | AVBOIT backend | Clear/Build/Integrate/Shade/Resolve 可调试 |
| UE-5 | 自动验证 | Ground Truth 与误差指标通过 |
| UE-6 | UE Alpha reference | 同条件 Mode 0/Mode 1 对比 |
| UE-7 | Card input | 插件私有 Card 接口稳定 |

阶段切换必须由 checkpoint 明确确认退出条件。

## UE-4.2D Status Addendum

- 2026-06-30T17:10:30Z: UE-4.2D Native OIT-Guided AVBOIT Foundation is `partial`.
- Root recovery pointer: `docs/plan/CURRENT.md`.
- Latest checkpoint: `CHECKPOINT-0044-20260630T171030Z-UE-4-2D-NATIVE-OIT-GUIDED-AVBOIT-FOUNDATION.md`.
- KeyResults promotion remains blocked until real Niagara AVBOIT draw, SceneColor composite, and GPU readback gates pass.
