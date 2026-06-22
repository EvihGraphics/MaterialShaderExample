# UE-2：RDG 与场景资源闭环

## 前置读取

- 总路线 Skill
- Checkpoint Skill
- MaterialShaderExample Baseline Skill
- CURRENT 与最新 checkpoint
- 本 Plan 指定知识资料

## 目标

读取 SceneColor/SceneDepth，创建插件私有 RDG 资源，执行可识别 Pass，并安全 Resolve/调试显示。

## 验收

- ViewRect 和 extent 正确；
- Reversed-Z 记录；
- PIE/Standalone 可运行；
- Resize 不崩溃；
- GPU Marker 可见。


## 强制 Checkpoint 门禁

每条指令结束时创建不可变 checkpoint、追加索引、更新 CURRENT。未归档不得声称完成。
