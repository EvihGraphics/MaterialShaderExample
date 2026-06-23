# PLAN: UE-2.2 GPU Capture and Portability Hardening

See `docs/plan/ue5_avboit/UE-2.2 GPU Capture 与可移植性加固计划.md` for the exact original text.
This document serves as the formal plan tracker.

## Summary
- 当前已核验：`MaterialShaderExample_AVBOIT` 在 `AVBOIT开发`，本地/远端 HEAD 均为 `5623a427...`。
- 本轮目标：修复旧状态指针，审计被删/重命名历史证据，移除共享代码里的 `HIVE-4090X2` 默认路径，拆分测试工具边界，并用 RenderDoc 或 PIX 产出可验收 GPU Capture。
- 不进入 UE-3，不修改 UE Engine，不修改 The Forge，不自动 push。
