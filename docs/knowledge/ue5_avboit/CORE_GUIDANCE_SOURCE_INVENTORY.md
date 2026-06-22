# Core Guidance Source Inventory

Generated UTC: 2026-06-22T08:19:40Z

## Roots

| Item | Path | Status |
|---|---|---|
| AVBOIT worktree | `D:\HTC\avboit\AVBOIT开发` | present |
| Baseline worktree | `D:\HTC\avboit\MaterialShaderExample` | present, read-only baseline |
| Guidance root in AVBOIT worktree | `D:\HTC\avboit\AVBOIT开发\UE5_AVBOIT_Agent_Guidance_Package_v1` | present |
| Markdown root in AVBOIT worktree | `UE5_AVBOIT_Agent_Guidance_Package_v1\references\pdf_md` | present |
| PDF root in AVBOIT worktree | `UE5_AVBOIT_Agent_Guidance_Package_v1\sources\pdf` | missing, because `*.pdf` is ignored and not tracked |
| External local PDF evidence | `D:\HTC\avboit\MaterialShaderExample\UE5_AVBOIT_Agent_Guidance_Package_v1\sources\pdf` | present in baseline checkout only |

## Markdown Files In AVBOIT Worktree

| File | Bytes | SHA-256 |
|---|---:|---|
| `INDEX.md` | 539 | `CC15C32F61A7EDD8CE2DD46D32F41969F841C037F095479D6288B42088B68D67` |
| `AVBOIT_SIG2025_MDROBOT-final.md` | 223722 | `B40D174DBA1CF34D0871AA70CD348429503F307975F628B8A2548550133D5D82` |
| `在UE5外部模块中扩展复杂渲染管线实践.md` | 349924 | `F8A1F9492396B13F26FCCCBF24AFB0FC7E7945127784083137BAA71F6E127749` |
| `在UE5中打造多层体积天空.md` | 110733 | `D8EEE5812EDE542ED8A6B4CCA7C740DA7B47F54807A617EAA3960D94CBF5E497` |
| `MaterialShader.txt` | 53 | `78108A8501C770146DB046BCDDEAAAFBD77501FB1BFA751762F6307FF57B0257` |

## External Local PDF Evidence

These files are ignored by `.gitignore` and are not present in the new worktree. They were hashed from the original local baseline checkout and must be treated as machine-local evidence, not commit-ready content.

| File | Bytes | SHA-256 |
|---|---:|---|
| `AVBOIT_SIG2025_MDROBOT-final.pdf` | 6089237 | `60A04C28C6C403BBF32D30D757302F7241561D5106A5E5DD56FF1D9D68D46708` |
| `在UE5外部模块中扩展复杂渲染管线实践.pdf` | 6535918 | `E03E4695F2635F38DB198532F7651AB2DC261062B96CA03C027B645898DEF94A` |
| `在UE5中打造多层体积天空.pdf` | 32824810 | `4F50C766DA8E1C868B7B160B9E4781A18A3D82535C3EDA5F5589966F8F8567BB` |

## Missing Or Drifted Sources

| Expected Source | Status | Impact |
|---|---|---|
| `references\pdf_md\近三年工业级实时天气与体渲染案例研究.md` | missing in baseline and AVBOIT worktree | P2 product/weather context only; non-blocking for bootstrap |
| `sources\pdf\近三年工业级实时天气与体渲染案例研究.pdf` | missing in baseline and AVBOIT worktree | P2 product/weather context only; non-blocking for bootstrap |
| `UE5_AVBOIT_Agent_Guidance_Package_v1\sources\pdf` in AVBOIT worktree | missing | Partial inventory; external local PDFs recorded above |

## Inventory Result

Status: `partial`

Reason: P0/P1 Markdown sources are present, but ignored PDF evidence is absent from the new worktree and exists only in the original checkout. The weather-study file listed by `INDEX.md` is missing and recorded as non-blocking drift.

