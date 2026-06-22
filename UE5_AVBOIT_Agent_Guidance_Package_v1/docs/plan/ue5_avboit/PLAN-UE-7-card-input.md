# UE-7：插件私有 Card 输入

## 前置读取

- 总路线 Skill
- Checkpoint Skill
- MaterialShaderExample Baseline Skill
- CURRENT 与最新 checkpoint
- 本 Plan 指定知识资料

## 目标

把固定测试几何提升为稳定 Card/Mesh 数据接口，为 Flipbook 沙暴播放器准备。

## 进入条件

UE-5 与 UE-6 已通过。当前仍不接 Niagara。


## 强制 Checkpoint 门禁

每条指令结束时创建不可变 checkpoint、追加索引、更新 CURRENT。未归档不得声称完成。
