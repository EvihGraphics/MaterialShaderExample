# The Forge AVBOIT Source Map

Generated UTC: 2026-06-22T08:19:40Z

## Repository

| Item | Value |
|---|---|
| Worktree | `D:\HTC\avboit\The-Forge` |
| Remote | `https://github.com/EvihGraphics/The-Forge.git` |
| Branch | `baseline/theforge-1.58-windows-vs-dx12` |
| HEAD | `39046051cef37bff0e52e497ca7df07ba9aebe65` |
| AVBOIT root | `Examples_3\Unit_Tests\src\15_Transparency` |

## Core Files

| File | Bytes | SHA-256 |
|---|---:|---|
| `15_Transparency.cpp` | 247986 | `443656BCD4FB11AD3567940611C57E0A4C74000E91D567F7AB8F886F592B4C69` |
| `Shaders\FSL\AVBOIT.frag.fsl` | 1687 | `10B45B0561E839B33614C296974EFA3C35D98AE3921CFBD4EF7A348CD6F43475` |
| `Shaders\FSL\avboit.h.fsl` | 1229 | `686A4768071CC8B907E9118EC267CDDE5AD4694E2278B8011A2A058E4C8111FF` |
| `Shaders\FSL\avboit_clear.comp.fsl` | 401 | `A22BC579F43CCBC27182C85BF409920F09312AF88FD22533B4571C87A007650C` |
| `Shaders\FSL\avboit_integrate.comp.fsl` | 694 | `52748C8F990821F16F9833AB5C171DFB76FD9235F29FB803744867B59FB2A6A8` |
| `Shaders\FSL\avboit_composite.frag.fsl` | 1043 | `52E6EB5A26C1621F50573B6BBD4912A2AC9CFFF65E9A5E7D3CF12BD48C88D08A` |
| `Shaders\FSL\avboit_forward.frag.fsl` | 2004 | `C2D315D502668C5E5413B2E7E9F5442740B8BD4DBAD02B48D63C68F8CB083517` |
| `Scripts\Test_AVBOIT.lua` | 185 | `5AA56A50F2356654E3B8FF249E3B2DB30D08D756158F3B4DABAB71B9553B4640` |

## Pass Chain Confirmed From Source

| Stage | Source Evidence |
|---|---|
| Clear | `Clear AVBOIT` marker, compute dispatch over width/height/depth |
| Build/Splat | `Splat AVBOIT` marker and `DrawObjects(... pRootSignatureAVBOITSplat)` |
| Integrate | `Integrate AVBOIT` marker and 2D compute dispatch over width/height |
| Composite | `Composite AVBOIT` marker and composite pipeline |
| Forward/Shade | `Forward AVBOIT` marker and `DrawObjects(... pRootSignatureAVBOITForward)` |

Current reference behavior includes `gAVBOITMultiplier`; UE deterministic tests must record it separately from the math ground truth.

