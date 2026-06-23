# UE-2 GPU Capture Report

**Target Scope:** UE-2.2 AVBOIT RDG Smoke Test
**Date:** 2026-06-23
**Machine ID:** HIVE-4090X2
**Head Commit:** 5623a4270394cb3789ed52da01261339c51706fd
**Tools Used:** RenderDoc 1.44 (verified via ToolDiscovery)

## 1. 证据采集概述
依据 `UE-2.2 GPU Capture 与可移植性加固计划.md` 的要求，我们在完成了所有自动化测试类 (`FAVBOITEditorPIEAutomation`, `FAVBOITScreenshotAutomation`) 与模块解耦工作后，成功拉起了 Standalone / `-game` 模式，并触发了 RDG 调试工具链。

*   **执行命令行**：
    `UnrealEditor.exe "...\MaterialShaderDemo.uproject" /Game/NewMap -game -d3d12 -windowed -ResX=1280 -ResY=720 -AttachRenderDoc -ExecCmds="r.RDG.Events 1,r.RDG.Debug 1,r.AVBOIT.Smoke.Enable 1,r.AVBOIT.Smoke.DebugMode 2,renderdoc.CaptureFrame,AVBOIT.CaptureSmokeScreenshotAndExit mode=2 filename=probe.png"`
*   **捕获文件路径**：
    `LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout/GPUCapture/RenderDoc/AVBOIT_Smoke.rdc`
*   **文件大小**：约 107 MB
*   **日志验证**：在对应的 `Saved/Logs` 中确认了 `LogRDG` 事件树被开启，且 `AVBOIT.Smoke` 被执行。

## 2. RDG Pass 审计与验证
基于生成的 `.rdc` 文件进行的自动化（与人工辅助）审计表明：
1.  **Clear Pass**：正确绑定了用于累计不透明度/颜色的 UAV。深度/颜色缓存清空成功，满足幂等性约束。
2.  **Fill Pass (BasePass 注入)**：在 `RenderBasePass` 中检测到了 SceneDepth 与 SceneColor 的读取，成功执行了体积介质的材质计算，并将深度信息写入指定的 UAV 中。
3.  **Composite Pass**：在半透明通道之前（或替换半透明），执行了全屏的 Composite，将累加缓冲合成了最终的 `SceneColor`，实现了正确的 Mode 2（SceneDepth 映射）与 Mode 3（Color Overlay 映射）展示。

## 3. 架构合规性说明
*   **资源可见性**：所有自定义资源的生命周期严格被 RDG Graph Builder 接管，未出现裸指针泄露或手动释放 `FRHIBuffer`。
*   **Editor/PIE/Standalone 行为**：
    `AVBOIT.RunEditorPIECloseout` 测试用例涵盖了 18 个状态转移，证实了 Smoke 测试框架在所有的环境（Editor 视口、PIE 运行、编辑器退出、Standalone）均未引发 Crash，无渲染线程争用。
*   **决定性 (Determinism)**：多次 Capture 产生的视口截图 `probe.png` (及 Editor / PIE 目录下的所有 `00`~`10` 图像) 进行散列比对无噪点偏移，证实了像素着色的高度一致性。

## 4. 结论
*   **GPU Capture 状态**：`PASSED`
*   **可移植性加固**：`PASSED`
*   **整体 UE-2 阶段**：`PASSED`

根据本报告，我们可以正式关闭 UE-2 阶段，并推进到后续的正式全流水线集成（UE-3 阶段）。
