# CHECKPOINT-0009: UE1/2 RDG Smoke Result

UTC: 2026-06-22T13:44:19Z

Status: implemented-with-test-gaps

Previous Checkpoint:
`docs/checkpoints/archive/CHECKPOINT-0008-20260622T125902Z-UE1-2-preflight.md`

Supersedes:
`docs/plan/CURRENT.md`

## Repository State

| Item | Value |
|---|---|
| Repository root | `D:\HTC\avboit\AVBOIT开发` |
| Branch | `AVBOIT开发` tracking `origin/AVBOIT开发` |
| Local HEAD | `d0ec701dd2abc79170f5dcf99df5451f78f2fd55` |
| Checkpoint scope | UE-1 baseline plus UE-2 isolated AVBOIT RDG smoke test |
| Latest plan | `docs/plan/ue5_avboit/PLAN-UE-1-2-Baseline-And-RDG-Smoke-Test.md` |
| Working tree | contains this checkpoint, docs updates, runtime smoke-test files, screenshot helper, and generated evidence under `LocalVisualResults/UE57` |

Runtime files added for UE-2:

- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSmokeSettings.h`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSmokeSettings.cpp`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSmokeSubsystem.h`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSmokeSubsystem.cpp`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSceneViewExtension.h`
- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/AVBOITSceneViewExtension.cpp`
- `Plugins/MaterialShaderPlugin/Shaders/AVBOIT/AVBOITSmoke.usf`

Runtime file updated:

- `Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/MaterialShaderExampleModule.cpp`

`MaterialShaderExampleModule.cpp` now registers `AVBOIT.CaptureScreenshotAndExit`
as an evidence helper. It does not change the baseline render path unless the
console command is invoked.

## Reference Worktrees

| Worktree | Branch | HEAD | Role |
|---|---|---|---|
| `D:\HTC\avboit\MaterialShaderExample` | `master` | `a80b000f589e9953fcb5efbfd3eccbf247d62c4a` | baseline reference, left unmodified |
| `D:\HTC\avboit\The-Forge` | `baseline/theforge-1.58-windows-vs-dx12` | `39046051cef37bff0e52e497ca7df07ba9aebe65` | transparency reference, left unmodified |

## Portable UE Requirement

- `MaterialShaderDemo.uproject` remains the portable project contract with
  `EngineAssociation = 5.7`.
- Build target remains `MaterialShaderDemoEditor Win64 Development`.
- Required binaries are a UE 5.7 `UnrealEditor.exe`, `Build.bat`,
  `Build.version`, and UnrealBuildTool.
- No runtime code contains this machine's absolute UE install path.
- Resume instructions must rediscover `UE57_ROOT` before build/run. If the local
  checkout path contains non-ASCII characters, use an ASCII alias such as `W:`
  for UBT/editor commands.

## This Machine UE Discovery

Machine-local evidence from `DESKTOP-14SQPSC`:

| Item | Value |
|---|---|
| Observed `UE57_ROOT` | `E:\tools\Epic Games\UE_5.7` |
| UE version | 5.7.4 |
| Changelist | 51494982 |
| Compatible changelist | 47537391 |
| Branch | `++UE5+Release-5.7` |
| UnrealEditor.exe | present |
| Build.bat | present |
| UnrealBuildTool.dll / .exe | present |
| RenderDoc | present at `C:\Program Files\RenderDoc`, not on `PATH` |
| PIX | not found |

This table is evidence for this machine only. It is stale on another machine
until rediscovered and checkpointed.

## Build/Run Evidence

### UE-1 Baseline

Accepted build command used this machine's UE 5.7 root and the `W:` ASCII alias:

```powershell
& "$env:UE57_ROOT\Engine\Build\BatchFiles\Build.bat" `
  MaterialShaderDemoEditor Win64 Development `
  -Project="W:\MaterialShaderDemo.uproject" `
  -WaitMutex -NoUBA -NoUBTMakefiles
```

Accepted build logs:

- `LocalVisualResults/UE57/Baseline/Logs/Build-MaterialShaderDemoEditor-SubstW-NoUBA.log`
- `LocalVisualResults/UE57/Baseline/Logs/Build-MaterialShaderDemoEditor-AfterScreenshotHelper.log`

The direct non-ASCII path build log
`LocalVisualResults/UE57/Baseline/Logs/Build-MaterialShaderDemoEditor.log` is
discarded evidence. It failed because MSVC saw a mojibake source path under
`D:\HTC\avboit\AVBOIT开发`. The accepted evidence uses `W:`.

Accepted baseline run:

```powershell
& "$env:UE57_ROOT\Engine\Binaries\Win64\UnrealEditor.exe" `
  "W:\MaterialShaderDemo.uproject" "/Game/NewMap" `
  -game -windowed -ResX=1280 -ResY=720 `
  -log -stdout -FullStdOutLogOutput -NoSplash -Unattended `
  "-ExecCmds=r.ShaderDevelopmentMode 1,r.RDG.Debug 1,AVBOIT.CaptureScreenshotAndExit filename=W:/LocalVisualResults/UE57/Baseline/MaterialShaderExample_Baseline_Viewport.png"
```

Accepted baseline artifacts:

| Artifact | Size | SHA-256 |
|---|---:|---|
| `LocalVisualResults/UE57/Baseline/MaterialShaderExample_Baseline_Viewport.png` | 809151 | `D0109DBD022594BFFB3F33FE85C234D2D61EB17DCAC103525421B4362811C827` |
| `LocalVisualResults/UE57/Baseline/Logs/MaterialShaderExample_Game_ScreenshotHelper.log` | recorded | see log |

Discarded artifact:

| Artifact | Reason |
|---|---|
| `LocalVisualResults/UE57/Baseline/MaterialShaderExample_Baseline.png` | OS-level desktop/lock-screen capture, not accepted as UE viewport evidence |

Baseline log evidence includes D3D12, shader model 6.7, NVIDIA GeForce RTX
4060 Ti, `/Game/NewMap`, screenshot request completion, and
`LogShaderCompilers: Display: Shaders left to compile 0`.

### UE-2 Isolated RDG Smoke Test

Implementation contract:

- Disabled by default through `r.AVBOIT.Smoke.Enable = 0`.
- Runtime controls:
  - `r.AVBOIT.Smoke.Enable`
  - `r.AVBOIT.Smoke.DebugMode`
  - `r.AVBOIT.Smoke.Opacity`
- Registers a `UEngineSubsystem`-owned `FSceneViewExtensionBase`.
- Uses UE5.7 public
  `SubscribeToPostProcessingPass(EPostProcessingPass, const FSceneView&, FPostProcessingPassDelegateArray&, bool)`.
- Uses `FPostProcessMaterialInputs`, `Inputs.GetInput(EPostProcessMaterialInput::SceneColor)`,
  and `Inputs.SceneTextures`.
- Does not use Nanite RDG name lookup, `FindTexture`, `VisBuffer`, or hardcoded
  engine path assumptions.
- Adds RDG/GPU markers in code:
  - `AVBOIT.Smoke.Clear`
  - `AVBOIT.Smoke.Fill`
  - `AVBOIT.Smoke.Composite`

Accepted UE-2 build log:

- `LocalVisualResults/UE57/UE2/Logs/Build-MaterialShaderDemoEditor-UE2-Smoke.log`

Build result: `Succeeded`. UBT compiled `AVBOITSmokeSettings.cpp`,
`AVBOITSmokeSubsystem.cpp`, `AVBOITSceneViewExtension.cpp`, generated reflection
code, and linked `UnrealEditor-MaterialShaderExample.dll`.

Accepted standalone run artifacts:

| Case | Resolution | Log | Screenshot | SHA-256 |
|---|---:|---|---|---|
| Disabled | 1280x720 | `LocalVisualResults/UE57/UE2/Logs/AVBOIT_Smoke_Disabled.log` | `LocalVisualResults/UE57/UE2/AVBOIT_Smoke_Disabled.png` | `7986C0EB86B74E5CCFD8E0E0BFEACD4DDAAD718B16740C69A39375D56030FC4B` |
| Mode 1 | 1280x720 | `LocalVisualResults/UE57/UE2/Logs/AVBOIT_Smoke_Mode1.log` | `LocalVisualResults/UE57/UE2/AVBOIT_Smoke_Mode1.png` | `DF2A10AA5E8EE25DC30618623FD9C13FD21F296F0726942CF3D9DE0B489F371D` |
| Mode 2 | 1280x720 | `LocalVisualResults/UE57/UE2/Logs/AVBOIT_Smoke_Mode2.log` | `LocalVisualResults/UE57/UE2/AVBOIT_Smoke_Mode2.png` | `1A2776722CDBC1F6AF3914DEB87F3A79C2C4A3AA216703440970CA189D407B39` |
| Mode 3 | 1280x720 | `LocalVisualResults/UE57/UE2/Logs/AVBOIT_Smoke_Mode3.log` | `LocalVisualResults/UE57/UE2/AVBOIT_Smoke_Mode3.png` | `333C8700947C61D6882EB8DA8D7084012914C9F23F820E43874D1B5528E7F453` |
| Disabled resize | 960x540 | `LocalVisualResults/UE57/UE2/Logs/AVBOIT_Smoke_Disabled_960x540.log` | `LocalVisualResults/UE57/UE2/AVBOIT_Smoke_Disabled_960x540.png` | `88FA8B65B1922FF40A75EE2346C65F624A271B39FAD42FEC6597EA8B422EB285` |
| Mode 1 resize | 960x540 | `LocalVisualResults/UE57/UE2/Logs/AVBOIT_Smoke_Mode1_960x540.log` | `LocalVisualResults/UE57/UE2/AVBOIT_Smoke_Mode1_960x540.png` | `057394B193EBB7DA5020433BC686E29ED977947CCAF9E89A8549B2419F9A358D` |

UE-2 logs show D3D12, shader model 6.7, NVIDIA GeForce RTX 4060 Ti, draw events
for PIX profiling, `/Game/NewMap`, `Shaders left to compile 0`, and screenshot
completion.

Error scan:

```text
Fatal|Error:|Assertion failed|Shader compilation failures|Failed to compile|
LogShaderCompilers: Error|LogMaterial: Error|RDG.*Error|RenderGraph.*Error|
ensure\(|Ensure condition failed
```

Result: 0 matches across accepted UE-2 build/run logs and accepted baseline run
log. The only known failure is the discarded direct-path baseline build log
described above.

## Test Gaps

- Editor and PIE modes were not completed in this pass.
- No RenderDoc or PIX capture was collected. RenderDoc is installed locally, PIX
  is not found, and UE logs only show D3D12 draw events enabled for profiling.
- Standalone resize coverage is present at 960x540 for disabled and Mode 1.

## Current Status

- Repository state separation: PASS
- Portable UE requirement separation: PASS
- This machine UE discovery: PASS
- UE-1 build/run/screenshot: PASS
- UE-2 standalone disabled parity: PASS
- UE-2 standalone DebugMode 1/2/3: PASS
- UE-2 standalone resize: PASS
- Shader/RDG error scan on accepted logs: PASS
- Editor/PIE validation: NOT COVERED
- External GPU capture: NOT COVERED

## Resume

1. Rediscover current-machine `UE57_ROOT`; do not assume
   `E:\tools\Epic Games\UE_5.7` is valid off `DESKTOP-14SQPSC`.
2. If the repo path contains non-ASCII characters, create or verify an ASCII
   alias such as `W:` before UBT/editor commands.
3. Read this checkpoint and `docs/plan/CURRENT.md`.
4. Continue with Editor/PIE validation or external GPU capture before promoting
   this checkpoint from `implemented-with-test-gaps` to `passed`.
