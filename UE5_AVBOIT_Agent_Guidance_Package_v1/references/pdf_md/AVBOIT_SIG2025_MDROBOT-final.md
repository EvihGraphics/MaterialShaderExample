# Adaptive Voxel-Based OIT - Markdown 转译

## 资料身份

- 原始文件：`AVBOIT_SIG2025_MDROBOT-final.pdf`
- 页数：136
- 用途：AVBOIT 算法规范、质量和性能参考
- 转译方式：按页提取文本，并对可识别的代码／接口片段生成代码块。
- 原始 PDF 未包含在本指导包中，以适配仓库忽略 PDF 的策略。

## 使用约束

- 本 Markdown 是 Agent 可检索版本，不替代原 PDF 的视觉排版。
- 架构图、流程图和多栏代码应结合页码理解。
- 代码转录中无法完全确认的行不得直接复制进生产代码，必须回到真实 UE5.7 源码或实际工程验证。

## 页码索引

- [第 1 页：ADAPTIVE VOXEL-BASED](#第-1-页adaptivevoxelbased)
- [第 2 页：INTRODUCTION](#第-2-页introduction)
- [第 3 页：THE OIT TEAM](#第-3-页theoitteam)
- [第 4 页：ACKNOWLEDGEMENTS](#第-4-页acknowledgements)
- [第 5 页：Our games need technology to support vastly different scenarios and environments.](#第-5-页ourgamesneedtechnologytosupportvastlydifferentscenariosandenvironments)
- [第 6 页：Tight, close quarter combat running at highest possible framerate.](#第-6-页tightclosequartercombatrunningathighestpossibleframerate)
- [第 7 页：Crazy spectacular modes that are full of hundreds of NPCs with absolute visual](#第-7-页crazyspectacularmodesthatarefullofhundredsofnpcswithabsolutevisual)
- [第 8 页：As well as super high quality in-game cinematics that require highest precision and](#第-8-页aswellassuperhighqualityingamecinematicsthatrequirehighestprecisionand)
- [第 9 页：TRANSPARENCY IN GAMES](#第-9-页transparencyingames)
- [第 10 页：PAINTERS ALGORITHM](#第-10-页paintersalgorithm)
- [第 11 页：SOLUTIONS :](#第-11-页solutions)
- [第 12 页：SOLUTIONS :](#第-12-页solutions)
- [第 13 页：WEIGHTED BLENDING](#第-13-页weightedblending)
- [第 14 页：WEIGHTED BLENDING :](#第-14-页weightedblending)
- [第 15 页：WEIGHTED BLENDING :](#第-15-页weightedblending)
- [第 16 页：SOLUTIONS :](#第-16-页solutions)
- [第 17 页：APPROXIMATE TRANSMITTANCE INTEGRAL](#第-17-页approximatetransmittanceintegral)
- [第 18 页：PROJECT TRANSMITTANCE ON FUNCTION BASIS](#第-18-页projecttransmittanceonfunctionbasis)
- [第 19 页：TRANSMITTANCE INTEGRAL WEIGHTED OIT](#第-19-页transmittanceintegralweightedoit)
- [第 20 页：TRANSMITTANCE INTEGRAL WEIGHTED OIT](#第-20-页transmittanceintegralweightedoit)
- [第 21 页：OIT JOURNEY](#第-21-页oitjourney)
- [第 22 页：CALL OF DUTY TRANSPARENCY RENDERING](#第-22-页callofdutytransparencyrendering)
- [第 23 页：MANUAL / CPU SORT FAIL CASES](#第-23-页manualcpusortfailcases)
- [第 24 页：CALL OF DUTY TRANSPARENCY RENDERING](#第-24-页callofdutytransparencyrendering)
- [第 25 页：CALL OF DUTY TRANSPARENCY RENDERING](#第-25-页callofdutytransparencyrendering)
- [第 26 页：NEW SOLUTION](#第-26-页newsolution)
- [第 27 页：MOMENT-BASED OIT [PMWK17][SHA18]](#第-27-页momentbasedoitpmwk17sha18)
- [第 28 页：WAVELET-BASED OIT [ASM21]](#第-28-页waveletbasedoitasm21)
- [第 29 页：NO OIT](#第-29-页nooit)
- [第 30 页：MBOIT](#第-30-页mboit)
- [第 31 页：WBOIT](#第-31-页wboit)
- [第 32 页：NO OIT](#第-32-页nooit)
- [第 33 页：MBOIT](#第-33-页mboit)
- [第 34 页：WBOIT](#第-34-页wboit)
- [第 35 页：MBOIT NOT DEPTH COMPLEXITY INVARIANT](#第-35-页mboitnotdepthcomplexityinvariant)
- [第 36 页：MBOIT NOT DEPTH COMPLEXITY INVARIANT](#第-36-页mboitnotdepthcomplexityinvariant)
- [第 37 页：MBOIT NOT DEPTH COMPLEXITY INVARIANT](#第-37-页mboitnotdepthcomplexityinvariant)
- [第 38 页：MBOIT NOT DEPTH COMPLEXITY INVARIANT](#第-38-页mboitnotdepthcomplexityinvariant)
- [第 39 页：WBOIT BETTER COMPLEXITY INVARIANCE](#第-39-页wboitbettercomplexityinvariance)
- [第 40 页：WBOIT BETTER COMPLEXITY INVARIANCE](#第-40-页wboitbettercomplexityinvariance)
- [第 41 页：WBOIT BETTER COMPLEXITY INVARIANCE](#第-41-页wboitbettercomplexityinvariance)
- [第 42 页：WBOIT NOT SHIFT INVARIANT](#第-42-页wboitnotshiftinvariant)
- [第 43 页：WBOIT NOT SHIFT INVARIANT](#第-43-页wboitnotshiftinvariant)
- [第 44 页：WBOIT NOT SHIFT INVARIANT](#第-44-页wboitnotshiftinvariant)
- [第 45 页：NEED NEW SOLUTION](#第-45-页neednewsolution)
- [第 46 页：VOXEL-BASED](#第-46-页voxelbased)
- [第 47 页：VOXEL-BASED OIT](#第-47-页voxelbasedoit)
- [第 48 页：VBOIT : BASIC STEPS](#第-48-页vboitbasicsteps)
- [第 49 页：VBOIT : DEPTH DISTRIBUTION](#第-49-页vboitdepthdistribution)
- [第 50 页：VBOIT : DEPTH DISTRIBUTION](#第-50-页vboitdepthdistribution)
- [第 51 页：VOXELIZATION WITH RASTERIZATION](#第-51-页voxelizationwithrasterization)
- [第 52 页：VOXELIZATION WITH ATOMICS](#第-52-页voxelizationwithatomics)
- [第 53 页：EXTINCTION VOXELIZATION](#第-53-页extinctionvoxelization)
- [第 54 页：EXTINCTION VOXELIZATION](#第-54-页extinctionvoxelization)
- [第 55 页：EXTINCTION VOXELIZATION](#第-55-页extinctionvoxelization)
- [第 56 页：EXTINCTION WITH ATOMICS : OVERFLOW](#第-56-页extinctionwithatomicsoverflow)
- [第 57 页：EXTINCTION VOXELIZATION](#第-57-页extinctionvoxelization)
- [第 58 页：EXTINCTION WITH ATOMICS : OVERFLOW](#第-58-页extinctionwithatomicsoverflow)
- [第 59 页：EXTINCTION VOXELIZATION](#第-59-页extinctionvoxelization)
- [第 60 页：COLOR EXTINCTION WITH ATOMICS](#第-60-页colorextinctionwithatomics)
- [第 61 页：VBOIT : INTEGRATION](#第-61-页vboitintegration)
- [第 62 页：Slice 0](#第-62-页slice0)
- [第 63 页：VBOIT : ALIASING AT SPLAT](#第-63-页vboitaliasingatsplat)
- [第 64 页：VBOIT : ALIASING AT SPLAT](#第-64-页vboitaliasingatsplat)
- [第 65 页：VBOIT : ALIASING AT SPLAT](#第-65-页vboitaliasingatsplat)
- [第 66 页：Slice 0](#第-66-页slice0)
- [第 67 页：VBOIT : ALIASING AT SAMPLE](#第-67-页vboitaliasingatsample)
- [第 68 页：Slice 0](#第-68-页slice0)
- [第 69 页：VBOIT : ALIASING AT SAMPLE](#第-69-页vboitaliasingatsample)
- [第 70 页：Slice 0](#第-70-页slice0)
- [第 71 页：VBOIT : SCALAR SLICES](#第-71-页vboitscalarslices)
- [第 72 页：VBOIT : COLOR SLICES](#第-72-页vboitcolorslices)
- [第 73 页：VBOIT : INTEGRATION](#第-73-页vboitintegration)
- [第 74 页：VBOIT : INTEGRATION](#第-74-页vboitintegration)
- [第 75 页：VBOIT : TILED DEPTH PREPASS](#第-75-页vboittileddepthprepass)
- [第 76 页：VBOIT : TILED DEPTH PREPASS OPTIMIZATION](#第-76-页vboittileddepthprepassoptimization)
- [第 77 页：VBOIT : ZERO TRANSMITTANCE EARLY OUT](#第-77-页vboitzerotransmittanceearlyout)
- [第 78 页：VBOIT : ZERO TRANSMITTANCE EARLY OUT](#第-78-页vboitzerotransmittanceearlyout)
- [第 79 页：VBOIT VFX WITH ZERO TRANSMITTANCE EARLY OUT](#第-79-页vboitvfxwithzerotransmittanceearlyout)
- [第 80 页：ZERO TRANSMITTANCE DEPTH](#第-80-页zerotransmittancedepth)
- [第 81 页：ZERO TRANSMITTANCE DEPTH SLICING](#第-81-页zerotransmittancedepthslicing)
- [第 82 页：ZERO TRANSMITTANCE DEPTH SLICING](#第-82-页zerotransmittancedepthslicing)
- [第 83 页：ZERO TRANSMITTANCE DEPTH SLICING](#第-83-页zerotransmittancedepthslicing)
- [第 84 页：ZERO TRANSMITTANCE DEPTH SLICING](#第-84-页zerotransmittancedepthslicing)
- [第 85 页：ZERO TRANSMITTANCE EARLY OUT PERFORMANCE](#第-85-页zerotransmittanceearlyoutperformance)
- [第 86 页：VBOIT : FAIL CASES](#第-86-页vboitfailcases)
- [第 87 页：VBOIT : FAIL CASES : NO OIT](#第-87-页vboitfailcasesnooit)
- [第 88 页：VBOIT : FAIL CASES : OIT WITH EXACTLY 1 SLICE SEPARATION](#第-88-页vboitfailcasesoitwithexactly1sliceseparation)
- [第 89 页：VBOIT : FAIL CASES : OIT SLICE OVERLAP WITH POINT SPLAT](#第-89-页vboitfailcasesoitsliceoverlapwithpointsplat)
- [第 90 页：VBOIT : FAIL CASES](#第-90-页vboitfailcases)
- [第 91 页：VBOIT : FAIL CASES : OIT SLICE OVERLAP WITH POINT SPLAT](#第-91-页vboitfailcasesoitsliceoverlapwithpointsplat)
- [第 92 页：VBOIT : FAIL CASES : OIT SLICE OVERLAP WITH LINEAR SPLAT](#第-92-页vboitfailcasesoitsliceoverlapwithlinearsplat)
- [第 93 页：ADAPTIVE VOXEL](#第-93-页adaptivevoxel)
- [第 94 页：ADAPTIVE VBOIT](#第-94-页adaptivevboit)
- [第 95 页：ADAPTIVE VBOIT](#第-95-页adaptivevboit)
- [第 96 页：ADAPTIVE VBOIT](#第-96-页adaptivevboit)
- [第 97 页：ADAPTIVE VBOIT](#第-97-页adaptivevboit)
- [第 98 页：RESULTS](#第-98-页results)
- [第 99 页：TESTING SETUP](#第-99-页testingsetup)
- [第 100 页：RGB PERFORMANCE](#第-100-页rgbperformance)
- [第 101 页：NO OIT](#第-101-页nooit)
- [第 102 页：MBOIT](#第-102-页mboit)
- [第 103 页：WBOIT](#第-103-页wboit)
- [第 104 页：AVBOIT](#第-104-页avboit)
- [第 105 页：COVERAGE AT DISTANCE](#第-105-页coverageatdistance)
- [第 106 页：PERFORMANCE](#第-106-页performance)
- [第 107 页：RGB PRACTICAL PERFORMANCE : NO OIT](#第-107-页rgbpracticalperformancenooit)
- [第 108 页：RGB PRACTICAL PERFORMANCE : AVBOIT MONO](#第-108-页rgbpracticalperformanceavboitmono)
- [第 109 页：RGB PRACTICAL PERFORMANCE : AVBOIT TOTAL EXT ONLY](#第-109-页rgbpracticalperformanceavboittotalextonly)
- [第 110 页：RGB PRACTICAL PERFORMANCE : AVBOIT RGB](#第-110-页rgbpracticalperformanceavboitrgb)
- [第 111 页：AVBOIT : PRODUCTION IMPACT](#第-111-页avboitproductionimpact)
- [第 112 页：VFX EMITTERS /W MANUAL SORT : NO OIT](#第-112-页vfxemitterswmanualsortnooit)
- [第 113 页：VFX EMITTERS /W MANUAL SORT : OIT](#第-113-页vfxemitterswmanualsortoit)
- [第 114 页：VFX EMITTERS /W MANUAL SORT : NO OIT](#第-114-页vfxemitterswmanualsortnooit)
- [第 115 页：VFX EMITTERS /W MANUAL SORT : OIT](#第-115-页vfxemitterswmanualsortoit)
- [第 116 页：UI DOESN’T SORT CORRECTLY](#第-116-页uidoesntsortcorrectly)
- [第 117 页：MIN SPEC PERFORMANCE / GEN8](#第-117-页minspecperformancegen8)
- [第 118 页：CONCLUSION](#第-118-页conclusion)
- [第 119 页：AVBOIT : CONCLUSION](#第-119-页avboitconclusion)
- [第 120 页：EXTENSIONS : DISTORTION](#第-120-页extensionsdistortion)
- [第 121 页：EXTENSIONS : DISTORTION](#第-121-页extensionsdistortion)
- [第 122 页：AVBOIT : FUTURE WORK](#第-122-页avboitfuturework)
- [第 123 页：REFERENCES](#第-123-页references)
- [第 124 页：T HE PRE M IE R C ONF E RE NC E & E X HIBIT ION ON](#第-124-页thepremierconferenceexhibitionon)
- [第 125 页：research.activision.com T HE PRE M IE R C ONF E RE NC E & E X HIBIT ION ON](#第-125-页researchactivisioncomthepremierconferenceexhibitionon)
- [第 126 页：T HE PRE M IE R C ONF E RE NC E & E X HIBIT ION ON](#第-126-页thepremierconferenceexhibitionon)
- [第 127 页：VBOIT MONOCHROME](#第-127-页vboitmonochrome)
- [第 128 页：VBOIT COLOR - TRANS PASS EXTINCTION ONLY](#第-128-页vboitcolortranspassextinctiononly)
- [第 129 页：VBOIT COLOR - TRANS PASS EXTINCTION](#第-129-页vboitcolortranspassextinction)
- [第 130 页：VBOIT COLOR – RGB EXTINCTION](#第-130-页vboitcolorrgbextinction)
- [第 131 页：VBOIT : COLOR APPROXIMATION : FAIL CASE](#第-131-页vboitcolorapproximationfailcase)
- [第 132 页：VBOIT : COLOR](#第-132-页vboitcolor)
- [第 133 页：VBOIT : COLOR APPROXIMATION : FAIL CASE](#第-133-页vboitcolorapproximationfailcase)
- [第 134 页：VBOIT : COLOR APPROXIMATION : SELF TINT](#第-134-页vboitcolorapproximationselftint)
- [第 135 页：VBOIT : COLOR APPROXIMATION : SELF CORRECTION](#第-135-页vboitcolorapproximationselfcorrection)
- [第 136 页：A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5](#第-136-页advancesinrealtimerenderinginggamescoursesiggraph2025)

---

## 第 1 页：ADAPTIVE VOXEL-BASED

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 1 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
ADAPTIVE VOXEL-BASED
 ORDER INDEPENDENT
   TRANSPARENCY
     MICHAL DROBOT




                       1
```

## 第 2 页：INTRODUCTION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 2 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
INTRODUCTION




               2
```

## 第 3 页：THE OIT TEAM

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 3 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
THE OIT TEAM



        MICHAL DROBOT                        PIOTR BRENDEL                                        NICHOLAS JORDAN

        • Technology Fellow                  • Expert Rendering Engineer                          • Rendering Engineer
        • Activision Central Technology      • Infinity Ward Poland                               • Raven Software




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5   3




                                                                                                                                                                                   3
```

## 第 4 页：ACKNOWLEDGEMENTS

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 4 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 ACKNOWLEDGEMENTS



 ALL THE PEOPLE WHO HELPED

         • Engineering : Piotr Brendel, Nicholas Jordan, Brandon Whitley, Francois Durand, Felipe Gomez
         • Art & Tech Art : Patrick Hagar, Gavin Lerner, Rober Kowalchuk, Daniel Stern
         • Treyarch : Robert Moffat, Yale Miller, Kevin Myers
         • Central Technology : Natasha Tatarchuk, Michael Vance, Paul Edelstein
         • Production : David Guo, Neal Nikaido, Christopher Reimschussel




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                  A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5   4




Big shoutout to everyone who helped with this project!




                                                                                                                                                                                         4
```

## 第 5 页：Our games need technology to support vastly different scenarios and environments.

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 5 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Our games need technology to support vastly different scenarios and environments.
This includes large open worlds with huge viewing distances.




                                                                                    5
```

## 第 6 页：Tight, close quarter combat running at highest possible framerate.

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 6 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Tight, close quarter combat running at highest possible framerate.




                                                                     6
```

## 第 7 页：Crazy spectacular modes that are full of hundreds of NPCs with absolute visual

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 7 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Crazy spectacular modes that are full of hundreds of NPCs with absolute visual
mayhem on screen mixing explosions with magic.




                                                                                 7
```

## 第 8 页：As well as super high quality in-game cinematics that require highest precision and

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 8 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
As well as super high quality in-game cinematics that require highest precision and
quality.




                                                                                      8
```

## 第 9 页：TRANSPARENCY IN GAMES

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 9 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 TRANSPARENCY IN GAMES



 USUALLY REFERS TO                             RENDERING IN RIGHT ORDER

 • Glass
 • VFX meshes
                                                                                                        1
 • VFX cards                                                                                                   2
                                                                                                                      3




                                                                                                        2
                                                                                                               3
                                                                                                                      1



 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C               A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5   9




Rendering transparencies is non-trivial.
We usually deal with transparencies when rendering effects and glass in games.
Unfortunately drawing those surfaces in any order is not correct, and what is worse,
switching order every frame will result in flicker and artifacts as alpha blend is order-
dependent.




                                                                                                                                                                      9
```

## 第 10 页：PAINTERS ALGORITHM

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 10 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
PAINTERS ALGORITHM



REFERENCE SOLUTION TO N EVENTS

• Render each event in back to front order
• Requires solving composition equation for each pixel               𝐶𝑓 = 𝐶1 + 1 − 𝛼1 𝐶0 𝑓𝑜𝑟 𝑖 = 1

• In physical terms, this means solving a composition
     using the transmittance integral

                                             𝐶𝑓 = [𝐶𝑛 + 1 − 𝛼𝑗 ∙∙∙ 𝐶2 + 1 − 𝛼2 𝐶1 + 1 − 𝛼1 𝐶0 ∙∙∙]




                                              𝑛

• Transmittance Integral at event i          ෑ (1 − 𝛼𝑗 )
                                             𝑗=𝑖+1

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                 A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                   10
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
• Requires solving composition equation for each pixel               𝐶𝑓 = 𝐶1 + 1 − 𝛼1 𝐶0 𝑓𝑜𝑟 𝑖 = 1
                                             𝐶𝑓 = [𝐶𝑛 + 1 − 𝛼𝑗 ∙∙∙ 𝐶2 + 1 − 𝛼2 𝐶1 + 1 − 𝛼1 𝐶0 ∙∙∙]
• Transmittance Integral at event i          ෑ (1 − 𝛼𝑗 )
                                             𝑗=𝑖+1
```

## 第 11 页：SOLUTIONS :

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 11 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
SOLUTIONS :
BIG FIELD OF RESEARCH [VAS20] [WYM16]



EXACT SOLUTIONS                                            PROS                                  CONS

• Sort back to front on CPU                                • Exact                               • High bandwidth
     − For non overlapping geometry only                                                         • High complexity
• Capture all events and sort                                                                    • High memory usage
     − Linked Lists
                                                                                                 • Requires synchronization
     − Depth Peel
                                                                                                       primitives
     − Bounded A / K buffers
          •     Fallback to approximate on out of memory

     − Many more




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                             11
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
BIG FIELD OF RESEARCH [VAS20] [WYM16]
```

## 第 12 页：SOLUTIONS :

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 12 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
SOLUTIONS :
BIG FIELD OF RESEARCH [VAS20] [WYM16]



STOCHASTIC SOLUTIONS

• Rasterize with stochastic dither
     − Based on pixel transparency [ESSL10][WYM17]

• Super sample
     − Spatially [ESSL10]
     − Over time

• Converge to solution
• Often used in games

PROS                                         CONS

• Cheap                                      • Situational

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                     12
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
BIG FIELD OF RESEARCH [VAS20] [WYM16]
     − Based on pixel transparency [ESSL10][WYM17]
     − Spatially [ESSL10]
```

## 第 13 页：WEIGHTED BLENDING

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 13 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
WEIGHTED BLENDING



APPROXIMATE SOLUTION TO N EVENTS

• Rewrites the equation to weighted
                                                                                                                                                     𝑛
     components                                                𝑛                        1 − ς𝑛𝑖=1 1 − 𝛼𝑖
                                                        𝐶𝑓 = ෍     𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼𝑖 ) ∙                        + 𝐶0 ෑ 1 − 𝛼𝑖
                                                               𝑖=1                      σ𝑛𝑖=1 𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼𝑖
• Normalize sum                                                                                                                                    𝑖=1

• Converge to reference                                                   Event
                                                                                         Normalization Factor                                        Total Transmittance Integral
                                                                          Weight
     − Weight -> Integrated Transmittance up to Event


                                                          Event
                                                          Weight
                                                                       Approximates ς𝑛𝑗=𝑖+1(1 − 𝛼𝑗 )




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                               A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                 13
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
     components                                                𝑛                        1 − ς𝑛𝑖=1 1 − 𝛼𝑖
                                                        𝐶𝑓 = ෍     𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼𝑖 ) ∙                        + 𝐶0 ෑ 1 − 𝛼𝑖
                                                               𝑖=1                      σ𝑛𝑖=1 𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼𝑖
• Normalize sum                                                                                                                                    𝑖=1
     − Weight -> Integrated Transmittance up to Event
                                                                       Approximates ς𝑛𝑗=𝑖+1(1 − 𝛼𝑗 )
```

## 第 14 页：WEIGHTED BLENDING :

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 14 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
WEIGHTED BLENDING :
TRANSPARENCY RENDERING SETUP
                                                                                                    𝑛
                                                   𝑛                        1 − ς𝑛𝑖=1 1 − 𝛼𝑖
                                 𝐶𝑓 = ෍                𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼𝑖 ) ∙                        + 𝐶0 ෑ 1 − 𝛼𝑖
                                                                            σ𝑛𝑖=1 𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼𝑖




                                                                                                                                                    MRT0
                                                   𝑖=1
                                                                                                   𝑖=1


                              Move from alpha to extinction matching volumetric rendering notation
                                              𝑛                             𝑛

                                             ෑ 1 − 𝛼𝑖 = exp(− ෍ −log(1 − 𝛼𝑖 ))
                                             𝑖=1                            𝑖=1




                                                                                                                                                    MRT1
                             Draw all transparencies with following outputs

                                              RT                 Function                   Format                Op
                                  MRT0 RGB                     𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼 𝑖 )       11R11G10B Float           Add




                                                                                                                                                    MRT2
                                MRT1 R(GB)                    𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼 𝑖          11R11G10B Float           Add
                                MRT2 R(GB)                    −log(1 − 𝛼𝑖 )            11R11G10B Float           Add


© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                       A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                                         14
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                   𝑛                        1 − ς𝑛𝑖=1 1 − 𝛼𝑖
                                 𝐶𝑓 = ෍                𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼𝑖 ) ∙                        + 𝐶0 ෑ 1 − 𝛼𝑖
                                                                            σ𝑛𝑖=1 𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼𝑖
                                             ෑ 1 − 𝛼𝑖 = exp(− ෍ −log(1 − 𝛼𝑖 ))
                                             𝑖=1                            𝑖=1
                                  MRT0 RGB                     𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼 𝑖 )       11R11G10B Float           Add
                                MRT1 R(GB)                    𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼 𝑖          11R11G10B Float           Add
                                MRT2 R(GB)                    −log(1 − 𝛼𝑖 )            11R11G10B Float           Add
```

## 第 15 页：WEIGHTED BLENDING :

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 15 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
WEIGHTED BLENDING :
TRANSPARENCY RESOLVE SETUP
                                                                                                𝑛
                                              𝑛                        1 − ς𝑛𝑖=1 1 − 𝛼𝑖
                             𝐶𝑓 = ෍               𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼𝑖 ) ∙                        + 𝐶0 ෑ 1 − 𝛼𝑖
                                              𝑖=1                      σ𝑛𝑖=1 𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼𝑖
                                                                                                𝑖=1
                                 𝑛                               𝑛

                              ෑ 1 − 𝛼𝑖 = exp(− ෍ −log(1 − 𝛼𝑖 ))
                               𝑖=1                              𝑖=1

                             Full screen resolve pass




                                                                                                                                                                              Resolve
                                               Inputs                        Function                 Op
                                             Background                           𝐶0             Replace
                                             MRT0 RGB                       𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼 𝑖 )         Add
                                             MRT1 R(GB)                    𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼 𝑖            Add
                                             MRT2 R(GB)                    −log(1 − 𝛼𝑖 )              Add

                                                    1 − exp(−𝑀𝑅𝑇2)
                             𝐶𝑓 = 𝑀𝑅𝑇0 ∙                           + 𝐵𝑎𝑐𝑘𝑔𝑟𝑜𝑢𝑛𝑑 ∙ exp(−𝑀𝑅𝑇2)
                                                         𝑀𝑅𝑇1

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                  A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                                    15
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                              𝑛                        1 − ς𝑛𝑖=1 1 − 𝛼𝑖
                             𝐶𝑓 = ෍               𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼𝑖 ) ∙                        + 𝐶0 ෑ 1 − 𝛼𝑖
                                              𝑖=1                      σ𝑛𝑖=1 𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼𝑖
                              ෑ 1 − 𝛼𝑖 = exp(− ෍ −log(1 − 𝛼𝑖 ))
                               𝑖=1                              𝑖=1
                                             MRT0 RGB                       𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼 𝑖 )         Add
                                             MRT1 R(GB)                    𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼 𝑖            Add
                                             MRT2 R(GB)                    −log(1 − 𝛼𝑖 )              Add
                                                    1 − exp(−𝑀𝑅𝑇2)
                             𝐶𝑓 = 𝑀𝑅𝑇0 ∙                           + 𝐵𝑎𝑐𝑘𝑔𝑟𝑜𝑢𝑛𝑑 ∙ exp(−𝑀𝑅𝑇2)
```

## 第 16 页：SOLUTIONS :

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 16 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 SOLUTIONS :
 BIG FIELD OF RESEARCH [VAS20] [WYM16]



 APPROXIMATE BLEND SOLUTION                   PROS                                           CONS

 • Change “over blend” operator with          • Low complexity                               • Frequency soup
      order independent operator              • Low bandwidth                                • Hard to tune
      − Weighted Sum [Mes07]
                                              • Low memory usage                             • Hard to represent
      − Weighted Average [BM08]
                                                                                                   occlusion
      − Weighted Blended OIT [MB13]
           •     Most common in games

           •     Improvements in [KIR18]

 • Use ad hoc arbitrary weights
      − Depth / luma based




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                      A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Approximate blend solution often suffer from a lot of ambiguous tuning values,
making it hard to replicate results across content.




                                                                                                                                                                         16
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
 BIG FIELD OF RESEARCH [VAS20] [WYM16]
      − Weighted Sum [Mes07]
      − Weighted Average [BM08]
      − Weighted Blended OIT [MB13]
           •     Improvements in [KIR18]
```

## 第 17 页：APPROXIMATE TRANSMITTANCE INTEGRAL

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 17 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
APPROXIMATE TRANSMITTANCE INTEGRAL



CONSTRUCT FUNCTION

• Approximate transmittance over depth
                                                                Event
                                                                Weight
                                                                               Approximates ς𝑛𝑗=𝑖+1(1 − 𝛼𝑗 )
     − Exponential
     − Statistical
                                                                         0.5             0.75                                0.5
     − DCT
                                                                 1.0
                                                1.0
     − Piecewise

• Project all transmission events to generate
     function coefficients                                                         0.5
                                                Transmittance




     − Render all transparencies in prepass
     − Can be lower resolution                                                                              0.125
                                                                                                                                        0.0625

                                                0.0                                                                                             Depth



© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                 A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                   17
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                                               Approximates ς𝑛𝑗=𝑖+1(1 − 𝛼𝑗 )
```

## 第 18 页：PROJECT TRANSMITTANCE ON FUNCTION BASIS

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 18 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
PROJECT TRANSMITTANCE ON FUNCTION BASIS


                                                                             𝑛
                         𝑛                           1 − ς𝑛𝑖=1 1 − 𝛼𝑖
      𝐶𝑓 = ෍                    𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼𝑖 ) ∙                        + 𝐶0 ෑ 1 − 𝛼𝑖
                         𝑖=1                         σ𝑛𝑖=1 𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼𝑖
                                                                            𝑖=1

      Projection                                                                   Reconstruction
      Accumulate event transmittance projections on function basis                 Evaluate function at Z with accumulated coefficients

                                                                𝑇 𝑧 𝑏𝑎𝑠𝑖𝑠
        𝑃𝑟𝑜𝑗𝑒𝑐𝑡 (𝑧0 , 𝛼0)                                            𝑐0
                                              +                      𝑐1
        𝑃𝑟𝑜𝑗𝑒𝑐𝑡 (𝑧1 , 𝛼1 )                                                         𝑤 (𝑧𝑖 , 𝛼𝑖 ) = 𝑇(𝑧, 𝑐0 , 𝑐1 , … , 𝑐𝑛 )
              ...                                                    𝑐2
        𝑃𝑟𝑜𝑗𝑒𝑐𝑡 (𝑧0 , 𝛼0)                                           ...           Approximates ς𝑛𝑗=𝑖+1(1 − 𝛼𝑗 )
                                                                    𝑐𝑛

        Coefficient stored pixel
        Can be lower resolution



© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                                  18
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                         𝑛                           1 − ς𝑛𝑖=1 1 − 𝛼𝑖
      𝐶𝑓 = ෍                    𝐶𝑖 ∙ 𝑤(𝑧𝑖 , 𝛼𝑖 ) ∙                        + 𝐶0 ෑ 1 − 𝛼𝑖
                         𝑖=1                         σ𝑛𝑖=1 𝛼𝑖 ∙ 𝑤 𝑧𝑖 , 𝛼𝑖
        𝑃𝑟𝑜𝑗𝑒𝑐𝑡 (𝑧0 , 𝛼0)                                            𝑐0
        𝑃𝑟𝑜𝑗𝑒𝑐𝑡 (𝑧1 , 𝛼1 )                                                         𝑤 (𝑧𝑖 , 𝛼𝑖 ) = 𝑇(𝑧, 𝑐0 , 𝑐1 , … , 𝑐𝑛 )
        𝑃𝑟𝑜𝑗𝑒𝑐𝑡 (𝑧0 , 𝛼0)                                           ...           Approximates ς𝑛𝑗=𝑖+1(1 − 𝛼𝑗 )
```

## 第 19 页：TRANSMITTANCE INTEGRAL WEIGHTED OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 19 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
TRANSMITTANCE INTEGRAL WEIGHTED OIT


                                                                 Moments
                                              Render trans




                                                                                                                                           C1
                                                                                 C0
TRANSMITTANCE INTEGRAL                        draws project      Wavelets
                                                extinction
• Split composing into 2 passes              (low resolution)     Voxels
     − Transmittance integral calculation




                                                                                                                                           Cn
                                                                                 C2
     − Trans rendering with pre-integrated
       transmittance in any order             Render trans
• Approximate transmittance                        draws
                                             (full resolution)
  integral
     − Transmittance Function Mapping
       [DEL11]
     − Moment Based OIT [PMWK17]
     − Wavelet Based OIT [ASM21]
     − Voxel Based OIT                                                      Resolve



© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                    A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                      19
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
• Split composing into 2 passes              (low resolution)     Voxels
                                             (full resolution)
       [DEL11]
     − Moment Based OIT [PMWK17]
     − Wavelet Based OIT [ASM21]
```

## 第 20 页：TRANSMITTANCE INTEGRAL WEIGHTED OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 20 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
TRANSMITTANCE INTEGRAL WEIGHTED OIT



 PROS                                            CONS

 • Medium complexity                             • Approximate
 • Low to Medium bandwidth                       • Different tradeoffs
 • Low to Medium memory usage                      • smoothness and correctness over depth

 • Allows splitting event rendering resolution   • Occlusion can be tricky
   − from transmittance resolution




                                                                    A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                            20
```

## 第 21 页：OIT JOURNEY

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 21 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
OIT JOURNEY
      IN
CALL OF DUTY




               21
```

## 第 22 页：CALL OF DUTY TRANSPARENCY RENDERING

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 22 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
CALL OF DUTY TRANSPARENCY RENDERING



IW & CURRENT TECHNOLOGY                      PROS

• Manual sort orders                         • Fast
     − Emitters
                                             • No memory overhead
     − Materials
                                               − 8-bit accumulated alpha buffer for culling of flares
     − Meshes

• Sort on CPU
• Allow inter-emitter sorting
     − CPU
     − GPU

• Support Blend Add RGB mode
     − Hardware dual blend




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                               22
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                               − 8-bit accumulated alpha buffer for culling of flares
```

## 第 23 页：MANUAL / CPU SORT FAIL CASES

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 23 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
MANUAL / CPU SORT FAIL CASES




                                                                         CONS

                                                                         • Labor intensive
                                                                         • Sometimes doesn’t work

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     23
```

## 第 24 页：CALL OF DUTY TRANSPARENCY RENDERING

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 24 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
CALL OF DUTY TRANSPARENCY RENDERING



TA / BLACK OPS 4 [KOH16]                                       Layer Data           Monochrome                                             RGB
                                                               9 x Color            11R11G10B Float                                        11R11G10BF
• Exact sorting
                                                               9 x Transmittance    8UNORM                                                 11R11G10BF
     − 9-layer A buffer
                                                               9 x Depth            16F                                                    16F
          •     Meshes raster with UAVs

          •     Blend into one layer on fallback
                                                               1 x Sync Counter     8UINT |4x in U32                                       8UNT |4x in U32
     − VFX rasterizes in software CS resolve                   Sum bits per pixel   512                                                    728
          •     sort and resolve with A Buffer                 Memory @4k           ~530MB                                                 ~754MB
          •     Generate Adaptive Transparency OIT structure
                for other media composition [SAL11]            Bandwidth per pixel Write                                                   Read
                                                               per event
• No support for explicit sort orders
                                                               Monochrome           88bits                                                 88-512
                                                               RGB                  112bits                                                112-728


© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                          A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                            24
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
TA / BLACK OPS 4 [KOH16]                                       Layer Data           Monochrome                                             RGB
                for other media composition [SAL11]            Bandwidth per pixel Write                                                   Read
• No support for explicit sort orders
```

## 第 25 页：CALL OF DUTY TRANSPARENCY RENDERING

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 25 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
CALL OF DUTY TRANSPARENCY RENDERING



TA / BLACK OPS 4 [KOH16]

• Pros
     − Shipped
     − Overflows not apparent <w/ that content>

• Cons
     • Heavy performance hit >2.5x vs non-OIT (current)
     • SW raster path requires a lot of maintenance
     • More modern VFX use less camera-oriented cards
          •     Forcing A-Buffer overflows more often
          •     Higher cost

     • No simple support for multi-resolution
     • 4K / RGB – memory / performance prohibitive



© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                  25
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
TA / BLACK OPS 4 [KOH16]
     • Heavy performance hit >2.5x vs non-OIT (current)
     • No simple support for multi-resolution
```

## 第 26 页：NEW SOLUTION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 26 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
NEW SOLUTION



IMPORTANT                                            WHAT OTHER GAMES USE

• Occlusion                                          • Weight Blended OIT
     − Smoke grenades and visibility is an           • Moment-Based OIT
          important gameplay feature
                                                     • Bound A-Buffer
• Polychromic Transparency
     − Call of Duty games use plenty colored glass

• Robustness in long distance views
• Memory & Performance




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                26
```

## 第 27 页：MOMENT-BASED OIT [PMWK17][SHA18]

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 27 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
MOMENT-BASED OIT [PMWK17][SHA18]


  𝑇(𝑧)           Event
                 Weight
                                 Approximates ς𝑛𝑗=𝑖+1(1 − 𝛼𝑗 )                     Prepass renders P channel MRT:
                                                                                   𝑧1 ∙ 𝑤
         With power moments function
                                                                                   𝑧 2…∙ 𝑤
         Or trigonometric moments
                                                                                    𝑧𝑝 ∙ 𝑤
                           0.5                     0.75           0.5              Sampling reconstructs transmittance integral:
                 1.0                                                                                     𝑏
 1.0                                                                                𝑇 𝑧 = exp(−𝐻𝑎𝑚𝑏𝑢𝑟𝑔𝑒𝑟( ′ , 𝑧) ∙ 𝑑′ )
                                                                                        𝑛
                                                                                                         𝑑
                                                                                    𝑏 = ෍(𝑧𝑖 , 𝑧𝑖2 , … , 𝑧𝑖𝑝 ) ∙ 𝑤𝑖
                                             0.5                                            𝑖=1
 Transmittance




                                                                                   • Potential issues
                                                          0.125
                                                                                     − Hard to represent step function
                                                                        0.0625
                                                                                        •   Thin opaque features smoothed out
 0.0                                                                       Depth
                                                                                     − All projections change all coefficients
                                                                                        •   Not depth complexity invariant
© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                        A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                                          27
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
MOMENT-BASED OIT [PMWK17][SHA18]
  𝑇(𝑧)           Event
                                 Approximates ς𝑛𝑗=𝑖+1(1 − 𝛼𝑗 )                     Prepass renders P channel MRT:
 1.0                                                                                𝑇 𝑧 = exp(−𝐻𝑎𝑚𝑏𝑢𝑟𝑔𝑒𝑟( ′ , 𝑧) ∙ 𝑑′ )
                                                                                    𝑏 = ෍(𝑧𝑖 , 𝑧𝑖2 , … , 𝑧𝑖𝑝 ) ∙ 𝑤𝑖
                                             0.5                                            𝑖=1
```

## 第 28 页：WAVELET-BASED OIT [ASM21]

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 28 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
WAVELET-BASED OIT [ASM21]


  𝑇(𝑧)           Event
                                 Approximates ς𝑛𝑗=𝑖+1(1 − 𝛼𝑗 )                     Prepass renders (n+1) channels to 2𝑛 channel MRT:
                 Weight

         With Haar Wavelets
         or other wavelet functions

                           0.5                     0.75           0.5


 1.0
                 1.0                                                               Sampling reconstructs transmittance integral:



                                             0.5
 Transmittance




                                                                                   • Potential issues
                                                          0.125
                                                                                     − Thin opaque feature projection
                                                                        0.0625
                                                                                       •   Ringing
 0.0                                                                       Depth
                                                                                       •   Translation variance
                                                                                           −   Not shift invariant

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                                             28
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
WAVELET-BASED OIT [ASM21]
  𝑇(𝑧)           Event
                                 Approximates ς𝑛𝑗=𝑖+1(1 − 𝛼𝑗 )                     Prepass renders (n+1) channels to 2𝑛 channel MRT:
```

## 第 29 页：NO OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 29 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 NO OIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




First let’s compare no OIT – including an obvious fail case where we sort my particle
emitter, yet actual sprites interleave resulting in visual artifacts.




                                                                                                                                                                29
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
First let’s compare no OIT – including an obvious fail case where we sort my particle
```

## 第 30 页：MBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 30 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 MBOIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




MBOIT correctly resolves order dependencies resulting in stable good looking image.




                                                                                                                                                              30
```

## 第 31 页：WBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 31 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 WBOIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Similar with WBOIT.
So far so good.
Let’s try something more complex closer to in-game scenario.




                                                                                                                                                              31
```

## 第 32 页：NO OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 32 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 NO OIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Here we can observe no OIT – standard ordered version that exhibits some minor
issues due to mixing z-feathered effects and distortion effects with imperfect back to
front sorting (this is rather unusual setup for in-game).
One character silhouette shows a bit through smoke grenade (which is far from ideal
because the main goal of smoke grenade is to obscure what is behind it).




                                                                                                                                                                32
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
front sorting (this is rather unusual setup for in-game).
```

## 第 33 页：MBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 33 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 MBOIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C            A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Switching to MBOIT generates even worse result when it comes to silhouettes.
Suddenly we can see yet another character that was previously hiding in smoke and
this time it is a perfectly legitimate opaque geometry character that should be 100%
occluded.
Furthermore, we can also easily observe up sampling artifacts from MBOIT (as the
function coefficients don’t behave well under linear filter at depth discontinuities
which result in function discontinuities).




                                                                                                                                                               33
```

## 第 34 页：WBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 34 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 WBOIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C            A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




WBOIT exhibits same issues as MBOIT – with visibly better silhouette resolution
preservation that is mostly due to better wavelet behavior under linear magnification
(which stems from implicit depth partitioning by wavelet).




                                                                                                                                                               34
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
(which stems from implicit depth partitioning by wavelet).
```

## 第 35 页：MBOIT NOT DEPTH COMPLEXITY INVARIANT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 35 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 MBOIT NOT DEPTH COMPLEXITY INVARIANT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C               A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Let’s investigate this odd behavior.
First, this is out resting harness. Center left of screen you can see a magenta crosshair
that is pointing at opaque background right next to top left corner of front most card.
Bottom left of screen, black rectangle draws the transmittance function, using chosen
reconstruction, at crosshair.
Currently not drawing anything because we are pointing at opaque background.

There are 3 cards drawn as transparent VFX with 0% transmittance. They are draws as
opaque objects would, but with OIT, where we would expect 100% occlusion (as in
smoke grenade case).




                                                                                                                                                                  35
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
smoke grenade case).
```

## 第 36 页：MBOIT NOT DEPTH COMPLEXITY INVARIANT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 36 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 MBOIT NOT DEPTH COMPLEXITY INVARIANT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Moving right, we hit first single surface, overlapping the background. Function is
steep enough (although ideally this would be a vertical line considering we are
working with infinitely thin sprites / events).




                                                                                                                                                                 36
```

## 第 37 页：MBOIT NOT DEPTH COMPLEXITY INVARIANT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 37 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 MBOIT NOT DEPTH COMPLEXITY INVARIANT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Moving further right, we hit a pixel that has two transparent sprites overlapping. At
this point we start seeing the problem where the previously steep line is starting to
skew losing its slope with further smoothing around toe and shoulder.
It also becomes offset in depth but not enough to cause trouble.




                                                                                                                                                                 37
```

## 第 38 页：MBOIT NOT DEPTH COMPLEXITY INVARIANT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 38 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 MBOIT NOT DEPTH COMPLEXITY INVARIANT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C            A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Moving pointer further right we see that pixel overlapping 3 opaque cards shows very
significant transmittance function skew which is just too much for rather small depth
difference between cards – which would explain why there is some blue piecing
through from 2nd sprite.




                                                                                                                                                               38
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
significant transmittance function skew which is just too much for rather small depth
```

## 第 39 页：WBOIT BETTER COMPLEXITY INVARIANCE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 39 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 WBOIT BETTER COMPLEXITY INVARIANCE




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Switching to WBOIT we can immediately see that blue is not piercing through any
card.
Actually things look pretty good. However there is some odd sloping visible on
reconstructed transmittance graph.




                                                                                                                                                              39
```

## 第 40 页：WBOIT BETTER COMPLEXITY INVARIANCE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 40 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 WBOIT BETTER COMPLEXITY INVARIANCE




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Moving further we see no change to transmittance curve. So far so good.




                                                                                                                                                              40
```

## 第 41 页：WBOIT BETTER COMPLEXITY INVARIANCE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 41 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 WBOIT BETTER COMPLEXITY INVARIANCE




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C            A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




And same with final event.
This can be easily explained by thinking about wavelets as implicit separation over
depth. Each wavelet frequency hits certain ‘depth ranges’ – and in this case we seem
to be lucky enough to have good separation.
But let's see what happens if we start moving between those ranges.




                                                                                                                                                               41
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
depth. Each wavelet frequency hits certain ‘depth ranges’ – and in this case we seem
But let's see what happens if we start moving between those ranges.
```

## 第 42 页：WBOIT NOT SHIFT INVARIANT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 42 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 WBOIT NOT SHIFT INVARIANT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Moving the camera back and pointing at same overlap type (of 3 opaques) shows
that reconstructed transmittance significantly shifted. There is not much slope
change, but some smoothing at toe and shoulder is visible.
Also colors start mixing between sprites.




                                                                                                                                                              42
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
Moving the camera back and pointing at same overlap type (of 3 opaques) shows
```

## 第 43 页：WBOIT NOT SHIFT INVARIANT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 43 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 WBOIT NOT SHIFT INVARIANT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Moving the camera slightly in results in dramatic slope and offset shift – that in
motion looks very much like ringing. Meaning we just hit a depth region between
wavelet frequencies. This unfortunately results in significant visual oscillation of
occlusion between sprites.




                                                                                                                                                                 43
```

## 第 44 页：WBOIT NOT SHIFT INVARIANT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 44 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 WBOIT NOT SHIFT INVARIANT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C            A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




And then once we are past problematic depth range (frequency peaks) it all goes back
to normal.
In the end this is an unreliable behavior.
There are other methods available to reduce ringing in frequency domain, but all of
them trade signal smoothness and clipping for ringing prevention – thus with existing
experience we decided not to pursue.




                                                                                                                                                               44
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
And then once we are past problematic depth range (frequency peaks) it all goes back
them trade signal smoothness and clipping for ringing prevention – thus with existing
```

## 第 45 页：NEED NEW SOLUTION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 45 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 NEED NEW SOLUTION

 • Complexity invariant
 • Shift invariant




                MBOIT                         WBOIT   NO OIT                                                                             VBOIT


 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                    A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




And finally, comparison of problematic are of smoke grenade across all methods.
Last panel present our new proposed method using Voxels.




                                                                                                                                                                       45
```

## 第 46 页：VOXEL-BASED

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 46 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                                 VOXEL-BASED
                                                     OIT




Before we get to final form of our OIT solution – Adaptive Voxel-Based OIT – we
would like to present basic method version as building blocks necessary for fully
featured solution.




                                                                                    46
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
would like to present basic method version as building blocks necessary for fully
```

## 第 47 页：VOXEL-BASED OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 47 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VOXEL-BASED OIT



TRANSMITTANCE IN VOLUMETRIC RENDERING
                                                       1.0
• Solves same problem by voxelizing air density into




                                                       Transmittance
  froxels




                                                                                0.75




                                                                                                                             0.75
• Compute job integrates transmittance over eye rays




                                                                                                           0.5
• Generates a 3D LUT of transmittance integral from    0.0                                                              Voxels along camera Z
  eye to point in question
• Render transparencies
     − sample 3D LUT for transmittance integral        1.0             1.0
     − linear or higher order sampling                                                  0.75
                                                       Transmittance
                                                                                                            0.375
                                                                                                                                   0.281
• Similar framework used in volumetric fog
  [WRO14][DRO17]                                       0.0                                                              Voxels along camera Z

• Deep Shadow Maps [LOK00][NGU05][SIN08][BAV13]

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                     47
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
     − sample 3D LUT for transmittance integral        1.0             1.0
  [WRO14][DRO17]                                       0.0                                                              Voxels along camera Z
• Deep Shadow Maps [LOK00][NGU05][SIN08][BAV13]
```

## 第 48 页：VBOIT : BASIC STEPS

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 48 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : BASIC STEPS



ALGORITHM

• Fit curve distribution for slices over depth
• Clear volumetric extinction buffer
• Splat transparency into volumetric extinction buffer at lower resolution
• Sum extinction along view rays into 3D extinction integral texture
• Draw opaque geometry
• Render transparency in any order
     − Lookup extinction integral at point of surface
     − Multiply surface opacity by transmittance integral

• Resolve opaque background with normalized transparency accumulated color and accumulated extinction




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                     48
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
• Fit curve distribution for slices over depth
```

## 第 49 页：VBOIT : DEPTH DISTRIBUTION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 49 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : DEPTH DISTRIBUTION



 LOG CURVE

 • Adjustable
      − Far plane
      − Linearization factor
      − Slice count

 • Implicit near plane




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




For depth distribution of slices of froxel buffer we use a parametric logarithmic curve
that allows us to change precision at near plane depending on how many slices we
have at our disposal.




                                                                                                                                                                 49
```

## 第 50 页：VBOIT : DEPTH DISTRIBUTION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 50 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : DEPTH DISTRIBUTION



                                             128 Slices                                         1024 Slices


• Adjust slice count together with linearization factor
     − Keeps spatial slice resolution close to camera ~constant
          •     Minimal expected precision in the scene

     − Dynamically cover longer ranges sacrificing precision far away




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                50
```

## 第 51 页：VOXELIZATION WITH RASTERIZATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 51 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VOXELIZATION WITH RASTERIZATION



VFX & MESHES
                                                              1. Clear slices to 1.0
• Accumulate transmittance via splatting                      2. VS assign VFX quad to slice
• [BAV13] suggests using SV_RenderTargetArrayIndex            3. PS writes (1.0 – alpha) with MUL BLEND
     − Support on HW might require GS path
     − Fast path needs                                                      0.5                                0.5

          VPAndRTArrayIndexFromAnyShaderFeedingRasterizer
     − Only works on per-primitive basis
                                                            1.0
     − Can only voxelize camera facing quads

• Can we use atomics instead?




                                                                                               0.25
                                                            0.0                                                                        Voxels along camera Z




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                               51
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
• [BAV13] suggests using SV_RenderTargetArrayIndex            3. PS writes (1.0 – alpha) with MUL BLEND
```

## 第 52 页：VOXELIZATION WITH ATOMICS

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 52 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VOXELIZATION WITH ATOMICS



VFX & MESHES
                                                float extinction = saturate(-log(1.0 - alpha) /
• Atomics mesh voxelization [DRO17.2]            -log(1.0 / ((float)VBT_EXTINCTION_BIT_MASK)));
     − Splat from PS                            uint depthSlice = depth * (float)VbtGetVoxelSliceCnt();
                                                uint uintExtinction = ((float)VBT_EXTINCTION_BIT_MASK) * extinction + 0.5;
     − Fast if contention is minimized          InterlockedAdd( vbt[uint3( coord, depthSlice) ], uintExtinction);

     − There is no InterlockedAtomicMultiply

• Move from transmittance to extinction
     − Log space
     − InterlockedAtomicAdd

• Work with UINT
     − Rescale extinction to normalized range
     − Scale by bit depth




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                     52
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                float extinction = saturate(-log(1.0 - alpha) /
• Atomics mesh voxelization [DRO17.2]            -log(1.0 / ((float)VBT_EXTINCTION_BIT_MASK)));
     − Splat from PS                            uint depthSlice = depth * (float)VbtGetVoxelSliceCnt();
                                                uint uintExtinction = ((float)VBT_EXTINCTION_BIT_MASK) * extinction + 0.5;
     − Fast if contention is minimized          InterlockedAdd( vbt[uint3( coord, depthSlice) ], uintExtinction);
```

## 第 53 页：EXTINCTION VOXELIZATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 53 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
EXTINCTION VOXELIZATION
U32 ATOMICS

                                              Slice 1                                                                                    Slice 2




                                             Resolve    Splat Type                                                Time (ms)
                                                        MRT MUL                                                            0.75
                                                        U32                                                                0.91




                                                        Rendering 17mil pixels overdraw @ 1080p PS5
© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                  53
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                             Resolve    Splat Type                                                Time (ms)
```

## 第 54 页：EXTINCTION VOXELIZATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 54 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
EXTINCTION VOXELIZATION
PACKED 8BIT ATOMICS


PACKING INTO 8 BITS                                   DWORD     Word3     Word2        Word1              Word0

                                                                 64        128          192                   32
• U32 for extinction accumulation is overkill           +         0         0           128                    0
     − Memory and performance
                                                        =                  129          64                    32

• Pack slices into 8bit words of U32                  Correct    64        128          255                   32




• 2 problems
                                                1.0                              1.0                                                   1.0
     − Overflows
     − Carry over between words                                            +                                                     !=

                                                0.0                   Z          0.0                                Z                  0.0                                        Z
                                                      W3 W2 W1 W0



© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                               54
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
• U32 for extinction accumulation is overkill           +         0         0           128                    0
                                                        =                  129          64                    32
     − Carry over between words                                            +                                                     !=
```

## 第 55 页：EXTINCTION VOXELIZATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 55 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 EXTINCTION VOXELIZATION
 PACKED 8BIT ATOMICS
                                       Overflows    Slice 1                                                                                    Slice 2




                                       Overflows   Resolve    Splat Type                                                Time (ms)
                                                              MRT MUL                                                            0.75
                                                              U32                                                                0.91
                                                              U8                                                                 0.89



                                                              Rendering 17mil pixels overdraw @ 1080p PS5
 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                     A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Overflows result in unwanted function change – that will show as artifacts in final
image.




                                                                                                                                                                                        55
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                       Overflows   Resolve    Splat Type                                                Time (ms)
```

## 第 56 页：EXTINCTION WITH ATOMICS : OVERFLOW

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 56 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
EXTINCTION WITH ATOMICS : OVERFLOW



PACKING INTO 8 BITS
                                             uint uintExtinctionPacked = pack( extinction, wordIdx );
• Using                                      uint current = 0;
                                             uint expected = 0;
     InterlockedAtomicCompareExchange        uint newData = uintExtinctionPacked;
                                             while ( true )
     − Can implement any operation           {
                                               AtomicCmpExchange(vbt[uint3( coord, sliceIdx)], expected, newData, current);
     − Equivalent to spinlock                  if ( expected == current )
                                                 break;
• Reference version                            newData = pack( saturate( unpack( current, wordIdx) + extinction ), wordIdx );
                                               expected = current;
• 2x-10x more expensive                      }




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                  A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                    56
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                             uint uintExtinctionPacked = pack( extinction, wordIdx );
• Using                                      uint current = 0;
                                             uint expected = 0;
     InterlockedAtomicCompareExchange        uint newData = uintExtinctionPacked;
                                             while ( true )
     − Can implement any operation           {
                                               AtomicCmpExchange(vbt[uint3( coord, sliceIdx)], expected, newData, current);
     − Equivalent to spinlock                  if ( expected == current )
                                                 break;
• Reference version                            newData = pack( saturate( unpack( current, wordIdx) + extinction ), wordIdx );
                                               expected = current;
• 2x-10x more expensive                      }
```

## 第 57 页：EXTINCTION VOXELIZATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 57 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
EXTINCTION VOXELIZATION
PACKED 8BIT ATOMICS /W SPINLOCK


                                              Slice 1                                                                                    Slice 2




                                             Resolve    Splat Type                                                Time (ms)
                                                        MRT MUL                                                            0.75
                                                        U32                                                                0.91
                                                        U8                                                                 0.89
                                                        U8 Spinlock Overflow                                               1.95
                                                        Rendering 17mil overdraw pixels @ 1080p PS5


© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                  57
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                             Resolve    Splat Type                                                Time (ms)
```

## 第 58 页：EXTINCTION WITH ATOMICS : OVERFLOW

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 58 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
EXTINCTION WITH ATOMICS : OVERFLOW


                                                   1.0                                        1.0                                                      1.0

OVERFLOW DETECTION
                                                                                         +
                                                                                                                                                                                                      Carry Over
• Order words along integration direction
                                                   0.0                         Z              0.0                                   Z                  0.0                                        Z
• Integrating along ray
                                                         W0 W1 W2 W3                                                                                                                                  Mark
     − Data past overflow does not matter                                                                                                                                                      Overflow Z2
     − Don’t care about carry over
                                                   1.0                                                                                                1.0
• Detect overflow                                                                                        Integrate
     − On overflow store depth                                                                         Stop at Z2
          •     Min over ray with InterlockedMin
                                                                                                         saturate
                                                   0.0                         Z                                                                      0.0                                        Z
• During integration
     − Read overflow min depth                     InterlockedAdd( vbt[uint3( coord, sliceIdx ) ], uintExtinction, preData );
                                                   bool isOverflow = ( unpack( prevData, wordIdx ) + extinction ) > 1.0;
     − Saturate                                    if ( isOverflow )
                                                   {
     − Stop Integrate                                InterlockedMin( vbtOverflowDepth [uint3( coords, 0 )], sliceIdx );
                                                   }

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                               A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                                   58
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
     − Read overflow min depth                     InterlockedAdd( vbt[uint3( coord, sliceIdx ) ], uintExtinction, preData );
                                                   bool isOverflow = ( unpack( prevData, wordIdx ) + extinction ) > 1.0;
     − Saturate                                    if ( isOverflow )
     − Stop Integrate                                InterlockedMin( vbtOverflowDepth [uint3( coords, 0 )], sliceIdx );
```

## 第 59 页：EXTINCTION VOXELIZATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 59 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 EXTINCTION VOXELIZATION
 PACKED 8BIT ATOMICS /W OVERFLOW MIN DEPTH DETECTION

                                                                                                                             Overflow Min Depth
                                              Slice 1




                                              Slice 2




                                                        Resolve   Splat Type                                                Time (ms)
                                                                  MRT MUL                                                            0.75
                                                                  U32                                                                0.91
                                                                  U8                                                                 0.89
                                                                  U8 Spinlock Overflow                                               1.95
                                                                  U8 Overflow Min                                                    0.91
                                                                  Rendering 17mil pixels overdraw @ 1080p PS5
 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                         A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Even though overflows are visible in individual slices, it does not matter for final
integration, as we can saturate the results using overflow mind depth buffer data.




                                                                                                                                                                                            59
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                        Resolve   Splat Type                                                Time (ms)
Even though overflows are visible in individual slices, it does not matter for final
```

## 第 60 页：COLOR EXTINCTION WITH ATOMICS

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 60 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
COLOR EXTINCTION WITH ATOMICS



SPLATTING COLOR

• Pack RGB into 32 bits as 8|(2)|8|(2)|8|(2)                                                       0xFF         00                0xFF                    00                0xFF                 0000

     − Update with single atomic                                                                   RED    overflow              GREEN               overflow                BLUE              overflow

     − Same execution speed as packed 8bit monochrome
     − Some loss due to worse caching
          •     Less slice sharing -> Less atomic contention                                      Splat Type                                                             Time (ms)
          •     First Overflow method does not protect against carry over to next ‘wavelength’
          •     Pack 8bit values in 10bit fields
                                                                                                  MRT 8UNORM MUL                                                                  0.75
                −    Allows 2 bits for overflows and carry over                                   MRT 10R10G10B MUL                                                               0.81
          •     Carry over only touches lowest bits of next ‘wavelength’
                −    Not visible in practice unless all your VFX have heavy color skew
                                                                                                  U8 Overflow Min                                                                 0.91
     − First Overflow stores first slice for each R G B                                           10|10|10 Overflow Min                                                           0.95
          •     Faster with 3x InterlockedMin then spinlock updating single U32
                                                                                                 Rendering 17mil pixels overdraw @ 1080p PS5




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                 A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                                   60
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
• Pack RGB into 32 bits as 8|(2)|8|(2)|8|(2)                                                       0xFF         00                0xFF                    00                0xFF                 0000
          •     Less slice sharing -> Less atomic contention                                      Splat Type                                                             Time (ms)
                −    Allows 2 bits for overflows and carry over                                   MRT 10R10G10B MUL                                                               0.81
     − First Overflow stores first slice for each R G B                                           10|10|10 Overflow Min                                                           0.95
```

## 第 61 页：VBOIT : INTEGRATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 61 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : INTEGRATION



INTEGRATION CS JOB
                                                     1.0
• Generate Integral 3D LUT




                                                     Transmittance
     − Iterate over all slices




                                                                              0.75




                                                                                                                           0.75
                                                                                                         0.5
     − Add extinction
     − Stop ray when extinction integral saturates   0.0                                                              Voxels along camera Z

          •     Or overflow detected


                                                     1.0             1.0
                                                                                      0.75
                                                     Transmittance
                                                                                                          0.375
                                                                                                                                 0.281

                                                     0.0                                                              Voxels along camera Z




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                 A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                   61
```

## 第 62 页：Slice 0

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 62 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                              Slice 0




                                              Slice 1




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                     A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




First try – works – but clear aliasing artifacts are visible.




                                                                                                                                                                        62
```

## 第 63 页：VBOIT : ALIASING AT SPLAT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 63 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : ALIASING AT SPLAT



ALIASING AT SOURCE                                                           ~0.0                                ~0.0                                ~0.0


• During splatting we discretize into slice resolution   1.0




                                                         Transmittance
• Changes over splat depth create aliasing


                                                         0.0                                                               Voxels along camera Z



                                                                         ~0.0       ~0.0                                                             ~0.0

                                                         1.0




                                                         Transmittance

                                                         0.0                                                               Voxels along camera Z

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                 A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                   63
```

## 第 64 页：VBOIT : ALIASING AT SPLAT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 64 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
  VBOIT : ALIASING AT SPLAT



  LINEAR SPLATTING
                                                                                                                                     ~0.0          ~0.0                                                             ~0.0

  • Use slice fraction to split partially between slices                                                     1.0

  • Monochrome




                                                                                                             Transmittance
       − Merge splats to slice Z and Z + 1
       − Issue extra splat on straddling DWORDs
                                                                                                             0.0                                                                          Voxels along camera Z


       InterlockedAdd                          InterlockedAdd          2 x InterlockedAdd
                                                                                                                                         ~0.0       ~0.0                                                             ~0.0
W0       W1          W2          W3             W0     W1       W2     W3        W0     W1     W2     W3
0xFF    0xFF        0xFF        0xFF            0xFF   0xFF     0xFF   0xFF      0xFF   0xFF   0xFF   0xFF      1.0




                                                                                                                  Transmittance
                                                                                                                                  0.65                        0.65
                                                                                                                  0.0                                                                       Voxels along camera Z
  © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                                              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




During linear splatting, if objects have actual thickness – it would be preferable to
calculate exact volume of event intersecting a slice and only splat that amount of
extinction. This can significantly improve visual dimensionality of vfx – however that
specific improvement is orthogonal to discussed OIT algorithm.




                                                                                                                                                                                                                                                  64
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
During linear splatting, if objects have actual thickness – it would be preferable to
```

## 第 65 页：VBOIT : ALIASING AT SPLAT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 65 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : ALIASING AT SPLAT



LINEAR SPLATTING
                                              Splat Method                                                                            Time (ms)
• RGB
                                              Monochrome Point Splat                                                                          0.75
     − Issue 2 splats
                                              Monochrome Linear Splat                                                                         0.81
                                              RGB Point Splat                                                                                 0.91
• No performance delta                        RGB Linear Splat                                                                                0.95
     −     Splats are within cache lines     Rendering 17mil pixels overdraw @ 1080p PS5
     − They are independent




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                       A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                         65
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                              Splat Method                                                                            Time (ms)
```

## 第 66 页：Slice 0

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 66 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                             Slice 0




                                             Slice 1




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                               66
```

## 第 67 页：VBOIT : ALIASING AT SAMPLE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 67 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : ALIASING AT SAMPLE


                                                                                                           ~0.0         ~0.0
ALIASING AT SAMPLING
                                                      1.0

• Extinction can be linearly interpolated




                                                      Transmittance
• Use HW linear
                                                                                              0.65                                0.65
• Sampling at splat point != value during splatting   0.0                                                                   Voxels along camera Z


                                                                                                           0.16

• Chance for self occlusion?                          1.0




                                                      Integral
                                                                                              0.65
                                                      0.0                                                                   Voxels along camera Z




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                            A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                              67
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
• Sampling at splat point != value during splatting   0.0                                                                   Voxels along camera Z
• Chance for self occlusion?                          1.0
```

## 第 68 页：Slice 0

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 68 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                              Slice 0




                                              Slice 1




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C               A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




As we can see glass pane is starting intersect itself and self occlude due to
imprecision of depth slicing. It is further exacerbated by lower resolution of integral
buffer.




                                                                                                                                                                  68
```

## 第 69 页：VBOIT : ALIASING AT SAMPLE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 69 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : ALIASING AT SAMPLE


                                                                                                                    ~0.0         ~0.0
 ALIASING AT SAMPLING
                                                               1.0

 • Use HW linear




                                                               Transmittance
      − Requires 2 slices bias
                                                                                                       0.65                                0.65
 • Linear interpolation over a linear splat
                                                               0.0                                                                   Voxels along camera Z
      − Not C1 continuous
                                                                                 1.0             ~0.7                0.0
      − Requires 2nd order sampling
           •     Bicubic works well but needs 2.5 slice bias   1.0




                                                               Integral
                                                                                                       0.65
                                                               0.0                                                                   Voxels along camera Z
                                                                                              2 slice bias




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                    A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




We need to make sure that sampling of extinction is offset in depth enough to avoid
self occlusion.




                                                                                                                                                                                       69
```

## 第 70 页：Slice 0

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 70 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                             Slice 0




                                                       • Linear Interpolation
                                             Slice 1
                                                         − -2.0 bias to avoid self occlusion

                                                       • Bicubic Interpolation
                                                         − -2.5 bias to avoid self occlusion

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                  70
```

## 第 71 页：VBOIT : SCALAR SLICES

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 71 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : SCALAR SLICES

                          1                   5        9




                          11                  12       13




                          44                  66       88




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C            A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Before moving further let’s look at some extinction slice captures from an example
map.
Here I picked a random selection of slices, ordered in ascending order from camera.
Those slices represent any monochromatic event that splat.




                                                                                                                                                               71
```

## 第 72 页：VBOIT : COLOR SLICES

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 72 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : COLOR SLICES

                          1                   3       4




                          5                   6       12




                          13                  14      15




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Similar here but this time around for RGB events.
As RGB buffers are split from scalar – you can see some overlapping at same depth
but not visible together.
What is immediately clear is that occupancy of those buffers is quite sparse




                                                                                                                                                              72
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
Similar here but this time around for RGB events.
```

## 第 73 页：VBOIT : INTEGRATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 73 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : INTEGRATION

                          1                   3          4




                          5                   6          11




                          23                  66         88




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




During extinction integration from both scalar and RGB sources we can still observe
somewhat sparse nature of the buffer.
I specifically picked more interesting ranges here, but there is nothing happening in 0-
3 as well as in 20-40 range etc
Also, empty space doesn’t impact our extinction integral in any way (only costing us
more).
We can also see some areas that saturate to full extinction earlier than end of buffer
or even opaque scene behind it.
Maybe we can exploit this.




                                                                                                                                                                 73
```

## 第 74 页：VBOIT : INTEGRATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 74 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : INTEGRATION



INTEGRATION CS JOB                                  1.0             1.0
                                                                                  0.75




                                                    Transmittance
• Observations
                                                                                                      0.375
     − Reading empty slices is redundant                                                                                     0.281
          •     Can we skip them?
                                                    0.0                                                            Voxels along camera Z
     − Extinction integral often saturates early
                                                                                                                            14
          •     Before reaching opaque scene
                                                                                                                     10
          •     Before reaching last event                                                   4          7
                                                                                                                   9                                    9
          •     Can we early out other rendering?

                                                                                                                                                    10
                                                                                         Scene
                                                                                          Depth




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                  74
```

## 第 75 页：VBOIT : TILED DEPTH PREPASS

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 75 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : TILED DEPTH PREPASS



SOFTWARE RASTER BOUNDS

• Low resolution occupancy bit buffer
     − XY 1/8th resolution of Extinction resolution
     − Z 1 bit for each slice
          •     Stored in U32

• CS job
     − Gather all bounding boxes and quads
     − Conservatively Rasterize / Voxelize
     − Separate for Scalar and RGB extinction
          •     Most VFX use Scalar
          •     Most Meshes use RGB




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C            A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                              75
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
     − Z 1 bit for each slice
     − Separate for Scalar and RGB extinction
```

## 第 76 页：VBOIT : TILED DEPTH PREPASS OPTIMIZATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 76 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : TILED DEPTH PREPASS OPTIMIZATION

                                                     1440p Target : RGB OIT

SKIP EMPTY SPACES IN CLEAR AND INTEGRATION           Scalar Extinction Buffer             : 320x180x32 UINT32 (packed 8|8|8|8)

                                                     RGB Extinction Buffer                : 320x180x128 UINT32 (packed 10|10|10)
• Empty space is not needed by any pass              Integrated Extinction Buffer : 320x180x128 RGBA8
     − Nothing writes to it                          Total : 66.5MB
     − Nothing reads it
                                                      Pass                                                     Full                                    Occupied
          •     Integration can skip
                                                      Clear                                                  0.153                                           0.01
• Clear & Integrate CS jobs
                                                      Integrate                                                 0.2                                          0.04
     − Read occupancy bit buffer per wavefront
          •     Skip empty
                                                      Total                                                  0.353                                           0.05

• Can load balance for optimal performance
     − Not necessary at our resolutions

• Clear / Integration scale with spatial occupancy
• Good async compute candidates
• Use for resolve pass



© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                            A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                              76
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
SKIP EMPTY SPACES IN CLEAR AND INTEGRATION           Scalar Extinction Buffer             : 320x180x32 UINT32 (packed 8|8|8|8)
                                                     RGB Extinction Buffer                : 320x180x128 UINT32 (packed 10|10|10)
• Can load balance for optimal performance
• Use for resolve pass
```

## 第 77 页：VBOIT : ZERO TRANSMITTANCE EARLY OUT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 77 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : ZERO TRANSMITTANCE EARLY OUT



SKIP DRAWING FULLY EXTINCT PIXELS

• Once pixel transmittance goes to zero
     − We can stop drawing whatever is behind
          •     Transparencies

          •     Opaque geometry

• Extinction Integration job                                                          14
                                                                               10
     − Stops when extinction integral along ray
                                                          4      7
          reaches assumed zero transmittance                                 9                                    9
     − Write out slice index at which zero was reached
                                                                                                              10
          •     zeroTransmittanceDepth                   Slice
                                                         Depth




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                             77
```

## 第 78 页：VBOIT : ZERO TRANSMITTANCE EARLY OUT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 78 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : ZERO TRANSMITTANCE EARLY OUT

                                                                     Transmittance                           Zero transmittance depth

EARLY DEPTH PIPELINE

• CS Job Generates quad list
     − For each screen tile
          •     Conservatively read zeroTransmittanceDepth texture

          •     Generate quad covering screen tile at depth                                                                                CS Quad
                                                                                  DrawIndirect
                                                                                                                                            Spawn
• Populate depth buffer
                                                                     Quad draws                        Depth buffer
     − Render all quads with DrawIndirect
     − Much faster than fullscreen SV_DEPTH
 Pass                                        Quads   SV_DEPTH
 Spawn                                       0.004     N/A
 Depth Draw                                  0.01      0.2
 Total                                       0.014     0.2
4k depth buffer from 480x270 zeroTransmittance | PS5

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                       A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                  Tile size at least 16x16
                           Guarantees compressed depth
                           HiZ benefits
                  Much faster than doing a single quad with SV_Depth output




                                                                                                                                                                                                         78
```

## 第 79 页：VBOIT VFX WITH ZERO TRANSMITTANCE EARLY OUT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 79 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT VFX WITH ZERO TRANSMITTANCE EARLY OUT




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     79
```

## 第 80 页：ZERO TRANSMITTANCE DEPTH

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 80 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
ZERO TRANSMITTANCE DEPTH




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     80
```

## 第 81 页：ZERO TRANSMITTANCE DEPTH SLICING

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 81 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 ZERO TRANSMITTANCE DEPTH SLICING




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Slicing through the accumulated integral we can see how our zero-transmittance
depth effectively culls out draws in 3D




                                                                                                                                                              81
```

## 第 82 页：ZERO TRANSMITTANCE DEPTH SLICING

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 82 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
ZERO TRANSMITTANCE DEPTH SLICING




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     82
```

## 第 83 页：ZERO TRANSMITTANCE DEPTH SLICING

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 83 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
ZERO TRANSMITTANCE DEPTH SLICING




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     83
```

## 第 84 页：ZERO TRANSMITTANCE DEPTH SLICING

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 84 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
ZERO TRANSMITTANCE DEPTH SLICING




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     84
```

## 第 85 页：ZERO TRANSMITTANCE EARLY OUT PERFORMANCE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 85 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 ZERO TRANSMITTANCE EARLY OUT PERFORMANCE


                                               Pass                              VBOIT(ms)    VBOIT + ZeroTrans (ms)          Alpha Blend (ms)
                                               Spawn                                N/A               0.004                                   N/A
                                               Depth Draw                           N/A                0.01                                   N/A
                                               Transparency Raster                 2.035               1.78                                  1.73
                                               Total                                1.98              1.742                                  1.73
                                               Trans Pixels Drawn                21,276,176         18,922,855                        21,276,176
                                               Opaque Pixels Drawn               2,759,432          1,867,980                          2,759,432
                                              VRS 4k | RGB VBOIT 480x270 | PS5




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                        A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Effectively leaving no trace of visual artifacts (as those should not be possible if we
guarantee that past that point nothing renders).
Using zero-transmittance depth comes as a significant optimization to all transparent
draws.
Furthermore, if your engine order allows rendering transparencies first, you might
use zero-transmittance depth to further cull any opaque draws.
For sake of this presentation and presented performance numbers we are NOT using
zero-transmittance depth to cull opaque geometry.




                                                                                                                                                                                                                           85
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                               Pass                              VBOIT(ms)    VBOIT + ZeroTrans (ms)          Alpha Blend (ms)
Effectively leaving no trace of visual artifacts (as those should not be possible if we
Furthermore, if your engine order allows rendering transparencies first, you might
```

## 第 86 页：VBOIT : FAIL CASES

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 86 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : FAIL CASES

                                              Point Splatting                                          Point Sampling (-1 bias)



                                0.0                0.0                 0.0                   1.0              0.0                               0.0

    1.0                                                                          1.0
     Transmittance




                                                                                 Integral
     0.0                                                 Voxels along camera Z   0.0                                 Voxels along camera Z



                          0.0                                          0.0                  1.0 ~1.0                                            0.0




                                                                                                                                                                                                                            Add & normalize
                                      0.0                                                                                                                                       1.0                    1.0

    1.0                                                                          1.0
     Transmittance




                                                                                 Integral




     0.0                                                 Voxels along camera Z   0.0                                 Voxels along camera Z

 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                         A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




If transmittance events overlap inside same slice
Surfaces will get over / under occluded
VBOIT inside single slice degenerates to Weight Blending with linear splatting implicit
weight.




                                                                                                                                                                                                                                              86
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                              Point Splatting                                          Point Sampling (-1 bias)
```

## 第 87 页：VBOIT : FAIL CASES : NO OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 87 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : FAIL CASES : NO OIT




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     87
```

## 第 88 页：VBOIT : FAIL CASES : OIT WITH EXACTLY 1 SLICE SEPARATION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 88 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : FAIL CASES : OIT WITH EXACTLY 1 SLICE SEPARATION




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




When there is enough depth separation resulting in at least a single slice separation,
rendering is artifact free.




                                                                                                                                                                88
```

## 第 89 页：VBOIT : FAIL CASES : OIT SLICE OVERLAP WITH POINT SPLAT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 89 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : FAIL CASES : OIT SLICE OVERLAP WITH POINT SPLAT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




However, when events overlap same slice, we are starting to deal with frequency
soup and thus a mix of all events landing over same depth slice.




                                                                                                                                                              89
```

## 第 90 页：VBOIT : FAIL CASES

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 90 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : FAIL CASES


                                              Linear Splatting                                       Linear Sampling (-2.0 bias)




                                                                                                                                                                                                                   Add & normalize
                          0.0   ~0.0                               0.0                  1.0   ~0.7                                     0.0                              1.0                   ~0.7

  1.0                                                                        1.0
   Transmittance




                                                                             Integral
                   0.65                0.65
                            0.0
   0.0                                               Voxels along camera Z   0.0                            Voxels along camera Z




 • Linear Splatting & Sampling improves Color mix via spatial information




                                                                                                           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




       Linear splat interpolation helps with over / under occlusion separation but
       extends self occlusion range
For precise results it is crucial to improve depth separation – thus our goal should be
to provide enough slices where it matters




                                                                                                                                                                                                                                     90
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                              Linear Splatting                                       Linear Sampling (-2.0 bias)
```

## 第 91 页：VBOIT : FAIL CASES : OIT SLICE OVERLAP WITH POINT SPLAT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 91 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : FAIL CASES : OIT SLICE OVERLAP WITH POINT SPLAT




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     91
```

## 第 92 页：VBOIT : FAIL CASES : OIT SLICE OVERLAP WITH LINEAR SPLAT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 92 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : FAIL CASES : OIT SLICE OVERLAP WITH LINEAR SPLAT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C               A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Linear splatting alone can help making the artifacts less visible as it does provide
some depth separation via weighting.
That said for precise results it is crucial to improve depth separation – thus our goal
should be to provide enough slices where it matters




                                                                                                                                                                  92
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
That said for precise results it is crucial to improve depth separation – thus our goal
```

## 第 93 页：ADAPTIVE VOXEL

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 93 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                             ADAPTIVE VOXEL
                                               BASED OIT




To address slice precision we would like to introduce extension to VBOIT called
Adaptive VBOIT




                                                                                  93
```

## 第 94 页：ADAPTIVE VBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 94 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 ADAPTIVE VBOIT



 SPARSE USAGE OVER DEPTH

 • Already have coverage information prior
 • Skip empty spaces
 • VBOIT is invariant (between slices)
      − Shift
      − Data

 • Can create compressed depth curve to pack slices




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C            A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




We already know that empty space can be skipped. Furthermore, due to sparse
buffer population necessary for clear / integration optimizations, we already know
where empty space is.




                                                                                                                                                               94
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
 • VBOIT is invariant (between slices)
buffer population necessary for clear / integration optimizations, we already know
```

## 第 95 页：ADAPTIVE VBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 95 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 ADAPTIVE VBOIT



 DEPTH WARP LUT

 • Z binning for depth slices [DRO17.2]
 • Before splatting
      − Calculate prefix sum of Z occupancy
      − Compact
      − Mark range begin / end as ‘filterable’ (*)
           •     Disable filtering in empty spaces
           •     Snap filtering to begin / end points
                 −    Recalculate fractional position

      − LUT needs to be filterable

 • Sample 1D Depth Warp LUT in                                      0   1   2           3            4            5           6            7            8            9          10
      − Splatting                                       Depth LUT   0   0   0           0           0*         *1*           *1           1*          *2* *3*                    *4
      − Sampling




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                     A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




A simple scheme that can trivially skip empty space in coherent manner would be to
warp depth over certain ranges.
Working only in single dimension (as opposed to going fully sparse in XY and Z)
significantly simplifies all the steps necessary to construct this data structure as well
as to sample it.
Using prefix sum of Z occupancy of slices, we generate a 1D Depth Warp LUT that on
splat and read – for a given depth – returns a slice skipping all empty space.
For linear interpolation it also needs to reserve extra bits marking if this warped
depth is beginning, or end or middle of a non-empty region – so linear sampling can
be adjusted in order to simulate a continuous unchanged space (by snapping to end
of previous slice).
This scheme provides an attractive alternative to partially skipping space in clear and
integration passes – while alone doesn’t yet improve visual precision.
It does however provide an effective packing scheme.




                                                                                                                                                                                        95
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
 • Z binning for depth slices [DRO17.2]
      − Mark range begin / end as ‘filterable’ (*)
Working only in single dimension (as opposed to going fully sparse in XY and Z)
splat and read – for a given depth – returns a slice skipping all empty space.
For linear interpolation it also needs to reserve extra bits marking if this warped
integration passes – while alone doesn’t yet improve visual precision.
```

## 第 96 页：ADAPTIVE VBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 96 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 ADAPTIVE VBOIT



 REDISTRIBUTE RESOLUTION

 • Calculate 1D Z coverage at virtual slice resolution
      − Sum occupied slices
      − If does not fit physical slice resolution
           •     Lower virtual slice resolution

           •     Re-adjust log depth curve

 • Fits best resolution into physical scratch

                                                                             0   1   2   3   4   5       6     7     8     9     1     1     1      1     1     1     1     1     1      1     2     2       2
                                                            Virtual Slices                                                       0     1     2      3     4     5     6     7     8      9     0     1       2

                                                                             0   0   0   0   0   0       0     0     0     0     0     *     *      1     1     1     1     *     *      *     *     *       *
                                                    Virtual Res Depth LUT                                                        *     1     1                        *     2     3      4     5     6       6
                                                                                                                                       *                                    *     *      *     *     *



                                                         Physical Slices


 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                          A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Our depth distribution curve is logarithmic and parametrized in such a way that we
can improve precision over visible range by consuming more slices (visually flattening
the curve).
Before even splatting we specify the requested minimal slice thickness over visible
depth range, set slice count to large number of slices that would allow that slice
thickness and generate occupancy buffer with that parametrization.
This will most likely result in oversubscription of slice count.
We generate slice occupancy at this ‘high slice resolution’ and check if count of
occupied slices would fit our expected physical budget.
If not we reparametrize the curve again by halving slice count and repeat packing
process.
Fwiw coverage data doesn’t need to be regenerated, just conservatively rewritten
into ‘lower resolution’.
The process is repeated until we can fit all slices in memory – which is bound by
parametrization of our physical slice buffer.

Once process is completed, we can splat, integrate and sample a parametrization that
can potentially have a higher count of ‘virtual slices’ than physical slices, using our 1D
Depth Warp LUT indirection.




                                                                                                                                                                                                                 96
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
 • Calculate 1D Z coverage at virtual slice resolution
           •     Lower virtual slice resolution
We generate slice occupancy at this ‘high slice resolution’ and check if count of
can potentially have a higher count of ‘virtual slices’ than physical slices, using our 1D
```

## 第 97 页：ADAPTIVE VBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 97 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
ADAPTIVE VBOIT




                                                 VBOIT
• Depth LUT
     − ~0 performance cost at sample
     − ~0 performance cost to generate

• Integration cost scales with slice occupancy
     − Tune physical memory pool
     − Performance
                                                 AVBOIT


     − Precision
     − Memory consumption




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                  97
```

## 第 98 页：RESULTS

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 98 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
RESULTS




          98
```

## 第 99 页：TESTING SETUP

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 99 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
TESTING SETUP



SHARED FRAMEWORK                                             Method                      Transmittance Function Memory 1/8th 4k
                                                                        Write(bpp)         Read(bpp)                  Sampling                          Size (mb)                     Mem Access
• All methods                                                MBOIT    [1 + (2𝑟 )] (10)        10                         2 x 2D                                1.3                           dense
     − Same Resolve & Transparency Rendering Pass
                                                             WBOIT    [1 + 𝑟 + 1] (8)          8                         3 x 2D                                  2                           dense
     − Transmittance Prepass at 1/8th resolution
     − Same depth bounds                                     AVBOIT          1                 1                         1 x 3D                               16.8                          sparse
• Prepass Output
     − MBOIT (rank 2) : 5xFP16 (x3)
                                                             Method                      Transmittance Function Memory 1/8th 4k
     − WBOIT (rank 2) : 8xFP16 (x3)
                                                                        Write(bpp)         Read(bpp)                  Sampling                          Size (mb)                     Mem Access
     − AVBOIT : 8bit x 128 slices (+ 32bit x 128 )
          •     8k Depth Warp LUT
                                                             MBOIT          30                30                         4 x 2D                                  4                           dense

• Transparency Output (RGB)                                  WBOIT          24                24                         3 x 2D                                  6                           dense
     − Accumulated Color : 11R11G10B_Float
                                                             AVBOIT         1-4               1-4                        1 x 3D                               66.3                          sparse
     − Accumulated Extinction : 16F(11R11G10B_Float)
     − Accumulated Norm Denominator : 16F(11R11G10B_Float)



© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                         A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                           99
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                                        Write(bpp)         Read(bpp)                  Sampling                          Size (mb)                     Mem Access
• All methods                                                MBOIT    [1 + (2𝑟 )] (10)        10                         2 x 2D                                1.3                           dense
                                                             WBOIT    [1 + 𝑟 + 1] (8)          8                         3 x 2D                                  2                           dense
     − MBOIT (rank 2) : 5xFP16 (x3)
     − WBOIT (rank 2) : 8xFP16 (x3)
                                                                        Write(bpp)         Read(bpp)                  Sampling                          Size (mb)                     Mem Access
     − AVBOIT : 8bit x 128 slices (+ 32bit x 128 )
• Transparency Output (RGB)                                  WBOIT          24                24                         3 x 2D                                  6                           dense
     − Accumulated Extinction : 16F(11R11G10B_Float)
     − Accumulated Norm Denominator : 16F(11R11G10B_Float)
```

## 第 100 页：RGB PERFORMANCE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 100 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 RGB PERFORMANCE



 SPLIT CHROMATIC SOURCES                                         Pass                  RGB Extra Cost (x)
                                                                             MBOIT                     WBOIT                              AVBOIT
 • AVBOIT                                                        Clear        3x                             3x                                  3x
      − Split chromatic transmittance sources                   Prepass       1.9x                         1.7x                              1.04x
           •     Pay extra prepass cost only for RGB sources   Integration    N/A                          N/A                                   3x

      − Combine in transmittance integration                     Draws        1.4x                         1.3x                              1.05x
                                                                Resolve       1.1x                         1.1x                                1.1x
 • MBOIT/WBOIT
                                                               Avg Cost      >1.5x                       >1.3x                              ~1.05x
      − Not obvious how to split
      − Not obvious how to combine for sampling




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                          A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




In case of AVBOIT, we store RGB extinction buffers separate at extra memory cost.
This however allows us to significantly cut down on bandwidth during clears, splatting
and integration – as those expensive operations only need to happen on slices used
by RGB surfaces.
Integration reads monochromatic extinction and RGB extinction – merging them
together into RGB integral (at which point both extinction types buffers can be
thrown away).
In general, in our content – it is way more likely to have a monochromatic event
coming from particles.
Obviously, this choice is very project and content dependent and, in a case, where vfx
use RGB transmittance – it becomes more efficient to just keep one RGB
representation around.




                                                                                                                                                                                             100
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
 SPLIT CHROMATIC SOURCES                                         Pass                  RGB Extra Cost (x)
           •     Pay extra prepass cost only for RGB sources   Integration    N/A                          N/A                                   3x
      − Not obvious how to combine for sampling
In case of AVBOIT, we store RGB extinction buffers separate at extra memory cost.
Obviously, this choice is very project and content dependent and, in a case, where vfx
```

## 第 101 页：NO OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 101 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
NO OIT




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     101
```

## 第 102 页：MBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 102 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 MBOIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Transparencies show through.




                                                                                                                                                      102
```

## 第 103 页：WBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 103 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 WBOIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Transparencies show through.




                                                                                                                                                      103
```

## 第 104 页：AVBOIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 104 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 AVBOIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Note: puff of smoke in the middle is in original asset. AVBOIT makes it stand our due
to improved depth separation.




                                                                                                                                                                104
```

## 第 105 页：COVERAGE AT DISTANCE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 105 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 COVERAGE AT DISTANCE
 150M (LONG DISTANCE ENGAGEMENT)


                  No OIT                      MBOIT   WBOIT                                                                     AVBOIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




No OIT exhibits correct occlusion of car in the background. Unfortunately, it doesn’t
correctly resolve glass panes on the building on right side.
MBOIT and WBOIT resolve glass and smoke to a degree but fail to occlude glass in the
background.
AVBOIT resolves bot problematic events correctly.




                                                                                                                                                                      105
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
 150M (LONG DISTANCE ENGAGEMENT)
```

## 第 106 页：PERFORMANCE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 106 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 PERFORMANCE




                                                  Pass                       Method (ms)
                                                             MBOIT   WBOIT    AVBOIT       AVBOIT+Cull                   No OIT
                                                  Clear      0.001   0.001     0.013          0.013                          N/A
                                                 Prepass     0.196   0.199     0.204          0.204                          N/A
                                                   Cull       N/A     N/A       N/A           0.006                          N/A
                                               Integration    N/A     N/A      0.025          0.025                          N/A
                                                  Draws       5.4     5.6       5.2            4.5                            4.4
                                                 Resolve     0.108   0.108     0.108          0.108                          N/A
                                                   Total     5.705   5.908     5.667          4.967                           4.4
                                              4k | PS5




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                     A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




For performance considerations we are assuming that MBOIT / WBOIT is running on a
platform configured for maximum bandwidth throughput (i.e. accelerated clears,
accelerated ‘zero’ render target output etc.)
MBOIT / WBOIT clears should be HW accelerated on all platforms and PC. ‘zero’
output might vary.
AVBOIT shows as a compelling alternative to all other OIT methods – getting close
into no OIT territory.




                                                                                                                                                                                                        106
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                  Pass                       Method (ms)
platform configured for maximum bandwidth throughput (i.e. accelerated clears,
```

## 第 107 页：RGB PRACTICAL PERFORMANCE : NO OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 107 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
RGB PRACTICAL PERFORMANCE : NO OIT




                                                                   Total Cost (ms)
                                              AVBOIT            AVBOIT            AVBOIT RGB                      No OIT
                                             Monochrome   Total Extinction Only   Transmittance
                                                                                                                       2.8
© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                                        4k | PS5
                                                                                                  A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                          107
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                                   Total Cost (ms)
```

## 第 108 页：RGB PRACTICAL PERFORMANCE : AVBOIT MONO

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 108 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
RGB PRACTICAL PERFORMANCE : AVBOIT MONO




                                                                   Total Cost (ms)
                                              AVBOIT            AVBOIT            AVBOIT RGB                      No OIT
                                             Monochrome   Total Extinction Only   Transmittance
                                                2.95                                                                   2.8
© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                                        4k | PS5
                                                                                                  A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                          108
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                                   Total Cost (ms)
```

## 第 109 页：RGB PRACTICAL PERFORMANCE : AVBOIT TOTAL EXT ONLY

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 109 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
RGB PRACTICAL PERFORMANCE : AVBOIT TOTAL EXT ONLY




                                                                   Total Cost (ms)
                                              AVBOIT            AVBOIT            AVBOIT RGB                      No OIT
                                             Monochrome   Total Extinction Only   Transmittance
                                                2.95              3.02                                                 2.8
© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                                        4k | PS5
                                                                                                  A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                          109
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                                   Total Cost (ms)
```

## 第 110 页：RGB PRACTICAL PERFORMANCE : AVBOIT RGB

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 110 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
RGB PRACTICAL PERFORMANCE : AVBOIT RGB




                                                                   Total Cost (ms)
                                              AVBOIT            AVBOIT            AVBOIT RGB                      No OIT
                                             Monochrome   Total Extinction Only   Transmittance
                                                2.95              3.02                3.11                             2.8
© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                                        4k | PS5
                                                                                                  A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                          110
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                                   Total Cost (ms)
```

## 第 111 页：AVBOIT : PRODUCTION IMPACT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 111 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 AVBOIT : PRODUCTION IMPACT



 LESS WORK & LESS BUGS

 • Environmental art team
      − No impact, no bugs, problems fixed
      − No need for manual sorting
           •     No custom sort order materials

           •     No thinking about ordering during design

 • VFX team
      − Many problems fixed
      − New recurring “bug”
           •     “VFX emitters don’t sort according to manual sort”

           •     “UI doesn’t sort correctly”




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Environments are usually well behaved and do not abuse physical logic of sorting.
Same can’t be said about VFX where the final effect is often achieved by smoke and
mirrors.




                                                                                                                                                                              111
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
      − No need for manual sorting
```

## 第 112 页：VFX EMITTERS /W MANUAL SORT : NO OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 112 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VFX EMITTERS /W MANUAL SORT : NO OIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C          A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Some of our VFX had emitters with manual sort order – that was used to exaggerate
certain elements. In this case fireball showing strongly through smoke.




                                                                                                                                                             112
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
certain elements. In this case fireball showing strongly through smoke.
```

## 第 113 页：VFX EMITTERS /W MANUAL SORT : OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 113 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VFX EMITTERS /W MANUAL SORT : OIT




                                                                 • Sort in depth
                                                                       − Manually
                                                                       − ‘camera offset’

                                                                 • Forces content to make more
                                                                       volumetric assets
                                                                       − Better for OIT & shadows




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Resolving this scene with OIT results in physically correct image – which unfortunately
was not the artists intention.
Recovering this look in exact way is impossible, but we can potentially get closer to it
by emulating sorting with placement of effects over depth.
Artists already have access to ‘camera offset’ that moves VFX depth along camera ray.
This can be used to place effects at certain ranges or even go as far as presetting fixed
amount of ‘biases’ that can be used for layering.




                                                                                                                                                                 113
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                                       − Better for OIT & shadows
amount of ‘biases’ that can be used for layering.
```

## 第 114 页：VFX EMITTERS /W MANUAL SORT : NO OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 114 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VFX EMITTERS /W MANUAL SORT : NO OIT




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C               A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




That said, manual sorting can result in a lot of side effects as we can see later during
explosion frames where smoke tendrils look buggy and are unintended at that point.




                                                                                                                                                                  114
```

## 第 115 页：VFX EMITTERS /W MANUAL SORT : OIT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 115 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VFX EMITTERS /W MANUAL SORT : OIT




                                                                • Looks different
                                                                      − Often better “according to artists”
 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Which can be naturally fixed by OIT.
Of course, this depends on what was the intention – but the important piece of
information is that with depth offsetting tools some of this look can be achieved even
with OIT.




                                                                                                                                                                115
```

## 第 116 页：UI DOESN’T SORT CORRECTLY

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 116 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 UI DOESN’T SORT CORRECTLY



 3D UI IN THE WORLD

 • 2D UI classic composition mode
 • 3D UI uses 3D VFX
      − Sorts with the scene
      − Needs similar 2D composition

 • Camera offset /w perspective adjust
      − ‘sorting’ burns slices but workable


 • Offscreen 2D UI compose
      − Use in 3D as composed texture




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Some content teams were using VFX to compose 2D UI in the 3D world.
UI composition layering has its own layering and blend logic that assumes specific
sorting orders.
Unfortunately, that is not supported by OIT as this is exactly the close depth overlap
fail case scenario.
There are some way to support it with proposed AVBOIT.
A certain number of slices can be dedicated to layering – which would emulate strict
ordering of those VFX.
Similar effect can be achieved by manually altering vertex shaders for those draws,
that would manually alter depth at which those draws render to simulate sorted
layering.
Finally, the recommended way is to render UI as 2D composite offscreen, and once
composed, use it as texture in 3D world – which avoids all those problems.




                                                                                                                                                                116
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
fail case scenario.
Similar effect can be achieved by manually altering vertex shaders for those draws,
```

## 第 117 页：MIN SPEC PERFORMANCE / GEN8

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 117 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 MIN SPEC PERFORMANCE / GEN8



 SIMILAR SCALING ACROSS GENERATIONS

 • Gen8 similar scaling to Gen9
 • Same for PC low specs
      − AMD RX470
      − Nvidia Gtx 1060




                Nvidia Gtx 1060 Cinematic test 1080p (ms)      Pass                                           PS4 1080p (ms)
            AVBOIT RGB                        No OIT                       MBOIT                           WBOIT                             AVBOIT                           No OIT
            Transmittance
                                                            Transparency   3.31                                3.04                              2.11                              1.8
                     12.2                      11.2
                                                                VFX        3.34                                3.12                              2.91                             2.72
                                                               Total       6.65                                6.16                              5.03                              4.5

 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                       A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




ABVOIT provides a reasonable performance alternative on all paltforms.




                                                                                                                                                                                          117
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
 • Same for PC low specs
                Nvidia Gtx 1060 Cinematic test 1080p (ms)      Pass                                           PS4 1080p (ms)
```

## 第 118 页：CONCLUSION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 118 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
CONCLUSION
 & FUTURE DEVELOPMENT




                        118
```

## 第 119 页：AVBOIT : CONCLUSION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 119 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
AVBOIT : CONCLUSION



GOOD ENOUGH

• Fast(er) as no OIT
     − Early cull
     − Multi resolution rendering

• Stable
• Minimal content rework
• RGB transparency OIT becomes practical
• Scalable
• Opens door for new features
     − OIT refraction & scattering
     − Transparency depth of field
     − Transparency motion blur



© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     119
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
• Fast(er) as no OIT
• Opens door for new features
```

## 第 120 页：EXTENSIONS : DISTORTION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 120 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
EXTENSIONS : DISTORTION




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                     120
```

## 第 121 页：EXTENSIONS : DISTORTION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 121 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
EXTENSIONS : DISTORTION
ACCUMULATE DISTORTION AND WEIGHT BY TRANSMITTANCE INTEGRAL




• Weight more things by transmittance integral
     − Motion vectors
     − Optical depth for DOF
     − Refraction and scattering



© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                   A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                     121
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
     − Optical depth for DOF
```

## 第 122 页：AVBOIT : FUTURE WORK

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 122 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
AVBOIT : FUTURE WORK



SPARSE VOXEL TEXTURE

• High precision needs a lot of memory
     − But usage is very sparse

• Software bounds depth pre-pass
     −     occupancy data at target resolution

• Use occupancy to allocate chunks of virtual buffer
• On physical memory overflow                          Physical Memory

     − Reallocate to lower target resolution
          •     Repat on fail




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                       A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                         122
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
• Use occupancy to allocate chunks of virtual buffer
```

## 第 123 页：REFERENCES

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 123 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
REFERENCES

•    [KOH16] Practical Order Independent Transparency, Johan Köhler, Treyarch 2016
•    [VAS20] A Survey of Multifragment Rendering, A. A. Vasilakis et al., Eurographics 2020
•    [WYM16] Exploring and Expanding the Continuum of OIT Algorithms, Wyman, C., HPG16
•    [MB13] Weighted Blended Order-Independent Transparency, Morgan McGuire, Louis Bavoil, Siggraph 2014
•    [KIR18] Rendering Technology in ‘Agents of Mayhem, Scott Kircher, GDC 2018
•    [ASM21] Wavelet Transparency, Maksim Aizenshtein, Niklas Smal, Morgan McGuire, 2021
•    [PMWK17] Improved moment shadow maps for translucent occluders, soft shadows and single scattering, Peters, Münstermann, Wetzstein, Klein, JCGT 6, 1 March 2017
•    [BAV13] Particle Shadows & Cache-Efficient Post-Processing, Louis Bavoil, GDC 2013
•    [WRO14] Volumetric fog: Unified, compute shader based solution to atmospheric scattering, Bart Wronski, Siggraph 2014
•    [DRO17] Rendering of Call of Duty : Infinite Warfare, Michal Drobot, Digital Dragons 2017
•    [DRO17.2] Improved Culling for Tiled and Clustered Rendering in Call of Duty: Infinite Warfare, Michal Drobot, Siggraph 2017
•    [SAL11] Adaptive Transparency, Marco Salvi et al., Siggraph 2011
•    [SHA18] Moment Transparency, Brian Sharpe, Hpg 18
•    [ESSL10] Stochastic transparency, Enderton E., Sintorn E., Shirley P., Luebke D.: I3DG 2010
•    [WYM17] Hashed Alpha Testing, Wyman Chris, Morgan McGuire, I3DG 2017
•    [DEL11] Transmittance Function Mapping , Cyril Delalandre, Pascal Gautron, Jean-Eudes Marvie, Guillaume Francois, I3D 2011
•    [LOK00] Deep Shadow Maps, Tom Lokovic, Eric Veach, Siggraph 2000
•    [NGU05] Real-time rendering and animation of realistic hair in ’nalu’, Hubert Nguyen, William Donnelly, GPU Gems 2
•    [SIN08] Real-time approximate sorting for self shadowing and transparency in hair rendering, Erik Sintron, Ulf Assarsson, Siggraph 2008



                                                                                                                                                                                                                                            12
© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                                          A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5    3




                                                                                                                                                                                                                                                 123
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
•    [KOH16] Practical Order Independent Transparency, Johan Köhler, Treyarch 2016
•    [VAS20] A Survey of Multifragment Rendering, A. A. Vasilakis et al., Eurographics 2020
•    [WYM16] Exploring and Expanding the Continuum of OIT Algorithms, Wyman, C., HPG16
•    [MB13] Weighted Blended Order-Independent Transparency, Morgan McGuire, Louis Bavoil, Siggraph 2014
•    [KIR18] Rendering Technology in ‘Agents of Mayhem, Scott Kircher, GDC 2018
•    [ASM21] Wavelet Transparency, Maksim Aizenshtein, Niklas Smal, Morgan McGuire, 2021
•    [PMWK17] Improved moment shadow maps for translucent occluders, soft shadows and single scattering, Peters, Münstermann, Wetzstein, Klein, JCGT 6, 1 March 2017
•    [BAV13] Particle Shadows & Cache-Efficient Post-Processing, Louis Bavoil, GDC 2013
•    [WRO14] Volumetric fog: Unified, compute shader based solution to atmospheric scattering, Bart Wronski, Siggraph 2014
•    [DRO17] Rendering of Call of Duty : Infinite Warfare, Michal Drobot, Digital Dragons 2017
•    [DRO17.2] Improved Culling for Tiled and Clustered Rendering in Call of Duty: Infinite Warfare, Michal Drobot, Siggraph 2017
•    [SAL11] Adaptive Transparency, Marco Salvi et al., Siggraph 2011
•    [SHA18] Moment Transparency, Brian Sharpe, Hpg 18
•    [ESSL10] Stochastic transparency, Enderton E., Sintorn E., Shirley P., Luebke D.: I3DG 2010
•    [WYM17] Hashed Alpha Testing, Wyman Chris, Morgan McGuire, I3DG 2017
•    [DEL11] Transmittance Function Mapping , Cyril Delalandre, Pascal Gautron, Jean-Eudes Marvie, Guillaume Francois, I3D 2011
•    [LOK00] Deep Shadow Maps, Tom Lokovic, Eric Veach, Siggraph 2000
•    [NGU05] Real-time rendering and animation of realistic hair in ’nalu’, Hubert Nguyen, William Donnelly, GPU Gems 2
•    [SIN08] Real-time approximate sorting for self shadowing and transparency in hair rendering, Erik Sintron, Ulf Assarsson, Siggraph 2008
```

## 第 124 页：T HE PRE M IE R C ONF E RE NC E & E X HIBIT ION ON

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 124 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                                                             T HE PRE M IE R C ONF E RE NC E & E X HIBIT ION ON
                                                                  C OM PUT E R G RA PHIC S & INT E RA C T IV E T E C HNIQUE S




                                              JOIN US
Proud to be a Special Interest Group Within
the Association for Computing Machinery.




     www.activisionblizzard.com/careers
© 2025 SIGGRAPH. ALL RIGHTS RESERVED.              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5
                                                                                                                           © 2025 Activision Publishing,
                                                                                                                                                    4
                                                                                                                                                           12
                                                                                                                                                         Inc.




                                                                                                                                                                124
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
the Association for Computing Machinery.
```

## 第 125 页：research.activision.com T HE PRE M IE R C ONF E RE NC E & E X HIBIT ION ON

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 125 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                              research.activision.com                        T HE PRE M IE R C ONF E RE NC E & E X HIBIT ION ON
                                                                                  C OM PUT E R G RA PHIC S & INT E RA C T IV E T E C HNIQUE S




                                                  Q&A
Proud to be a Special Interest Group Within
the Association for Computing Machinery.



                                               michal@infinityward.com
© 2025 SIGGRAPH. ALL RIGHTS RESERVED.               @MichalDrobot  A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5
                                                                                                                                           © 2025 Activision Publishing,
                                                                                                                                                                    5
                                                                                                                                                                           12
                                                                                                                                                                         Inc.




                                                                                                                                                                                125
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
the Association for Computing Machinery.
```

## 第 126 页：T HE PRE M IE R C ONF E RE NC E & E X HIBIT ION ON

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 126 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                                                            T HE PRE M IE R C ONF E RE NC E & E X HIBIT ION ON
                                                                 C OM PUT E R G RA PHIC S & INT E RA C T IV E T E C HNIQUE S




                                              BONUS
Proud to be a Special Interest Group Within
the Association for Computing Machinery.




                                                                                                                                                          12
                                                  A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5    6




                                                                                                                                                               126
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
the Association for Computing Machinery.
```

## 第 127 页：VBOIT MONOCHROME

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 127 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT MONOCHROME




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Using monochrome transmittance OIT results in visually correct results – but clearly
lacking color.




                                                                                                                                                                127
```

## 第 128 页：VBOIT COLOR - TRANS PASS EXTINCTION ONLY

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 128 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT COLOR - TRANS PASS EXTINCTION ONLY




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Accumulating color transmittance during transparency draws will result in correct
color of all opaque pixels, but will create odd looking color misses on any
transparency.
I.e. here opaque pixels behind glass show green tint, while smoke, that is behind glass
(and looked convincing in monochrome) suddenly sticks out missing color hint.




                                                                                                                                                                128
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
I.e. here opaque pixels behind glass show green tint, while smoke, that is behind glass
(and looked convincing in monochrome) suddenly sticks out missing color hint.
```

## 第 129 页：VBOIT COLOR - TRANS PASS EXTINCTION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 129 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT COLOR - TRANS PASS EXTINCTION
 + WEIGHTED CHROMATICITY SKEW IN RESOLVE




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Applying chromaticity skew in resolve partially solves the problem with incorrectly
colored transparency but is unable to resolve depth dependent complexities with
partial incorrect color occlusion.




                                                                                                                                                                129
```

## 第 130 页：VBOIT COLOR – RGB EXTINCTION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 130 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT COLOR – RGB EXTINCTION




 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




Using RGB OIT extinction integral correctly resolves the image.




                                                                                                                                                                130
```

## 第 131 页：VBOIT : COLOR APPROXIMATION : FAIL CASE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 131 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : COLOR APPROXIMATION : FAIL CASE



HARD PROBLEM
• VBOIT fails due to slice overlap
     −    Color skew will get mixed in
     −    Low precision of normalization and extinction does not help

• Car fixture is made of 2 glass surfaces
     −    Back to back
     −    0.125 inch depth delta
     −    Front is clear refractive plastic
     −    Back is red close to opaque plastic

• Slices with overlap can be detected
     −    Remove color skew
     −    Overly conservative but shippable

• Ask artists to avoid this setup
• RGB Transmittance fixes the problem
• More slices fixes the problem

© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                              A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                131
```

## 第 132 页：VBOIT : COLOR

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 132 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VBOIT : COLOR



FOR MEMORY CONSTRAINED PLATFORMS                                                 // Transparency draw
                                                                                 float3 extinction = GetExtinctionFromTransmittance( 1.0 - transparency );

• Skew final color by accumulated RGB extinction                                 if( OitEnableChromaTransSkew() )
                                                                                 {
     − Use monochrome transmittance integral                                       extinctionRGB = lerp( extinction, extinctionRGB, transmittanceIntegral * FogTransmittanceIntegral );
                                                                                 }
     − Accumulate color extinction in transparency pass
          •     Run only for color transparencies

     − During resolve                                                            // Resolve
                                                                                 if( OitEnableChromaTransSkew() )
          •     Accumulated RGB extinction integral from transparency pass for
                                                                                 {
                background                                                         float transmittanceLuma = GetLuma( transmittance );
                                                                                   chromaSkew = transmittanceRGB * rcp( transmittanceLuma );
          •     Skew normalization factor by transmittance chromaticity          }

                                                                                 float3 cf = opaqueColor * transmittance
                                                                                   + transColor * normFactor * chromaSkew;




© 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                                                                          A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                                                                            132
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
FOR MEMORY CONSTRAINED PLATFORMS                                                 // Transparency draw
                                                                                 float3 extinction = GetExtinctionFromTransmittance( 1.0 - transparency );
• Skew final color by accumulated RGB extinction                                 if( OitEnableChromaTransSkew() )
     − Use monochrome transmittance integral                                       extinctionRGB = lerp( extinction, extinctionRGB, transmittanceIntegral * FogTransmittanceIntegral );
          •     Run only for color transparencies
     − During resolve                                                            // Resolve
                                                                                 if( OitEnableChromaTransSkew() )
          •     Accumulated RGB extinction integral from transparency pass for
                background                                                         float transmittanceLuma = GetLuma( transmittance );
                                                                                   chromaSkew = transmittanceRGB * rcp( transmittanceLuma );
          •     Skew normalization factor by transmittance chromaticity          }
                                                                                 float3 cf = opaqueColor * transmittance
                                                                                   + transColor * normFactor * chromaSkew;
```

## 第 133 页：VBOIT : COLOR APPROXIMATION : FAIL CASE

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 133 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : COLOR APPROXIMATION : FAIL CASE

Fog scattering gets color skew                Weight chromaticity by total transmittance
                                              Including volumetric fog
 SHOW FOGGY CAR




                                               // Transparency draw
                                               if( OitEnableChromaTransSkew() )
                                               {
                                                 extinctionRGB = lerp( extinction, extinctionRGB, transmittanceIntegral *
                                               FogTransmittanceIntegral );
                                               }

 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                           A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                                                                                              133
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                               // Transparency draw
                                               if( OitEnableChromaTransSkew() )
                                                 extinctionRGB = lerp( extinction, extinctionRGB, transmittanceIntegral *
                                               FogTransmittanceIntegral );
```

## 第 134 页：VBOIT : COLOR APPROXIMATION : SELF TINT

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 134 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : COLOR APPROXIMATION : SELF TINT




                                              Surface skews itself – i.e. reflection gets tinted

 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                 A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




During resolve accumulated transmittance chromaticity will be applied to pixels –
thus all surfaces rendered without any depth / surface separation.
While this is unavoidable, there are some things we can do to improve the first event.




                                                                                                                                                                    134
```

## 第 135 页：VBOIT : COLOR APPROXIMATION : SELF CORRECTION

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 135 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 VBOIT : COLOR APPROXIMATION : SELF CORRECTION




                                                // Reconstruct self-extinction as in oit resolve reconstruction
                                                float3 transmittanceRGB = float3(
                                                  TransmittanceFromExtinctionIntegral( extinctionRGB.r ),
                                                  TransmittanceFromExtinctionIntegral( extinctionRGB.g ),
                                                  TransmittanceFromExtinctionIntegral( extinctionRGB.b ) );

                                                float3 chromaSkew = 1.0;
                                                float transmittanceRGBLuma = RGBToYUV( transmittanceRGB ).x;
                                                transmittanceRGBLuma = transmittanceRGBLuma > 0.0 ?
                                                 transmittanceRGBLuma : 1.0;
                                                chromaSkew = transmittanceRGB * rcp( transmittanceRGBLuma );

                                                // Inverse chroma skew avoid self chroma skew
                                                fragment.color.rgb /= chromaSkew;



                                                Apply reverse self skew during transparency rendering
                                                Fixed single layer problem
 © 2 0 2 5 AC TIVISIO N PU BL ISH ING, IN C                             A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




During transparency rendering we can calculate future skew, and apply its reverse to
pixels being rendered. This will effectively fix the event and partially correct others –
although it will become a frequency soup.




                                                                                                                                                                                135
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                // Reconstruct self-extinction as in oit resolve reconstruction
                                                float3 transmittanceRGB = float3(
                                                  TransmittanceFromExtinctionIntegral( extinctionRGB.r ),
                                                  TransmittanceFromExtinctionIntegral( extinctionRGB.g ),
                                                  TransmittanceFromExtinctionIntegral( extinctionRGB.b ) );
                                                float3 chromaSkew = 1.0;
                                                float transmittanceRGBLuma = RGBToYUV( transmittanceRGB ).x;
                                                transmittanceRGBLuma = transmittanceRGBLuma > 0.0 ?
                                                 transmittanceRGBLuma : 1.0;
                                                chromaSkew = transmittanceRGB * rcp( transmittanceRGBLuma );
                                                // Inverse chroma skew avoid self chroma skew
                                                fragment.color.rgb /= chromaSkew;
```

## 第 136 页：A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5

> 来源：`AVBOIT_SIG2025_MDROBOT-final.pdf` 第 136 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
A D V A N C ES I N R E A L - T IM E R E N D ER IN G I N G G A M E S C O U R S E, S I G G R A PH 202 5




                                                                                                        136
```
