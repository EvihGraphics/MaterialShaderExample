# UE-6：UE 标准 Alpha 参考

## 前置读取

- 总路线 Skill
- Checkpoint Skill
- MaterialShaderExample Baseline Skill
- CURRENT 与最新 checkpoint
- 本 Plan 指定知识资料

## 目标

同一场景提供 UE 标准有序 Alpha Blend 和 AVBOIT 两个模式，在非交叉受控场景中进行公平比较。

## 禁止

不把复杂交叉场景的对象级排序当成逐像素 Ground Truth。


## 强制 Checkpoint 门禁

每条指令结束时创建不可变 checkpoint、追加索引、更新 CURRENT。未归档不得声称完成。
