# UE-4：AVBOIT 后端

## 前置读取

- 总路线 Skill
- Checkpoint Skill
- MaterialShaderExample Baseline Skill
- CURRENT 与最新 checkpoint
- 本 Plan 指定知识资料

## 目标

逐阶段实现 Clear、Build、Integration、Transparent Shade 和 Resolve。

## 强制调试视图

Occupancy、Slice Index、Extinction、Integrated Transmittance、Radiance、Final Resolve。

## 禁止

不得一次性实现全部后再排错；每阶段必须构建运行。


## 强制 Checkpoint 门禁

每条指令结束时创建不可变 checkpoint、追加索引、更新 CURRENT。未归档不得声称完成。
