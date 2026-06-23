# UE-2.2 GPU Capture 与可移植性加固计划

**状态**: `PASSED`
**日期**: 2026-06-23
**目标**: 在验证 Mode 1/2/3 和决定性之后，解耦硬编码机器路径，完成自动化证据抓取工具链，产出经 RenderDoc / PIX 验证的 `.rdc` / `.wpix`，为 UE-3 (AVBOIT 算法重构) 铺平道路。

## Summary
- 当前已核验：`MaterialShaderExample_AVBOIT` 在 `AVBOIT开发`，本地/远端 HEAD 均为 `5623a4270394cb3789ed52da01261339c51706fd`，ahead/behind 为 `0/0`，工作树干净。
- 本轮目标：修复旧状态指针，审计被删/重命名历史证据，移除共享代码里的 `HIVE-4090X2` 默认路径，拆分测试工具边界，并用 RenderDoc 或 PIX 产出可验收 GPU Capture。
- 不进入 UE-3，不修改 UE Engine，不修改 The Forge，不自动 push。

## Key Changes
- 创建 `PLAN-UE-2-2-GPU-Capture-And-Portability-Hardening.md`，并先创建 `CHECKPOINT-0012-<UTC>-UE2-2-preflight.md`；结果阶段创建 `CHECKPOINT-0013-<UTC>-UE2-2-result.md`。
- 更新 `CURRENT.md`、`UE57_MACHINE_VERIFICATION_MATRIX.md`、`UE57_PROJECT_CONTRACT.md`，明确旧 Checkpoint 0011 记录的 base 为 `59d8c84...`，但当前真实 HEAD 为 `5623a427...`。
- 新增 `UE2_HISTORICAL_EVIDENCE_AUDIT.md`：列出 `59d8c84... -> 5623a427...` 中 `LocalVisualResults` 的删除/重命名；从 `59d8c84...` 恢复被旧 checkpoint 引用的 Baseline 证据到 `LocalVisualResults/UE57/Historical/DESKTOP-14SQPSC/UE1-Baseline/59d8c84.../`，不覆盖当前证据。
- 将 `Saved/AVBOIT/LocalMachine/` 加入 `.git/info/exclude`，不提交 `ue57-environment.json`。

## Interfaces And Code
- 新增 `Private/AVBOIT/Testing/`，把 `MaterialShaderExampleModule.cpp` 中的测试工具拆到：
  `AVBOITEvidencePath.*`、`AVBOITScreenshotAutomation.*`、`AVBOITEditorPIEAutomation.*`、`AVBOITEvidenceCommands.*`。
- 保留现有控制台命令和截图命名：`AVBOIT.CaptureScreenshotAndExit`、`AVBOIT.CaptureSmokeScreenshotAndExit`、`AVBOIT.RunEditorPIECloseout`。
- 增加路径解析接口：`ResolveAVBOITMachineId()` 与 `ResolveAVBOITEvidenceRoot(Stage, ExplicitRoot)`；优先级为 explicit `root=` > ignored local JSON > computer name > `UNKNOWN-MACHINE`，`UNKNOWN-MACHINE` 必须 warning。
- 默认 evidence root 改为 `LocalVisualResults/UE57/<MachineId>/UE2-Closeout`，不再在共享 C++ 中写死 `HIVE-4090X2`。
- `Build.cs` 保持 `LevelEditor`/`UnrealEd` 仅在 `Target.bBuildEditor` 下；为 JSON 读取增加 `Json` 私有依赖；保留并在 `UE2_MODULE_BOUNDARY_AUDIT.md` 记录 `Renderer/Private` 与 `Renderer/Internal` 的真实用途。

## Validation And Capture
- 使用用户提供 UE：`D:\UE\UnrealEngine_Animation_Tech`，确认 UE `5.7.4`；先构建 `MaterialShaderDemoEditor Win64 Development`，再构建 `MaterialShaderDemo Win64 Development` 以验证非 Editor target。
- 重跑 Editor、PIE、Standalone、DebugMode 0/1/2/3、Resize、生命周期、原示例回归和 accepted log scan；所有新证据写入当前机器 root。
- 工具发现保存到 `GPUCapture/ToolDiscovery/`：RenderDoc 1.44 的 `version/help/capture/inject/replay`，PIX 2603.25 的 `--help`、`--help launch`、`--help attach`、`--help save-event-list`。
- 首选 RenderDoc：用 `renderdoccmd capture -d <RepoRoot> -c <RenderDocCaptureTemplate> <UnrealEditor.exe> <ProjectFile> /Game/NewMap -game -d3d12 ... -ExecCmds="r.RDG.Events 1,r.RDG.Debug 1,r.AVBOIT.Smoke.Enable 1,r.AVBOIT.Smoke.DebugMode 2,renderdoc.CaptureDelay 3,renderdoc.CaptureFrame,AVBOIT.CaptureSmokeScreenshotAndExit mode=2 filename=<probe.png>"`；必须产生可重开 `.rdc`。
- RenderDoc 失败才走 PIX：优先 `pixtool launch <UnrealEditor.exe> --command-line=<args> --working-directory=<RepoRoot> ... take-capture save-capture`；CLI 不可靠时使用 PIX GUI Launch Win32，同样参数，保存 `.wpix`，再用 `pixtool open-capture ... save-event-list ...` 导出事件。
- Capture 验收必须包含 `.rdc` 或 `.wpix`、`EventList.txt/csv`、事件树截图、Clear/Fill/Composite 资源截图、`AVBOIT_RDG_Smoke_Capture_Report.md`，并证明 `AVBOIT.Smoke.Clear`、`Fill`、`Composite` 的顺序、资源绑定、SceneDepth/SceneColor、barrier 与格式。

## Assumptions
- 当前机器 ID 默认来自 local JSON：`HIVE-4090X2`；UE root 来源为 user-provided。
- `MaterialShaderExample` master worktree只读且保持不变；The Forge 当前本地树有脏状态，但本轮只读 tracked HEAD/必要参考文件，不清理、不恢复。
- 如果 RenderDoc 与 PIX 都不能生成可验收 capture，则记录失败证据，UE-2 保持 `PARTIAL`，不进入 UE-3。
- 最终本地提交建议三段：状态/证据审计、测试工具可移植性、capture 验收或 blocked capture 记录；不 push。
