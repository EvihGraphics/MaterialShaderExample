# Fluid and Volumetric Fog

> 原始资料：`流体模拟与体积雾.txt`

我按**数据库外部公开资料**重新筛了一遍。UE5 相关案例其实不少，但需要先看清它们各自在模拟什么：

- **沙地／雪地模拟**：大多是 Render Target、RVT 或材质高度场变形。
- **沙尘暴／雪暴**：大多是 Niagara 粒子、雾片、材质和灯光的分层组合。
- **动态体积雾**：分为局部雾密度控制、实时 Grid3D 气体模拟、VDB/SVT 回放三类。
- 真正采用 **MPM、DEM 或逐颗粒接触求解**的 UE5 开源沙雪案例非常少，这轮没有找到成熟的生产级完整项目。

# 一、沙地与雪地交互模拟

| 案例 | 核心技术 | 模拟真实性 | 资料完整度 | 推荐度 |
|---|---|---:|---:|---:|
| **FluidNinja LIVE：Snow, Sand and Complex Environments** | 局部二维流体场驱动沙、雪、表面材质、粒子与体积；Density、Velocity、Pressure 可作为 Render Target 输出，并可和 Niagara 双向交互 | 中：场模拟，不是颗粒 MPM | 官方完整教程＋示例资产 | **S** |
| **Interactive Snow World — UE5 Technical Art Breakdown 2.0** | Render Target 记录角色轨迹，驱动雪面高度、脚印、表面混合和二级粒子 | 中高：真实记录交互，但雪体并非粒子求解 | 完整技术拆解视频 | **S** |
| **UE5 Landscape Deformation Tutorial 系列** | 运行时地形／表面变形，明确覆盖雪和沙；通过局部高度数据制造脚印、凹陷和堆积感 | 中 | 系列教程 | **S** |
| **UE5 Surface Deformation — Real-Time Snow & Ground Depth** | 动态深度场记录角色经过位置，实时改变地面轮廓 | 中 | 完整教程 | **A** |
| **Footprint Masking and Sand Particle FX** | 脚印 Mask 控制材质层混合，同时用 Niagara 生成扬沙粒子 | 低到中：视觉反馈型 | 分步教程 | **A** |
| **Procedural Snow on Any Mesh** | 基于法线、高度和材质函数给任意 Mesh 添加积雪 | 低：不变形 | 简短公开教程 | B |

FluidNinja 是这一组中最系统的。官方页面明确列出它能够驱动水、云、烟、火、沙、雪和植被，并把模拟结果用于材质、Niagara、Fog、Cloud 与 Heterogeneous Volume；同时公开 Density、Velocity、Pressure 缓冲。([fab.com](https://www.fab.com/listings/80fcf53e-49f7-4635-a71c-ba81280c6618?lang=en&utm_source=chatgpt.com))

作者的沙雪教程展示了一个模拟系统如何同时驱动复杂环境；作者也明确说明，底层是经过调整的二维流体模拟，而不是固体颗粒求解，所以雪有时会表现得偏流体。([youtube.com](https://www.youtube.com/watch?v=3cBo9pHUXUA&utm_source=chatgpt.com))

`Interactive Snow World` 和 Landscape Deformation 系列更适合学习**雪地／沙地交互表面**：前者明确采用 Render Target 驱动雪面变形，后者直接覆盖雪、沙等地表实时变形。([youtube.com](https://www.youtube.com/watch?v=jLPnTz2MlCs&utm_source=chatgpt.com))

脚印 Mask 案例则把表面材质变化和沙粒特效组合起来，适合作为雪崩或沙尘暴经过后的地表反馈层。([youtube.com](https://www.youtube.com/watch?v=__-8JlcZKzw&utm_source=chatgpt.com))

# 二、UE5 沙尘暴案例

| 案例 | 技术路线 | 特点 | 资料完整度 | 推荐度 |
|---|---|---|---:|---:|
| **Infinite Desert Sandstorm** | Niagara Data Channels＋玩家周围动态生成＋无限地形 | 最适合大世界持续沙暴 | 完整视频案例 | **S** |
| **How to Create a Sand Storm in UE5 — MadVFX** | 多层 Niagara、沙墙、近景沙粒、雾、材质和灯光 | 从效果构建角度较完整 | 系列教程 | **S** |
| **Unreal Engine 5 — Sand Storm，CodeLikeMe** | 远处生成沙暴并逐渐靠近玩家，控制天气事件生命周期 | 适合“沙暴来袭”玩法 | 完整教程 | **A** |
| **5 Steps to Creating a Niagara Storm** | 材质、Niagara 粒子和灯光技巧组合 | 适合理解视觉层级 | 完整拆解 | **A** |
| **How To Make Blowing Dust** | 贴地风沙、局部扬尘和 Niagara 粒子 | 适合作为近景层 | 单项教程 | B |

`Infinite Desert Sandstorm` 值得重点看：案例使用 Niagara Data Channels，让沙暴在无限沙漠中持续围绕玩家存在，而不是在世界中铺设一个巨大粒子系统。这种设计对 World Partition、大地图和多人视点附近的局部效果预算很有启发。([youtube.com](https://www.youtube.com/watch?v=FHspdYNtDnY&utm_source=chatgpt.com))

MadVFX 的系列更偏完整视觉搭建，从沙墙、粒子到环境效果逐层制作。([youtube.com](https://www.youtube.com/watch?v=m7rw_5SZSrs&utm_source=chatgpt.com))

CodeLikeMe 的案例则带有明确的天气事件结构：沙暴最初出现在远方，之后向玩家区域移动，适合做危险区域逼近、能见度变化和 Gameplay 状态切换。([youtube.com](https://www.youtube.com/watch?v=EKT1guXbKAQ&utm_source=chatgpt.com))

Aziel Arts 的五步方案明确组合材质、Niagara 和灯光，其价值在于说明沙尘暴通常不是一个单独的模拟器，而是多个频率层共同构成。([youtube.com](https://www.youtube.com/watch?v=XzT3zvco5HY&utm_source=chatgpt.com))

典型分层是：

```text id="6gk7wo"
远景沙墙 / 体积轮廓
        ↓
中景流动雾片与噪声
        ↓
近景 Niagara 沙粒
        ↓
贴地扬尘与地表流痕
        ↓
曝光、天空、灯光和色调变化
```

这些案例基本都属于**视觉天气模拟**，并没有执行完整三维沙粒流体或 MPM 求解。

# 三、雪暴与暴风雪案例

| 案例 | 核心技术 | 特点 | 推荐度 |
|---|---|---|---:|
| **Create a Snow Storm VFX with Niagara** | 雪粒、曲面雾片、风向、近景浓雾和环境交互 | 单个暴风雪效果最完整的教程之一 | **S** |
| **Snow Storm Particle Effect in Niagara** | 风路径、雪粒运动和暴风雪粒子系统 | 适合学习风场导向 | **A** |
| **Weather Effects — Snow, Blizzard, Rain & Thunder** | 10 套 Niagara 天气系统，运行时可调密度、速度、扰动和方向 | 可直接拆解成天气组件 | **A** |
| **Hyper Dynamic Weather & Sky System v4** | 天气状态机、雪暴、沙暴、雾、粒子、天空和多人同步 | 上层天气系统案例 | **A** |
| **Snow Particle Tutorial** | 雪花、碰撞和基础 Niagara | 入门基础 | B |

Mao 的暴风雪教程使用 Niagara 构建雪暴，还包括雾状纹理、曲线雾层和靠近地形的风雪表现，比单纯发射雪花完整得多。([youtube.com](https://www.youtube.com/watch?v=ZOJ_AHlldfw&utm_source=chatgpt.com))

CodeLikeMe 的雪暴案例重点是雪粒和 Wind Path，适合研究如何让粒子沿阵风路径运动，而不是统一做线性下落。([youtube.com](https://www.youtube.com/watch?v=31vIBODqhhk&utm_source=chatgpt.com))

`Weather Effects` 是一个可运行的 Niagara 天气资产包，包含雪、暴风雪、冰雹、雨和雷暴，密度、速度、湍流与方向均可在运行时调整。([fab.com](https://www.fab.com/listings/bb2a4e45-a173-482c-a98a-2b0c4b332a83?lang=pl&utm_source=chatgpt.com))

`Hyper Dynamic Weather & Sky System v4` 则更偏系统层，包含 Blizzard、Sandstorm 等天气 Profile，并统一改变雾密度、风、能见度、光照和地表状态，还提供多人天气同步。([fab.com](https://www.fab.com/listings/8b835206-b9a5-4d27-9452-cf299dd09ef9?utm_source=chatgpt.com))

因此雪暴可以拆成：

```text id="wq10ba"
Weather State
├── 全局风速与阵风
├── 远景白化 / 能见度
├── 中景雪雾
├── 近景雪花和冰粒
├── 地面吹雪
├── 表面积雪参数
└── 脚印与雪粉反馈
```

# 四、动态体积雾与烟雾案例

| 案例 | 是否真实动态模拟 | 核心价值 | 开放程度 | 推荐度 |
|---|---:|---|---:|---:|
| **Niagara Fluids 3D Gas 官方案例** | **是** | Grid3D Density、Temperature、Velocity、碰撞、浮力与体积渲染 | 官方模板可直接拆 | **S** |
| **Creating VFX with Niagara Fluids, SVT and Heterogeneous Volumes** | 模拟＋缓存＋回放 | 从实时模拟到 SVT/HV 渲染的完整管线 | 官方完整演讲 | **S** |
| **UE5-Tornado** | **是** | Niagara 流体模拟＋地面尘土＋碎片，是可下载工程 | GitHub 完整项目 | **S** |
| **FluidNinja LIVE** | 是，二维／2.5D | 局部交互流场驱动 Fog、Cloud、HV 和 Niagara | 商业工程＋公开教程 | **S** |
| **Local Fog Volumes** | 否 | 高性能局部动态雾密度，参数可由蓝图实时变化 | UE 原生 | **A** |
| **Volumetric Fog＋Volume Material** | 密度可动态变化，但不自行求解流体 | Froxel 体积光照和局部密度注入 | UE 原生 | **A** |
| **Advanced Fog Shader UE5.3** | 否 | 自定义雾材质、噪声、方向运动和蓝图控制 | 完整公开教程 | **A** |
| **Niagara Fluids Smoke Portal / Cryogenic Smoke** | **是** | 实时 Grid3D 烟雾效果的具体应用 | 完整教程 | A |
| **Heterogeneous Volumes＋SVT** | 否，通常是缓存回放 | 高质量 VDB/SVT 烟雾、云、爆炸 | UE 原生实验功能 | A |

UE 官方 Niagara Fluids 提供 2D 与 3D 模板；官方定位是 2D 方案偏游戏实时，3D 模板更偏电影级使用。3D Gas Quick Start 直接从 `Grid3D_Gas_Explosion` 开始，包含模拟域、碰撞和气体参数。([dev.epicgames.com](https://dev.epicgames.com/documentation/unreal-engine/niagara-fluids-quick-start-guide-for-unreal-engine?utm_source=chatgpt.com))

Epic 的 Unreal Fest 课程进一步展示了：

```text id="22lxmf"
Niagara Fluids 实时求解
        ↓
Sparse Volume Texture 缓存
        ↓
Heterogeneous Volume 渲染
        ↓
环境灯光与 Sequencer
```

这条路线可在实时模拟成本过高时，把最终结果转换成较轻的缓存播放。([dev.epicgames.com](https://dev.epicgames.com/community/learning/talks-and-demos/1V6r/unreal-engine-creating-visual-effects-with-niagara-fluids-sparse-volume-textures-and-heterogeneous-volumes-in-ue-unreal-fest-2023?utm_source=chatgpt.com))

`UE5-Tornado` 是这轮搜索中少见的**可直接下载项目**。它基于 UE5.6，主体龙卷风采用流体模拟生成烟雾行为，同时组合地面尘土、烟迹和飞散碎片；作者也明确警告实时流体版本成本较高，更适合作为学习和原型。([github.com](https://github.com/RafaelColpani/UE5-Tornado?utm_source=chatgpt.com))

## UE 原生雾方案的区别

### Volumetric Fog

UE 的 Volumetric Fog 在相机视锥体内构建体积纹理，逐点计算参与介质的密度和光照；可以使用 Volume Domain 材质或粒子向局部区域注入密度。它本身是渲染和光照系统，不会自动解算速度场。([dev.epicgames.com](https://dev.epicgames.com/documentation/unreal-engine/volumetric-fog-in-unreal-engine?utm_source=chatgpt.com))

### Local Fog Volumes

Local Fog Volume 是低成本的局部球形／高度雾体。启用 Volumetric Fog 时，它会被体素化到体积雾中；未启用时则以自己的屏幕合成路径渲染。它适合沙尘区域、山谷雾、室内雾团和天气过渡，但不产生流体运动。([dev.epicgames.com](https://dev.epicgames.com/documentation/unreal-engine/local-fog-volumes-in-unreal-engine?utm_source=chatgpt.com))

Local Fog Volumes 从 UE 5.5 起被标记为 Production Ready，并覆盖所有平台和质量等级，因此非常适合作为低配回退层。([dev.epicgames.com](https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-5-5-release-notes?utm_source=chatgpt.com))

### Heterogeneous Volumes

Heterogeneous Volume 用 Volume Material 读取 Sparse Volume Texture，适合 VDB 烟雾、火焰和云序列；官方目前仍将其标记为 Experimental。([dev.epicgames.com](https://dev.epicgames.com/documentation/unreal-engine/heterogeneous-volumes-in-unreal-engine?utm_source=chatgpt.com))

# 五、最值得优先研究的 8 个案例

按你当前“沙暴／雪暴＋动态体积＋UE 管线扩展”的目标排序：

| 顺序 | 案例 | 最值得学习的模块 |
|---:|---|---|
| 1 | **FluidNinja Snow, Sand and Complex Environments** | 局部流场、沙雪地表、粒子与体积协同 |
| 2 | **Infinite Desert Sandstorm** | Niagara Data Channels、大世界局部天气 |
| 3 | **Interactive Snow World** | Render Target 雪面变形和积雪反馈 |
| 4 | **UE5 Landscape Deformation** | 沙地／雪地共用的高度场交互 |
| 5 | **Niagara Fluids 3D Gas** | 真正的 Density、Velocity、Pressure 体积模拟 |
| 6 | **Niagara Fluids＋SVT＋HV** | 模拟、缓存和最终体渲染管线 |
| 7 | **UE5-Tornado GitHub** | 可下载的 Niagara 流体综合工程 |
| 8 | **Local Fog Volumes** | 面向游戏和低配平台的动态局部雾回退 |

# 六、对你的方案最有用的组合

不建议只寻找一个“万能沙暴插件”，更合理的是把这些案例拼成分层架构：

```text id="1lp390"
Weather Manager
天气强度、风向、生命周期、能见度
             ↓
┌─────────────────────────────┐
│ 远景层                       │
│ Volumetric Cloud / 沙暴墙    │
└─────────────────────────────┘
             ↓
┌─────────────────────────────┐
│ 中景体积层                   │
│ Local Fog Volume             │
│ Niagara Grid3D / FluidNinja  │
└─────────────────────────────┘
             ↓
┌─────────────────────────────┐
│ 近景离散层                   │
│ Niagara 沙粒、雪花、碎屑     │
└─────────────────────────────┘
             ↓
┌─────────────────────────────┐
│ 地表反馈层                   │
│ RT/RVT 沙雪变形、脚印、堆积  │
└─────────────────────────────┘
```

其中最有研究价值的三条源码／数据链是：

```text id="vmmmnd"
FluidNinja
NinjaLiveComponent → Render Targets → 材质 Pass → 输出体积/粒子

Niagara Fluids
Grid3D → Simulation Stages → Density/Velocity → Volume Renderer

地表变形
角色/物体 → Scene Capture或Niagara → Render Target → Height/Material
```

**检索结论**：UE5 公开案例已经足够搭建一个高质量沙暴或雪暴原型；真正缺少的不是视觉案例，而是“颗粒沙雪求解＋体积空气耦合＋地表沉积”完整公开实现。当前最现实的生产路线仍然是 **FluidNinja／Niagara 局部场＋RT 地表变形＋多层天气 VFX**。
