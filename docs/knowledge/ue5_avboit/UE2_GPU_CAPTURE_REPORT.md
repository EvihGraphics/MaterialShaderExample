# UE2 GPU Capture Report

UTC: 2026-06-22T16:34:17Z

## Result

External GPU capture gate: `BLOCKED`

UE-2.1 remains `PARTIAL` because no accepted RenderDoc or PIX event list proves
all three RDG passes:

```text
AVBOIT.Smoke.Clear
AVBOIT.Smoke.Fill
AVBOIT.Smoke.Composite
```

## RenderDoc Attempt

RenderDoc was attempted first with RenderDoc 1.44 and UE `-AttachRenderDoc`.
The UE log shows that the RenderDoc plugin loaded successfully, but no `.rdc`
capture was produced under the closeout GPU capture namespace.

Evidence:

```text
LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout/GPUCapture/RenderDoc/RenderDoc_Attempt.log
```

Observed UE markers:

```text
RenderDocPlugin: RenderDoc plugin is ready!
RenderDocPlugin: Capture frame and launch renderdoc!
```

Rejected reason:

```text
No accepted `.rdc` capture or event list was produced.
```

## PIX Attempt

PIX 2603.25 was attempted next. `pixtool launch` works for an executable with no
arguments, but this local `pixtool` rejected its documented `--command-line`
option in both equals and space-separated forms. Launching UE without arguments
cannot bind the target project, map, or smoke CVars.

`pixtool attach` was also tested against a manually launched UE process and was
rejected by PIX because the process was not launched for GPU capture.

Evidence:

```text
LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout/GPUCapture/PIX/PIX_Capture.stdout.log
LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout/GPUCapture/PIX/PIX_Attach.stdout.log
LocalVisualResults/UE57/HIVE-4090X2/UE2-Closeout/GPUCapture/PIX/PIX_Syntax_*.log
```

Representative failures:

```text
Unknown option 'command-line=...'
PIXTOOL17 - Process not launched for GPU Capture
GPU Capture is not supported for processes not launched for GPU Capture.
```

## Acceptance State

No GPU capture is accepted. The gate remains blocked until RenderDoc produces a
usable `.rdc` with event proof or PIX produces a `.wpix` plus exported event CSV
showing `AVBOIT.Smoke.Clear`, `AVBOIT.Smoke.Fill`, and
`AVBOIT.Smoke.Composite`.

