# UE-3：插件私有 RGB 测试内容

## 前置读取

- 总路线 Skill
- Checkpoint Skill
- MaterialShaderExample Baseline Skill
- CURRENT 与最新 checkpoint
- 本 Plan 指定知识资料

## 目标

建立单层白片、红蓝双层、RGB 三层、六种 Draw Order、六种物理深度顺序和 opaque occluder。

## 验收

测试内容完全由插件控制，不依赖 Niagara、Substrate 或通用透明材质。


## 强制 Checkpoint 门禁

每条指令结束时创建不可变 checkpoint、追加索引、更新 CURRENT。未归档不得声称完成。
