# UE-5：自动正确性验证

## 前置读取

- 总路线 Skill
- Checkpoint Skill
- MaterialShaderExample Baseline Skill
- CURRENT 与最新 checkpoint
- 本 Plan 指定知识资料

## 目标

在线性颜色空间读回结果，计算 MAE、RMSE、Max Error、PSNR、Draw Order 方差和 NaN/Inf。

## Ground Truth

RGB 三层黑背景：`(0.5, 0.25, 0.125)`。


## 强制 Checkpoint 门禁

每条指令结束时创建不可变 checkpoint、追加索引、更新 CURRENT。未归档不得声称完成。
