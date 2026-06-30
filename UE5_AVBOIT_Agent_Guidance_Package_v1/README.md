# AVBOIT Agent Guidance Package

本指导包用于让 Agent 以“帮助用户学习并共同完成工程”为目标，分阶段开发：

1. 在 **The Forge Release 1.58** 的 `15_Transparency` 对比实验场中学习、实现和验证 AVBOIT；
2. 使用 RGB 三面片、交叉卡片和序列帧沙暴卡片完成统一对比；
3. 冻结算法、质量、显存和性能契约；
4. 再迁移为 UE 插件；
5. 最后才接入 Niagara。

## 使用顺序

Agent 每次开始工作前依次读取：

1. `docs/skill/avboit-learning-development-skill-v1/SKILL.md`
2. `docs/skill/checkpoint-archive-skill/SKILL.md`
3. 当前阶段专用 Skill，例如 `docs/skill/theforge-avboit-lab-skill/SKILL.md`
4. `docs/plan/CURRENT.md`
5. `CURRENT.md` 指向的最新 `docs/checkpoints/archive/CHECKPOINT-*.md`
6. 当前 checkpoint 对应的 `PLAN-*.md`
7. 当前计划指定的知识资料

可直接复制的入口 Prompt：

- 完整版：`prompt/AGENT_PROMPT_FULL.md`
- 精简版：`prompt/AGENT_PROMPT_COMPACT.md`

## 每轮指令强制归档

每条用户指令都构成一个独立 Instruction Cycle。Agent 在回复用户前必须：

1. 创建新的不可变 checkpoint；
2. 追加 `docs/checkpoints/CHECKPOINT_INDEX.md`；
3. 更新 `docs/plan/CURRENT.md`；
4. 在回复中给出 checkpoint 路径、状态和下一恢复入口。

成功、部分完成、阻塞、失败以及无文件改动都必须归档。**没有 checkpoint，就不能声称该轮完成。**

详细协议：`docs/skill/checkpoint-archive-skill/SKILL.md`

## 固定基线

- The Forge 主开发基线：Release 1.58
- Commit：`2f47c1445ca0062998b4b4aa81e5346a4c3cdf2d`
- The Forge 历史参考：Release 1.16
- Commit：`9ec216b40968365299362b305c0fed2f97d4be1b`

禁止跟随 master 开发。禁止将 Intel AOIT 与 Activision AVBOIT 混为同一算法。

## 资料规则

- 原始 PDF 直接保留在 `sources/pdf/`。
- EgakuRenderPipeline 作为代码参考保留在 `sources/reference_code/`。
- The Forge 不整库打包，只提供固定 commit 与链接指引。
- Niagara、Substrate 和 UE 插件资料属于未来阶段，当前 The Forge checkpoint 默认不加载。

## 参考项目与视觉验证规范

- **参考项目库 (UERP_Ref)**: `MaterialShaderExample_AVBOIT/UERP_Ref` 目录下包含了所有相关的引擎级和插件级参考实现（如 ComputeShaderDemo、UE5CustomSSGI 等）。在测试与升级时，所有该目录下的 `.uproject` 和 `.uplugin` 必须保持引擎版本与当前开发环境完全一致。
- **视觉结果留存 (LocalVisualResults)**: 所有的项目编译后运行的视觉结果、日志和验证材料必须严格按照规范留存于 `MaterialShaderExample_AVBOIT/LocalVisualResults` 目录下：
  - **KeyResults**: 存放主要视觉对比结果、功能成功验证的关键截图、或阶段性完成的 Markdown 总结及确凿的运行日志。
  - **TempResults**: 存放临时生成的运行数据、崩溃排查日志、或非结论性的中间调试产物。
