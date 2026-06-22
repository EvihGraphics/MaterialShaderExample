# 在 UE5 外部模块中扩展复杂渲染管线实践 - Markdown 转译

## 资料身份

- 原始文件：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf`
- 页数：69
- 用途：UE 插件复杂渲染管线、SceneViewExtension、SceneProxy、Mesh Pipeline 和全屏合成指导
- 转译方式：按页提取文本，并对可识别的代码／接口片段生成代码块。
- 原始 PDF 未包含在本指导包中，以适配仓库忽略 PDF 的策略。

## 使用约束

- 本 Markdown 是 Agent 可检索版本，不替代原 PDF 的视觉排版。
- 架构图、流程图和多栏代码应结合页码理解。
- 代码转录中无法完全确认的行不得直接复制进生产代码，必须回到真实 UE5.7 源码或实际工程验证。

## 页码索引

- [第 1 页：Single Volume 渲染器](#第-1-页singlevolume渲染器)
- [第 2 页：Part1: 统一体积渲染策略 Part2: 渲染管线设计 Part3: GPU渲染和插件部署](#第-2-页part1统一体积渲染策略part2渲染管线设计part3gpu渲染和插件部署)
- [第 3 页：如何统一体积渲染](#第-3-页如何统一体积渲染)
- [第 4 页：UFS 2023 回顾](#第-4-页ufs2023回顾)
- [第 5 页：渲染插件的设计初衷](#第-5-页渲染插件的设计初衷)
- [第 6 页：体积云](#第-6-页体积云)
- [第 7 页：体积雾](#第-7-页体积雾)
- [第 8 页：体积烟](#第-8-页体积烟)
- [第 9 页：燃烧过程](#第-9-页燃烧过程)
- [第 10 页：黑洞](#第-10-页黑洞)
- [第 11 页：上述方案大一统的可行性？](#第-11-页上述方案大一统的可行性)
- [第 12 页：解决介质表征手段：Shape Profile](#第-12-页解决介质表征手段shapeprofile)
- [第 13 页：渲染器扩展与管线设计](#第-13-页渲染器扩展与管线设计)
- [第 14 页：C++ ? U++?](#第-14-页cu)
- [第 15 页：如何在插件中向渲染器插入自定义管线](#第-15-页如何在插件中向渲染器插入自定义管线)
- [第 16 页：如何在插件中向渲染器插入自定义管线](#第-16-页如何在插件中向渲染器插入自定义管线)
- [第 17 页：扩展渲染器](#第-17-页扩展渲染器)
- [第 18 页：游戏线程](#第-18-页游戏线程)
- [第 19 页：问题1：逻辑线程与渲染线程的数据分离，同步，更新怎么做的？](#第-19-页问题1逻辑线程与渲染线程的数据分离同步更新怎么做的)
- [第 20 页：单体积材料组件和容器设计](#第-20-页单体积材料组件和容器设计)
- [第 21 页：单体积材料组件和容器设计](#第-21-页单体积材料组件和容器设计)
- [第 22 页：Material Component 的设计](#第-22-页materialcomponent的设计)
- [第 23 页：Material Component 的设计](#第-23-页materialcomponent的设计)
- [第 24 页：Material Component 的设计](#第-24-页materialcomponent的设计)
- [第 25 页：Material Component 的设计](#第-25-页materialcomponent的设计)
- [第 26 页：Material Component 的设计](#第-26-页materialcomponent的设计)
- [第 27 页：Scene Proxy 的设计](#第-27-页sceneproxy的设计)
- [第 28 页：Scene Proxy 的设计](#第-28-页sceneproxy的设计)
- [第 29 页：Scene Proxy 的设计](#第-29-页sceneproxy的设计)
- [第 30 页：Scene Proxy 的设计](#第-30-页sceneproxy的设计)
- [第 31 页：Scene Proxy 的设计](#第-31-页sceneproxy的设计)
- [第 32 页：Scene Proxy 的设计](#第-32-页sceneproxy的设计)
- [第 33 页：Scene Proxy 的设计](#第-33-页sceneproxy的设计)
- [第 34 页：Scene Proxy 的设计](#第-34-页sceneproxy的设计)
- [第 35 页：渲染管线的设计](#第-35-页渲染管线的设计)
- [第 36 页：渲染管线的设计](#第-36-页渲染管线的设计)
- [第 37 页：渲染管线的设计](#第-37-页渲染管线的设计)
- [第 38 页：渲染管线的设计](#第-38-页渲染管线的设计)
- [第 39 页：渲染管线的设计](#第-39-页渲染管线的设计)
- [第 40 页：void FSingleVolumeRendering::RenderSingleVolumeInternal( FSingleVolumeSceneProxy* Proxy, bool Translucent, bool ApplyEmissionAndTransmittanc](#第-40-页voidfsinglevolumerenderingrendersinglevolumeinternalfsinglevolumesceneproxyproxybooltranslucentboolapplyemissionandtransmittanc)
- [第 41 页：网格绘制管线](#第-41-页网格绘制管线)
- [第 42 页：class FSingleVolumeMeshProcessor : public FMeshPassProcessor](#第-42-页classfsinglevolumemeshprocessorpublicfmeshpassprocessor)
- [第 43 页：网格绘制管线](#第-43-页网格绘制管线)
- [第 44 页：网格绘制管线](#第-44-页网格绘制管线)
- [第 45 页：网格绘制管线](#第-45-页网格绘制管线)
- [第 46 页：网格绘制管线](#第-46-页网格绘制管线)
- [第 47 页：class FSingleVolumeShaderPS : public FMeshMaterialShader](#第-47-页classfsinglevolumeshaderpspublicfmeshmaterialshader)
- [第 48 页：网格绘制管线](#第-48-页网格绘制管线)
- [第 49 页：网格绘制管线](#第-49-页网格绘制管线)
- [第 50 页：网格绘制管线](#第-50-页网格绘制管线)
- [第 51 页：网格绘制管线](#第-51-页网格绘制管线)
- [第 52 页：全屏混合管线 void SingleVolumeComposite::CompositeFullScreen(FRDGBuilder& GraphBuilder,](#第-52-页全屏混合管线voidsinglevolumecompositecompositefullscreenfrdgbuildergraphbuilder)
- [第 53 页：引入统计数据监控](#第-53-页引入统计数据监控)
- [第 54 页：VRE 的高效 GPU 渲染](#第-54-页vre的高效gpu渲染)
- [第 55 页：VRE 的高效GPU渲染](#第-55-页vre的高效gpu渲染)
- [第 56 页：VRE 的高效GPU渲染](#第-56-页vre的高效gpu渲染)
- [第 57 页：VRE 的高效GPU渲染](#第-57-页vre的高效gpu渲染)
- [第 58 页：VRE 的高效GPU渲染](#第-58-页vre的高效gpu渲染)
- [第 59 页：VRE 的高效GPU渲染](#第-59-页vre的高效gpu渲染)
- [第 60 页：VRE 的高效GPU渲染](#第-60-页vre的高效gpu渲染)
- [第 61 页：VRE 的高效GPU渲染](#第-61-页vre的高效gpu渲染)
- [第 62 页：VRE 的高效GPU渲染](#第-62-页vre的高效gpu渲染)
- [第 63 页：VRE 的高效GPU渲染](#第-63-页vre的高效gpu渲染)
- [第 64 页：VRE 的高效GPU渲染](#第-64-页vre的高效gpu渲染)
- [第 65 页：VRE 的高效GPU渲染](#第-65-页vre的高效gpu渲染)
- [第 66 页：VRE 的高效GPU渲染](#第-66-页vre的高效gpu渲染)
- [第 67 页：Single Volume 插件](#第-67-页singlevolume插件)
- [第 68 页：Single Volume 插件](#第-68-页singlevolume插件)
- [第 69 页：实时气态流体解算器模块](#第-69-页实时气态流体解算器模块)

---

## 第 1 页：Single Volume 渲染器

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 1 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Single Volume 渲染器
在UE5外部模块中扩展复杂渲染管线实践
    腾讯生态发展部-技术中心-Angelou.lv
```

## 第 2 页：Part1: 统一体积渲染策略 Part2: 渲染管线设计 Part3: GPU渲染和插件部署

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 2 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Part1: 统一体积渲染策略            Part2: 渲染管线设计      Part3: GPU渲染和插件部署
研发背景回顾                     总体设计思路             体渲染方程的不同变种


云，雾，烟，火焰，燃烧特性分析            功能的组件化，容器对象与材料组件   功能的插件化部署


统一体积渲染的可行性                 统一渲染代理的设计


Shape Profile 与 VDB 数据管理   网格绘制管线与全屏混合管线的设计

                           数据监控策略
```

## 第 3 页：如何统一体积渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 3 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何统一体积渲染
   Part 1
```

## 第 4 页：UFS 2023 回顾

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 4 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
UFS 2023 回顾




                 多层云系统   单体积渲染


 虚幻引擎：在UE5中打造多层体积天空
```

## 第 5 页：渲染插件的设计初衷

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 5 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
渲染插件的设计初衷
● 已经有了足够多关于体积渲染的研发积累

● 拥有更深的理解和更多新颖的设计思路

● 并在项目实践中慢慢发现了引擎的支持上限

● 跨平台的多端项目逐渐成为常态化

● UE5更新太快了，项目需要熟悉且稳定的节奏

● 需要一个平台以便走得更远

● 可复用的产品沉淀思考
```

## 第 6 页：体积云

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 6 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
         体积云
         特征：形状，结构，颜色，层次

         分布：空间尺度，地形地貌



         造型：如何控制形状与结构

         属性：强非均质型介质

         渲染：多级散射几乎是必须的

         编辑：云海，静态造景




现状：离线渲染器通常采用Delta Tracking类技术去解决
非均质问题，考虑光谱的彩色介质渲染，介质相干性还原
等，针对碰撞系数有较为严格的数学物理论证；而游戏引
擎则不考虑非均质问题，即采用基于“独立散射假设”与
“局部连续性假设”的传统介质模型，通过Raymarching
评估照明，一般不单独对吸收系数和散射系数进行建模，
而采用单位物理量的密度表示消光系数，这几乎是所有实
时渲染器满足交互速率的前提
```

## 第 7 页：体积雾

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 7 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
         体积雾
         特征：形态，颜色，层次

         分布：小范围局部，地表建筑



         造型：流体特征的模拟，蒸汽类特效

         属性：弱非均质型介质

         渲染：单散射，但要考虑流体行为

         编辑：地形雾，遮挡用片状雾




现状：体积雾的渲染目前主要分为2种做法。一种是世界/
本地空间直接Raymarching，然后采用恒定常量/函数计
算/纹理采样三种方式表征密度，即一边步进采样一边执行
VRE积分，好处是直观方便，缺点是步进控制复杂；另外
一种将空间与数据通过Voxel组织起来，进行 Voxel
Tracing，即介质属性的搜集和积分计算为2个环节步骤，
好处是便于通过计算着色器并行计算和注入额外数据，缺
点是V-Buffer占用，渲染规模与体积阴影问题。目前大多
数引擎会选择第二种思路，具体仁者见仁智者见智
```

## 第 8 页：体积烟

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 8 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
          体积烟
          特征：形态，结构，颜色，层次

          分布：更小范围



          造型：强形态与结构塑造

          属性：弱非均质型介质

          渲染：单散射，但要考虑流体行为

          编辑：一般由粒子编辑器实现




现状：目前在渲染技术上并没有去严格区分雾与烟，但通
过美术对颜色，形态特征，出现场合的控制做到视觉上的
欺骗。但两者的介质属性并不相同，凝结核水滴和微小离
散介质颗粒导致了两种介质的光散射与吸收差别很大，在
形态与微结构塑造上也迥然不同。烟尘的实现在流体特征
还原上会更复杂，一般基于粒子编辑器机制和离线解算预
烘焙Flipbook技术去实现。
```

## 第 9 页：燃烧过程

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 9 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
           燃烧过程
           特征：形态，结构，颜色，层次

           分布：更小范围



           造型：强形态与结构塑造

           属性：弱非均质型介质

           渲染：单散射，但要考虑流体行为

           编辑：一般由粒子编辑器实现




现状：主流游戏技术依旧是Flipbook；另外一个是UE5的
Niagara Gas Fluid，多个Emitter分开执行烟，火焰的解
算与渲染，由System整合到一起；另外一种更激进的PC
端做法是渲染VDB序列，通过在Houdini或Embergen，
Blender中离线求解稀疏气态流体，保存为VDB序列，然
后导入引擎进行实时的渲染。目前来看Niagara主要为性
能限制，VDB因为渲染器只读的限制导致无法实现交互。
```

## 第 10 页：黑洞

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 10 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
        黑洞
        特征：事件视界，引力透镜，吸积盘

        分布：融入天空盒背景




        渲染：体积的，视觉导向的

        编辑：吸积盘调整




现状：现在让我们更卷一步吧，国外的一些技术美术大约
2022年开始小范围兴起一股渲染黑洞的热潮，国内目前来
看还鲜有类似的东西出现，所以这里应该提升下卷度，与
国际接轨。
```

## 第 11 页：上述方案大一统的可行性？

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 11 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
上述方案大一统的可行性？
● 现状：多类介质使用着各自不同的实现机制

● 共通性

  ●   都拥有相同的介质属性或其中部分：σ_s, σ_t, σ_a, σ_e, ρ

  ●   都需要进行VRE的积分过程


● 差异性

  ●   形态塑造的侧重点（介质属性表征方式，美术编辑手段）

  ●   散射阶数（影响了VRE的封装与算法实现）

  ●   边界问题（空间限定，边界流与边界条件）
```

## 第 12 页：解决介质表征手段：Shape Profile

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 12 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
解决介质表征手段：Shape Profile


  ● 可以是一段函数，一根曲线

  ● 可以是一张纹理，2D或3D   𝜎𝜎t   𝜎𝜎s = 𝜌𝜌 � 𝜎𝜎𝑡𝑡

  ● 可以是VDB
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
  ● 可以是一张纹理，2D或3D   𝜎𝜎t   𝜎𝜎s = 𝜌𝜌 � 𝜎𝜎𝑡𝑡
```

## 第 13 页：渲染器扩展与管线设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 13 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
渲染器扩展与管线设计
    Part2
```

## 第 14 页：C++ ? U++?

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 14 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
C++ ? U++?
●   对标准C++进行了大量扩展与定制
    ●   定义了自己的基础数据类型和高级数据结构
    ●   设计了多种GC机制
    ●   封装了反射系统
    ●   重新设计了智能指针
    ●   魔改了原子操作，提供线程安全的容器类型和锁
    ●   支持了Super


●   UE开发特点
    ●   基于原生C++自己造了大量其他语言的特性进来
    ●   大量使用参数宏
    ●   丰富的模板元编程
    ●   比原生C++更灵活，方便，也更消耗编译资源
```

## 第 15 页：如何在插件中向渲染器插入自定义管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 15 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何在插件中向渲染器插入自定义管线
通过SceneViewExtension进行SceneView扩展




按需覆写如下函数：
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
// SceneViewExtension 常见可覆写入口（根据第 15 页可见代码重建）
virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override;
virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override;
virtual void PrePostProcessPass_RenderThread(
    FRDGBuilder& GraphBuilder,
    const FSceneView& View,
    const FPostProcessingInputs& Inputs) override;
virtual void PostRenderBasePass_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override;
virtual void PostRenderBasePassMobile_RenderThread(
    FRHICommandListImmediate& RHICmdList,
    FSceneView& InView) override;
```

## 第 16 页：如何在插件中向渲染器插入自定义管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 16 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何在插件中向渲染器插入自定义管线
向Renderer模块注册渲染委托并绑定渲染主函数
                            class FPostOpaqueRenderParameters
                            {
                            public:
                               FIntRect ViewportRect;
                               FMatrix ViewMatrix;
                               FMatrix ProjMatrix;
                               FRDGTexture* ColorTexture = nullptr;
                               FRDGTexture* DepthTexture = nullptr;
                               FRDGTexture* NormalTexture = nullptr;
                               FRDGTexture* VelocityTexture = nullptr;
                               FRDGTexture* SmallDepthTexture = nullptr;
                               FRDGBuilder* GraphBuilder = nullptr;
                               FRHIUniformBuffer* ViewUniformBuffer = nullptr;
                               TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTexturesUniformParams = nullptr;
                               TRDGUniformBufferRef<FMobileSceneTextureUniformParameters> MobileSceneTexturesUniformParams = nullptr;
                               const FGlobalDistanceFieldParameterData* GlobalDistanceFieldParams = nullptr;
                               void* Uid = nullptr; // A unique identifier for the view.
                               const FViewInfo* View = nullptr;
                            };




                            DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostOpaqueRender, class FPostOpaqueRenderParameters&);
                            typedef FOnPostOpaqueRender::FDelegate FPostOpaqueRenderDelegate;




                            virtual FDelegateHandle RegisterPostOpaqueRenderDelegate(const FPostOpaqueRenderDelegate& PostOpaqueRenderDelegate) = 0;
                            virtual void RemovePostOpaqueRenderDelegate(FDelegateHandle PostOpaqueRenderDelegate) = 0;
                            virtual FDelegateHandle RegisterOverlayRenderDelegate(const FPostOpaqueRenderDelegate& OverlayRenderDelegate) = 0;
                            virtual void RemoveOverlayRenderDelegate(FDelegateHandle OverlayRenderDelegate) = 0;
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
// Renderer 委托结构（根据第 16 页可见代码与提取文本重建）
DECLARE_MULTICAST_DELEGATE_OneParam(
    FOnPostOpaqueRender,
    class FPostOpaqueRenderParameters&);

typedef FOnPostOpaqueRender::FDelegate FPostOpaqueRenderDelegate;

class FPostOpaqueRenderParameters
{
public:
    FIntRect ViewportRect;
    FMatrix ViewMatrix;
    FMatrix ProjMatrix;
    FRDGTexture* ColorTexture = nullptr;
    FRDGTexture* DepthTexture = nullptr;
    FRDGTexture* NormalTexture = nullptr;
    FRDGTexture* VelocityTexture = nullptr;
    FRDGTexture* SmallDepthTexture = nullptr;
    FRDGBuilder* GraphBuilder = nullptr;
    FRHIUniformBuffer* ViewUniformBuffer = nullptr;
};

// --- 自动提取的补充片段 ---
                            class FPostOpaqueRenderParameters
                               FIntRect ViewportRect;
                               FMatrix ViewMatrix;
                               FMatrix ProjMatrix;
                               FRDGTexture* ColorTexture = nullptr;
                               FRDGTexture* DepthTexture = nullptr;
                               FRDGTexture* NormalTexture = nullptr;
                               FRDGTexture* VelocityTexture = nullptr;
                               FRDGTexture* SmallDepthTexture = nullptr;
                               FRDGBuilder* GraphBuilder = nullptr;
                               FRHIUniformBuffer* ViewUniformBuffer = nullptr;
                               TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTexturesUniformParams = nullptr;
                               TRDGUniformBufferRef<FMobileSceneTextureUniformParameters> MobileSceneTexturesUniformParams = nullptr;
                               const FGlobalDistanceFieldParameterData* GlobalDistanceFieldParams = nullptr;
                               void* Uid = nullptr; // A unique identifier for the view.
                               const FViewInfo* View = nullptr;
                            DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostOpaqueRender, class FPostOpaqueRenderParameters&);
                            typedef FOnPostOpaqueRender::FDelegate FPostOpaqueRenderDelegate;
                            virtual FDelegateHandle RegisterPostOpaqueRenderDelegate(const FPostOpaqueRenderDelegate& PostOpaqueRenderDelegate) = 0;
                            virtual void RemovePostOpaqueRenderDelegate(FDelegateHandle PostOpaqueRenderDelegate) = 0;
                            virtual FDelegateHandle RegisterOverlayRenderDelegate(const FPostOpaqueRenderDelegate& OverlayRenderDelegate) = 0;
                            virtual void RemoveOverlayRenderDelegate(FDelegateHandle OverlayRenderDelegate) = 0;
```

## 第 17 页：扩展渲染器

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 17 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                             扩展渲染器
总体设计思路                  SingleVolumeContainer

                        SingleVolumeMaterialComponnet



                     SingleVolumeSceneProxy设计与渲染视
                     图扩展，代理会持有Extension的共享指
                     针，Extension持有Proxy的数组



               通过渲染委托的方式向Renderer模块注册和绑定
               主渲染函数，并返回对应的句柄



             主渲染函数内为标准两Pass方案，一道Main Pass用
             于逐个绘制单体积；一道全屏Pass用于混合颜色与深
             度到Back Buffer



         主Pass重新设计了MeshPassProcessor, AddMeshBatch
         与Process函数，通过模板化的方式实现不同参与介质渲染




                                 渲染器扩展整体设计思路
```

## 第 18 页：游戏线程

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 18 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
            游戏线程
            从具备基元渲染的Primitive Component开始

            重塑CreateSceneProxy的能力去new一个代理

            区分LoadMap和LevelStreaming 去AddPrimitive




      渲染线程
      Mesh Batch 封装（将Scene Proxy与最终的渲染结构
      分离）：覆写DrawStaticElements ，
      GetDynamicMeshElements;

      MeshPassProcessor（“网格管道处理器”）执行自
      身的AddMeshBatch方法，内部经过一系列处理生成
      对应Pass需要的MeshDrawCommand（一种更加紧凑，
      更加简洁，更有利于CPU访问的数据结构）



RHI 线程
包括一个抽象薄层与具体的硬件RHI层

接收来自渲染线程的绘制命令与数据指挥GPU渲染

                        如何绘制网格：Mesh Draw Pipeline
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
      GetDynamicMeshElements;
```

## 第 19 页：问题1：逻辑线程与渲染线程的数据分离，同步，更新怎么做的？

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 19 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
问题1：逻辑线程与渲染线程的数据分离，同步，更新怎么做的？



问题2：动态与静态绘制路径的区别， MeshBatch封装了什么内容？



问题3：MeshPassProcessor怎么由MeshBatch生成的DrawCommand？



问题4：MeshDrawCommand到底存了什么信息，数据？
```

## 第 20 页：单体积材料组件和容器设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 20 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
单体积材料组件和容器设计

               需要考虑清楚的点

               业务身份：定义的对象在整个游戏中扮演
               着什么类型的角色？

               特殊功能：我们希望设计哪些功能，它们
               与业务对象之间的关系

               UE组件和Unity组件的区别




               从何处派生，继承有什么类型的能力，需
               要覆写哪些函数，应当追加何种功能，持
               有哪些成员变量，生命周期与权限控制




                                 熟悉虚幻游戏框架
```

## 第 21 页：单体积材料组件和容器设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 21 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
单体积材料组件和容器设计
●   SingleVolumeMaterialComponent
    ●   用于参与介质材料的模拟：云，雾，干冰
    ●   派生自UPrimitiveComponent，以继承渲染和物理的能力
    ●   重塑渲染能力：最简网格，最简顶点工厂，接收UMaterialInterface，承载介质属性与渲染控制
    ●   将参数分为2类：渲染器设置与介质属性控制



●   SingleVolumeContainer
    ●   “容器”故名思意其本身不应当包含具体功能，而是负责承载与包装作为各项功能提供者的组件
    ●   因此应当派生自AActor（准确得说AInfo）
```

## 第 22 页：Material Component 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 22 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Material Component 的设计
渲染器设置                                                                             着色属性项

Data Setting：直接设置，Preset                                                          Sample Per Pixel： 原意是路径追踪模式下支持超采样（当前可忽略）


Volume Mode：云体积，雾体积，标准体积                                                          View Steps：主视图步进次数


Profile Mode：密度水平集，网格距离场，稀疏体积，材质图                                                 Shadow Steps：阴影步进次数


Detail Noise Mode：No Detail Added, FBM Detail Added, Billowy Wispy Detail Added   Density Multiplier：密度增益系数


Render After Translucent : 控制渲染在Translucent之前还是之后                                 Shadow Multiplier : 阴影密度缩放，会影响光吸收


                                                                                  Threshold：积分过程中断阈值，提前结束循环的策略
密度属性项

Shape Profile：接收Volume Texture或者Buffer                                            散射属性项
Volume Resolution：体积对象三维尺寸
                                                                                  Scatter Order：散射阶数，目前设计最大3阶散射

Scroll Displacement：每个轴向的细节运动方向
                                                                                  MS Scatter Contribution：多散射的散射系数倍率

Turbulence Intensity：湍流强度，控制细节运动强弱
                                                                                  MS Extinction Contribution：多散射消光系数倍率

Turbulence Scale : 细节信号的空间尺度
                                                                                  MS Phase Eccentricity：多散射相位系数倍率


自发辐射属性项
                                                                                  各项异性属性项
Emission Mode：Normal(常规自发光)，Blackbody(黑体模式)

                                                                                  Anisotropy Prime：控制前向散射
Emission Strength：自发光强度

                                                                                  Anisotropy Secondary：控制后项散射
Kelvin Temperature：开式温度

                                                                                  Weight：相位混合权重
Black Body Intensity：控制黑体辐射强度
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
Emission Mode：Normal(常规自发光)，Blackbody(黑体模式)
```

## 第 23 页：Material Component 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 23 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Material Component 的设计
使用功能修饰符与元数据让编辑器更好用

UCLASS                             UFUNCTION                         UPROPERTY

●   Blueprintable & BlueprinType   ●   BlueprintCallable             ●   Edit / Visible Anywhere
●   Class Group                    ●   BlueprintPure                 ●   Edit / Visible Default Only
●   Config                         ●   BlueprintImplementableEvent   ●   Edit / Visible Instance Only
●   Show & Hide Categories         ●   BlueprintNativeEvent          ●   BlueprintReadWrite
●   BlueprintSpawnbleComponent     ●   CallInEditor                  ●   BlueprintReadOnly
                                   ●   Category                      ●   Category
                                                                     ●   DisplayName (Meta)
USTRUCT / UENUM                    ●   DefaultToSelf (Meta)          ●   ToolTip (Meta)
                                   ●   DeprecatedFunction (Meta)     ●   EditCondition (Meta)
                                   ●   DeprecatedMessage (Meta)      ●   EditConditionHides (Meta)
●   BlueprintType
                                   ●   WorldContext (Meta)           ●   Clamp Min / Max (Meta)
                                   ●   AdvancedDisplay (Meta)        ●   UI Min / Max (Meta)
                                   ●   DisplayName (Meta)            ●   MultiLine (Meta)
                                   ●   HidePin (Meta)                ●   HideAlphaChannel (Meta)
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                                                                     ●   DisplayName (Meta)
USTRUCT / UENUM                    ●   DefaultToSelf (Meta)          ●   ToolTip (Meta)
                                   ●   DeprecatedFunction (Meta)     ●   EditCondition (Meta)
                                   ●   DeprecatedMessage (Meta)      ●   EditConditionHides (Meta)
                                   ●   WorldContext (Meta)           ●   Clamp Min / Max (Meta)
                                   ●   AdvancedDisplay (Meta)        ●   UI Min / Max (Meta)
                                   ●   DisplayName (Meta)            ●   MultiLine (Meta)
                                   ●   HidePin (Meta)                ●   HideAlphaChannel (Meta)
```

## 第 24 页：Material Component 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 24 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Material Component 的设计
覆写几个关键虚函数                                                                                                                                                             参数更改事件

FBoxSphereBounds USingleVolumeMaterialComponent::CalcBounds(const FTransform& LocalToWorld) const                                                                     #if WITH_EDITOR
{                                                                                                                                                                       void USingleVolumeMaterialComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
  FBoxSphereBounds NewBounds;                                                                                                                                           {
  NewBounds.Origin = FVector::ZeroVector;                                                                                                                                 FName PropertyName;
  NewBounds.BoxExtent = FVector(VolumeResolution) * 0.5;                                                                                                                  if (PropertyChangedEvent.Property)


                                                                                                                                         包围盒计算
  NewBounds.SphereRadius = NewBounds.BoxExtent.Length();                                                                                                                  {
  return NewBounds.TransformBy(LocalToWorld);                                                                                                                                PropertyName = PropertyChangedEvent.Property->GetFName();
}                                                                                                                                                                         }

                                                                                                                                                                           // *** 省略若干项 ***

                                                                                                                                                                           if (PropertyName == GET_MEMBER_NAME_CHECKED(USingleVolumeMaterialComponent, VolumeResolution))
                                          //~ Begin UActorComponent Interface                                                                                              {
                                          virtual void BeginPlay() override;                                                                                                  if (ProfileMode == EProfileMod::SparseVolumeTexture)
                                          virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;                                                            {
                                          virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;              VolumeResolution = ShapeProfileSVT->GetVolumeResolution();
                                          virtual void CreateRenderState_Concurrent(FRegisterComponentContext* Context) override;                                             }
                                          //~ End UActorComponent Interface                                                                                                }

                                          //~ Begin USceneComponent Interface                                                                                              if (PropertyName == GET_MEMBER_NAME_CHECKED(USingleVolumeMaterialComponent, DetailNoiseMode))
                                          virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;                                              {
                                          //~ End USceneComponent Interface                                                                                                   switch (DetailNoiseMode)
                                                                                                                                                                              {


    一些主要的函数
                                          //~ Begin UPrimitiveComponent Interface                                                                                               case ENoiseMod::Non_DetailAdded:
                                          virtual int32 GetNumMaterials() const override;                                                                                       {
                                          virtual void SetMaterial(int32 ElementIndex, UMaterialInterface* InMaterial) override;                                                  NoiseVT = nullptr;
                                          virtual UMaterialInterface* GetMaterial(int32 ElementIndex) const override {return Material;}                                           NoiseInjectMode = 0;
                                          virtual void GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials, bool bGetDebugMaterials) const override;                       break;
                                          virtual FPrimitiveSceneProxy* CreateSceneProxy() override;                                                                            }
                                          //~ End UPrimitiveComponent Interface                                                                                                 case ENoiseMod::FBM_DetailAdded:
                                                                                                                                                                                {
                                          #if WITH_EDITOR                                                                                                                         NoiseVT = LoadObject<UTexture>(nullptr, TEXT("/SingleVolume/Core/Misc/T_BalanceNoise_Volume.T_BalanceNoise_Volume'"));
                                            virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;                                            NoiseInjectMode = 1;
                                          #endif                                                                                                                                  break;
                                                                                                                                                                                }
                                                                                                                                                                                case ENoiseMod::Billowy_Wispy_DetailAdded:
                                                                                                                                                                                {


                                                                                                                                        创建渲染代理
                                                                                                                                                                                  NoiseVT = LoadObject<UTexture>(nullptr, TEXT("/SingleVolume/Core/Misc/T_AdaptiveNoise_Volume.T_ AdaptiveNoise_Volume'"));
FPrimitiveSceneProxy* USingleVolumeMaterialComponent::CreateSceneProxy()                                                                                                          NoiseInjectMode = 2;
{                                                                                                                                                                                 break;
  // 形状剖面是必须的                                                                                                                                                                   }
    if (!ShapeProfileVT && !ShapeProfileSVT)                                                                                                                                    default:
     return nullptr;                                                                                                                                                            {
                                                                                                                                                                                  NoiseVT = nullptr;
    // 需要为材质组件赋予一个材质以让渲染器正常工作                                                                                                                                                     NoiseInjectMode = 0;
       UMaterialInterface* SafeMaterial = GetMaterial(0);                                                                                                                         break;
    if (!SafeMaterial || SafeMaterial->GetMaterial()->MaterialDomain != MD_Volume)                                                                                              }
    {                                                                                                                                                                         }
       UE_LOG(LogSingleVolume, Warning, TEXT("SingleVolumeMaterialComponent %s needs a Volumetrics Material."), *GetName());                                               }
       return nullptr;
    }                                                                                                                                                                      Super::PostEditChangeProperty(PropertyChangedEvent);
                                                                                                                                                                       }
    return new FSingleVolumeSceneProxy(this);
}                                                                                                                                                                     #endif
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
FBoxSphereBounds USingleVolumeMaterialComponent::CalcBounds(const FTransform& LocalToWorld) const                                                                     #if WITH_EDITOR
{                                                                                                                                                                       void USingleVolumeMaterialComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
  FBoxSphereBounds NewBounds;                                                                                                                                           {
  NewBounds.Origin = FVector::ZeroVector;                                                                                                                                 FName PropertyName;
  NewBounds.BoxExtent = FVector(VolumeResolution) * 0.5;                                                                                                                  if (PropertyChangedEvent.Property)
  NewBounds.SphereRadius = NewBounds.BoxExtent.Length();                                                                                                                  {
  return NewBounds.TransformBy(LocalToWorld);                                                                                                                                PropertyName = PropertyChangedEvent.Property->GetFName();
}                                                                                                                                                                         }
                                                                                                                                                                           // *** 省略若干项 ***
                                                                                                                                                                           if (PropertyName == GET_MEMBER_NAME_CHECKED(USingleVolumeMaterialComponent, VolumeResolution))
                                          //~ Begin UActorComponent Interface                                                                                              {
                                          virtual void BeginPlay() override;                                                                                                  if (ProfileMode == EProfileMod::SparseVolumeTexture)
                                          virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;                                                            {
                                          virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;              VolumeResolution = ShapeProfileSVT->GetVolumeResolution();
                                          virtual void CreateRenderState_Concurrent(FRegisterComponentContext* Context) override;                                             }
                                          //~ End UActorComponent Interface                                                                                                }
                                          //~ Begin USceneComponent Interface                                                                                              if (PropertyName == GET_MEMBER_NAME_CHECKED(USingleVolumeMaterialComponent, DetailNoiseMode))
                                          virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;                                              {
                                          //~ End USceneComponent Interface                                                                                                   switch (DetailNoiseMode)
                                          //~ Begin UPrimitiveComponent Interface                                                                                               case ENoiseMod::Non_DetailAdded:
                                          virtual int32 GetNumMaterials() const override;                                                                                       {
                                          virtual void SetMaterial(int32 ElementIndex, UMaterialInterface* InMaterial) override;                                                  NoiseVT = nullptr;
                                          virtual UMaterialInterface* GetMaterial(int32 ElementIndex) const override {return Material;}                                           NoiseInjectMode = 0;
                                          virtual void GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials, bool bGetDebugMaterials) const override;                       break;
                                          virtual FPrimitiveSceneProxy* CreateSceneProxy() override;                                                                            }
                                          //~ End UPrimitiveComponent Interface                                                                                                 case ENoiseMod::FBM_DetailAdded:
                                          #if WITH_EDITOR                                                                                                                         NoiseVT = LoadObject<UTexture>(nullptr, TEXT("/SingleVolume/Core/Misc/T_BalanceNoise_Volume.T_BalanceNoise_Volume'"));
                                            virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;                                            NoiseInjectMode = 1;
                                          #endif                                                                                                                                  break;
                                                                                                                                                                                case ENoiseMod::Billowy_Wispy_DetailAdded:
                                                                                                                                                                                  NoiseVT = LoadObject<UTexture>(nullptr, TEXT("/SingleVolume/Core/Misc/T_AdaptiveNoise_Volume.T_ AdaptiveNoise_Volume'"));
FPrimitiveSceneProxy* USingleVolumeMaterialComponent::CreateSceneProxy()                                                                                                          NoiseInjectMode = 2;
{                                                                                                                                                                                 break;
  // 形状剖面是必须的                                                                                                                                                                   }
    if (!ShapeProfileVT && !ShapeProfileSVT)                                                                                                                                    default:
     return nullptr;                                                                                                                                                            {
                                                                                                                                                                                  NoiseVT = nullptr;
    // 需要为材质组件赋予一个材质以让渲染器正常工作                                                                                                                                                     NoiseInjectMode = 0;
       UMaterialInterface* SafeMaterial = GetMaterial(0);                                                                                                                         break;
    if (!SafeMaterial || SafeMaterial->GetMaterial()->MaterialDomain != MD_Volume)                                                                                              }
    {                                                                                                                                                                         }
       UE_LOG(LogSingleVolume, Warning, TEXT("SingleVolumeMaterialComponent %s needs a Volumetrics Material."), *GetName());                                               }
       return nullptr;
    }                                                                                                                                                                      Super::PostEditChangeProperty(PropertyChangedEvent);
    return new FSingleVolumeSceneProxy(this);
}                                                                                                                                                                     #endif
```

## 第 25 页：Material Component 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 25 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Material Component 的设计
Scene Proxy是游戏线程与渲染线程的分离的一个重要机制




通过对象的分离，操作数据的拷贝传递避免竞争


     Game Thread             Render Thread

        UWorld                   FScene

  UPrimitiveComponent   FPrimitiveSceneProxy/Info

           -             FSceneView / FViewInfo

     ULocalPlayer           FSceneViewState

   ULightComponent       FLightSceneProxy/Info
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
        UWorld                   FScene
  UPrimitiveComponent   FPrimitiveSceneProxy/Info
           -             FSceneView / FViewInfo
     ULocalPlayer           FSceneViewState
```

## 第 26 页：Material Component 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 26 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Material Component 的设计
游戏线程向渲染线程的数据更新


接口函数，通知渲染线程刷新数据
```

## 第 27 页：Scene Proxy 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 27 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Scene Proxy 的设计
一个最简的Scene Proxy 应当包含哪些内容？
class FSingleVolumeSceneProxy : public FPrimitiveSceneProxy
{
                                                                                                         构造函数：1）将Component属性Copy一份；2）重建程序化网格； 3）获取或创建渲染视图扩展
public:
  FSingleVolumeSceneProxy(const USingleVolumeMaterialComponent* InComponent);
  virtual ~FSingleVolumeSceneProxy();



                                                                                                         需要覆写的函数：
     //~ Begin Self Interface
     //~ ****
     //~ Begin Self Interface

     //~ Begin FPrimitiveSceneProxy Interface                                                            1）获取视图相关性
                                                                                                         2）动态Elements的搜集与Mesh Batch创建
     virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;


                                                                                                         3）获取Hash，保证该Scene Proxy的唯一性
     virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views,
       const FSceneViewFamily& ViewFamily,

                                                                                                         4）Scene Proxy的创建与销毁事件
       uint32 VisibilityMap,
       FMeshElementCollector& Collector) const override;

     virtual SIZE_T GetTypeHash() const override;                                                        5）内存占用记录
     virtual void CreateRenderThreadResources() override;
     virtual void DestroyRenderThreadResources() override;
     virtual uint32 GetMemoryFootprint() const override {return (sizeof(*this) + GetAllocatedSize());}

                                                                                                         渲染视图扩展的共享指针
     //~ End FPrimitiveSceneProxy Interface

private:
  // 渲染器扩展贡献指针

                                                                                                         需要的成员变量：
    TSharedPtr<class FSingleVolumeRendering, ESPMode::ThreadSafe> SingleVolumeRenderExtension;



                                                                                                         1）单体积网格数据：Vertex Buffer, Index Buffer, Num Primitives, Num Vertices
     // Basic
     FSingleVolumeMeshData SingleVolumeMeshData;
     FSingleVolumeMeshVertexFactory VertexFactory;
     UMaterialInterface* MaterialInterface = nullptr;
     FMaterialRelevance MaterialRelevance;                                                               2) 单体积网格的顶点工厂
     // RPI Settings                                                                                     3）Material在逻辑线程的载体
                                                                                                         4）FMaterialRelevance
     bool IsSDF;
     bool IsSVT;
     // ……

     // Participating media properties

                                                                                                         属性与管线设置类成员变量和对应的操作函数
     FIntVector4 RenderFlagsData;
     FVector4f RaymarchData;
     // ……

     // ViewInfo
     mutable TArray<const FSceneView*> VisibleViews;
     mutable TMap<const FSceneView*, FMeshBatch*> MeshBatchPerView;
};
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
class FSingleVolumeSceneProxy : public FPrimitiveSceneProxy
  FSingleVolumeSceneProxy(const USingleVolumeMaterialComponent* InComponent);
  virtual ~FSingleVolumeSceneProxy();
     //~ Begin Self Interface
     //~ ****
     //~ Begin Self Interface
     //~ Begin FPrimitiveSceneProxy Interface                                                            1）获取视图相关性
     virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;
     virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views,
       const FSceneViewFamily& ViewFamily,
       FMeshElementCollector& Collector) const override;
     virtual SIZE_T GetTypeHash() const override;                                                        5）内存占用记录
     virtual void CreateRenderThreadResources() override;
     virtual void DestroyRenderThreadResources() override;
     virtual uint32 GetMemoryFootprint() const override {return (sizeof(*this) + GetAllocatedSize());}
     //~ End FPrimitiveSceneProxy Interface
  // 渲染器扩展贡献指针
    TSharedPtr<class FSingleVolumeRendering, ESPMode::ThreadSafe> SingleVolumeRenderExtension;
     // Basic
     FSingleVolumeMeshData SingleVolumeMeshData;
     FSingleVolumeMeshVertexFactory VertexFactory;
     UMaterialInterface* MaterialInterface = nullptr;
     FMaterialRelevance MaterialRelevance;                                                               2) 单体积网格的顶点工厂
     // RPI Settings                                                                                     3）Material在逻辑线程的载体
     bool IsSDF;
     bool IsSVT;
     // ……
     // Participating media properties
     FIntVector4 RenderFlagsData;
     FVector4f RaymarchData;
     // ……
     // ViewInfo
     mutable TArray<const FSceneView*> VisibleViews;
     mutable TMap<const FSceneView*, FMeshBatch*> MeshBatchPerView;
```

## 第 28 页：Scene Proxy 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 28 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Scene Proxy 的设计
                                                                                                                                                                                                                                         如何定义网格
构造函数：数据拷贝，构造程序化网格，获取渲染视图扩展
                                                                                                          // 构造程序化网格
                                                                                                          void FSingleVolumeMeshData::BuildSingleVolumeMesh(FVector3f BboxMin, FVector3f BboxMax)
                                                                                                          {
                                                                                                             // 先定义一个数组容器，元素为Engine模块中的动态网格顶点，然后添加 8个顶点
                                                                                                               TArray<FDynamicMeshVertex> Vertices;
                                                                                                             // 定义背面
                                                                                                               Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMin.X, BboxMin.Y, BboxMin.Z)));
                                                                                                             Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMax.X, BboxMin.Y, BboxMin.Z)));
FSingleVolumeSceneProxy::FSingleVolumeSceneProxy(const USingleVolumeMaterialComponent* InComponent)          Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMin.X, BboxMax.Y, BboxMin.Z)));
  : FPrimitiveSceneProxy(InComponent)                                                                        Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMax.X, BboxMax.Y, BboxMin.Z)));
  , VertexFactory(GetScene().GetFeatureLevel())                                                              // 定义前面
  , MaterialInterface(InComponent->GetMaterial(0))                                                             Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMin.X, BboxMin.Y, BboxMax.Z)));
  , MaterialRelevance(MaterialInterface->GetRelevance_Concurrent(GetScene().GetFeatureLevel()))              Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMax.X, BboxMin.Y, BboxMax.Z)));
{                                                                                                            Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMin.X, BboxMax.Y, BboxMax.Z)));
  // 材料组件数据Copy                                                                                              Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMax.X, BboxMax.Y, BboxMax.Z)));
   // ……                                                                                                     // 以这8个顶点构建的Box初始化VertexBuffer
   // 材料组件数据Copy                                                                                             VertexBuffers.PositionVertexBuffer.Init(Vertices.Num());
                                                                                                             VertexBuffers.StaticMeshVertexBuffer.Init(Vertices.Num(), 1);
 // 构建单体积网格                                                                                                  VertexBuffers.ColorVertexBuffer.Init(Vertices.Num());
   SingleVolumeMeshData.BuildSingleVolumeMesh(                                                               // 轮询8个顶点，将顶点信息填充进VertexBuffer
FVector3f::ZeroVector - FVector3f(InComponent->VolumeResolution) * 0.5,                                      for (int32 i = 0; i < Vertices.Num(); i++)
FVector3f::ZeroVector + FVector3f(InComponent->VolumeResolution) * 0.5);                                     {
                                                                                                                const FDynamicMeshVertex& Vertex = Vertices[i];
    ENQUEUE_RENDER_COMMAND(FInitVolumeMesh)(
    [this](FRHICommandListImmediate& RHICmdList)                                                               VertexBuffers.PositionVertexBuffer.VertexPosition(i) = Vertex.Position;
    {                                                                                                          VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(i, FVector3f(Vertex.TangentX.ToFVector()), Vertex.GetTangentY(),
       SingleVolumeMeshData.InitResource(RHICmdList);                                                     FVector3f(Vertex.TangentZ.ToFVector()));
    });                                                                                                        VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(i, 0, Vertex.TextureCoordinate[0]);
                                                                                                               VertexBuffers.ColorVertexBuffer.VertexColor(i) = Vertex.Color;
    ENQUEUE_RENDER_COMMAND(FInitVolumeVertexFactory)(                                                       }
    [this](FRHICommandListImmediate& RHICmdList)                                                            // 定义6个面
    {                                                                                                         TArray<uint32> Indices{
       VertexFactory.Init(&SingleVolumeMeshData, RHICmdList);                                                  // bottom face
    });                                                                                                        0, 1, 2,
                                                                                                               1, 3, 2,
    SingleVolumeRenderExtension = FSingleVolumeRuntimeModule::GetRenderExtension();                            // right face
}                                                                                                              1, 5, 3,
                                                                                                               3, 5, 7,
                                                                                                               // front face
                                                                                                               3, 7, 6,
                                                                                                               2, 3, 6,
                                                                                                               // left face
                                                                                                               2, 4, 0,
                                                                                                               2, 6, 4,
        class FSingleVolumeMeshData : public FRenderResource                                                   // back face
                                                                                                               0, 4, 5,
        {
                                                                                                               1, 0, 5,
        public:                                                                                                // top face
          FStaticMeshVertexBuffers VertexBuffers;                                                              5, 4, 6,
          FRawStaticIndexBuffer IndexBuffer;                                                                   5, 6, 7 };
                                                                                                            // 存入IndexBuffer
          uint32 NumPrimitives;
                                                                                                            IndexBuffer.SetIndices(Indices, EIndexBufferStride::Force16Bit);
          uint32 NumVertices;                                                                               // 三个面为一组构成一个Primitives，共用 8个顶点
                                                                                                              NumPrimitives = Indices.Num() / 3;
             FSingleVolumeMeshData();                                                                       NumVertices = Vertices.Num();
                                                                                                          }

             void BuildSingleVolumeMesh(FVector3f BboxMin, FVector3f BboxMax);

             // 我们希望覆写父类中InitResource
             virtual void InitResource(FRHICommandListBase& RHICmdList) override;
                                                                                                          void FSingleVolumeMeshData::InitResource(FRHICommandListBase& RHICmdList)
                                                                                                          {                                                                                                            初始化渲染资源
                                                                                                  网格数据类
                                                                                                            VertexBuffers.PositionVertexBuffer.InitResource(RHICmdList);
             virtual void ReleaseResource() override;                                                       VertexBuffers.StaticMeshVertexBuffer.InitResource(RHICmdList);
        };                                                                                                  VertexBuffers.ColorVertexBuffer.InitResource(RHICmdList);
                                                                                                            IndexBuffer.InitResource(RHICmdList);
                                                                                                          }
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                                                                                                          // 构造程序化网格
                                                                                                          void FSingleVolumeMeshData::BuildSingleVolumeMesh(FVector3f BboxMin, FVector3f BboxMax)
                                                                                                             // 先定义一个数组容器，元素为Engine模块中的动态网格顶点，然后添加 8个顶点
                                                                                                               TArray<FDynamicMeshVertex> Vertices;
                                                                                                             // 定义背面
                                                                                                               Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMin.X, BboxMin.Y, BboxMin.Z)));
                                                                                                             Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMax.X, BboxMin.Y, BboxMin.Z)));
FSingleVolumeSceneProxy::FSingleVolumeSceneProxy(const USingleVolumeMaterialComponent* InComponent)          Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMin.X, BboxMax.Y, BboxMin.Z)));
  : FPrimitiveSceneProxy(InComponent)                                                                        Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMax.X, BboxMax.Y, BboxMin.Z)));
  , VertexFactory(GetScene().GetFeatureLevel())                                                              // 定义前面
  , MaterialInterface(InComponent->GetMaterial(0))                                                             Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMin.X, BboxMin.Y, BboxMax.Z)));
  , MaterialRelevance(MaterialInterface->GetRelevance_Concurrent(GetScene().GetFeatureLevel()))              Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMax.X, BboxMin.Y, BboxMax.Z)));
{                                                                                                            Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMin.X, BboxMax.Y, BboxMax.Z)));
  // 材料组件数据Copy                                                                                              Vertices.Add(FDynamicMeshVertex(FVector3f(BboxMax.X, BboxMax.Y, BboxMax.Z)));
   // ……                                                                                                     // 以这8个顶点构建的Box初始化VertexBuffer
   // 材料组件数据Copy                                                                                             VertexBuffers.PositionVertexBuffer.Init(Vertices.Num());
                                                                                                             VertexBuffers.StaticMeshVertexBuffer.Init(Vertices.Num(), 1);
 // 构建单体积网格                                                                                                  VertexBuffers.ColorVertexBuffer.Init(Vertices.Num());
   SingleVolumeMeshData.BuildSingleVolumeMesh(                                                               // 轮询8个顶点，将顶点信息填充进VertexBuffer
FVector3f::ZeroVector - FVector3f(InComponent->VolumeResolution) * 0.5,                                      for (int32 i = 0; i < Vertices.Num(); i++)
FVector3f::ZeroVector + FVector3f(InComponent->VolumeResolution) * 0.5);                                     {
                                                                                                                const FDynamicMeshVertex& Vertex = Vertices[i];
    ENQUEUE_RENDER_COMMAND(FInitVolumeMesh)(
    [this](FRHICommandListImmediate& RHICmdList)                                                               VertexBuffers.PositionVertexBuffer.VertexPosition(i) = Vertex.Position;
    {                                                                                                          VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(i, FVector3f(Vertex.TangentX.ToFVector()), Vertex.GetTangentY(),
       SingleVolumeMeshData.InitResource(RHICmdList);                                                     FVector3f(Vertex.TangentZ.ToFVector()));
    });                                                                                                        VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(i, 0, Vertex.TextureCoordinate[0]);
                                                                                                               VertexBuffers.ColorVertexBuffer.VertexColor(i) = Vertex.Color;
    ENQUEUE_RENDER_COMMAND(FInitVolumeVertexFactory)(                                                       }
    [this](FRHICommandListImmediate& RHICmdList)                                                            // 定义6个面
    {                                                                                                         TArray<uint32> Indices{
       VertexFactory.Init(&SingleVolumeMeshData, RHICmdList);                                                  // bottom face
    });                                                                                                        0, 1, 2,
    SingleVolumeRenderExtension = FSingleVolumeRuntimeModule::GetRenderExtension();                            // right face
}                                                                                                              1, 5, 3,
                                                                                                               // front face
                                                                                                               // left face
        class FSingleVolumeMeshData : public FRenderResource                                                   // back face
        public:                                                                                                // top face
          FStaticMeshVertexBuffers VertexBuffers;                                                              5, 4, 6,
          FRawStaticIndexBuffer IndexBuffer;                                                                   5, 6, 7 };
                                                                                                            // 存入IndexBuffer
          uint32 NumPrimitives;
                                                                                                            IndexBuffer.SetIndices(Indices, EIndexBufferStride::Force16Bit);
          uint32 NumVertices;                                                                               // 三个面为一组构成一个Primitives，共用 8个顶点
                                                                                                              NumPrimitives = Indices.Num() / 3;
             FSingleVolumeMeshData();                                                                       NumVertices = Vertices.Num();
             void BuildSingleVolumeMesh(FVector3f BboxMin, FVector3f BboxMax);
             // 我们希望覆写父类中InitResource
             virtual void InitResource(FRHICommandListBase& RHICmdList) override;
                                                                                                          void FSingleVolumeMeshData::InitResource(FRHICommandListBase& RHICmdList)
                                                                                                          {                                                                                                            初始化渲染资源
                                                                                                            VertexBuffers.PositionVertexBuffer.InitResource(RHICmdList);
             virtual void ReleaseResource() override;                                                       VertexBuffers.StaticMeshVertexBuffer.InitResource(RHICmdList);
        };                                                                                                  VertexBuffers.ColorVertexBuffer.InitResource(RHICmdList);
                                                                                                            IndexBuffer.InitResource(RHICmdList);
```

## 第 29 页：Scene Proxy 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 29 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Scene Proxy 的设计
设计顶点工厂

                                                                                                      // 使用自定义的VertexBuffer初始化顶点工厂
class FSingleVolumeMeshVertexFactory : public FLocalVertexFactory                                     // 因为我们重写了顶点工厂，所以无法使用"FStaticMeshVertexBuffers.InitFromDynamicVertex"来方便的构建顶点工厂
{                                                                                                     void FSingleVolumeMeshVertexFactory::Init(
                                                                                                         FSingleVolumeMeshData* InVertexBuffer, FRHICommandListBase& RHICmdList)
 DECLARE_VERTEX_FACTORY_TYPE(FSingleVolumeMeshVertexFactory);                                         {
                                                                                                         VertexBuffer = InVertexBuffer;
 typedef FLocalVertexFactory Super;                                                                      VertexBuffer->UpdateRHI(RHICmdList);

public:                                                                                                   // 初始化顶点工厂资源
                                                                                                            {
 FSingleVolumeMeshVertexFactory(ERHIFeatureLevel::Type InFeatureLevel)                                       // VertexBufferAttributeStream，一次设置位置，发现，UV和顶点颜色属性到各自缓冲
   : Super(InFeatureLevel, "FSingleVolumeMeshVertexFactory")                                                FDataType VertexData;
 {                                                                                                           FStaticMeshVertexBuffers& Buffers = VertexBuffer->VertexBuffers;
   VertexBuffer = nullptr;                                                                                   Buffers.PositionVertexBuffer.BindPositionVertexBuffer(this, VertexData);
 }                                                                                                           Buffers.StaticMeshVertexBuffer.BindTangentVertexBuffer(this, VertexData);
                                                                                                             Buffers.StaticMeshVertexBuffer.BindPackedTexCoordVertexBuffer(this, VertexData);
 ~FSingleVolumeMeshVertexFactory()                                                                           Buffers.StaticMeshVertexBuffer.BindLightMapVertexBuffer(this, VertexData, 0);
 {                                                                                                           Buffers.ColorVertexBuffer.BindColorVertexBuffer(this, VertexData);
   ReleaseResource();
 }                                                                                                            SetData(VertexData);

 void Init(FSingleVolumeMeshData* InVertexBuffer, FRHICommandListBase& RHICmdList);                           InitResource(RHICmdList);
                                                                                                          }
 bool HasIncompatibleFeatureLevel(ERHIFeatureLevel::Type InFeatureLevel)                              }
 {
   return InFeatureLevel != GetFeatureLevel();
 }

 static bool ShouldCompilePermutation(const FVertexFactoryShaderPermutationParameters& Parameters)
 {
   bool Cond =                                                                                        IMPLEMENT_VERTEX_FACTORY_TYPE(FSingleVolumeMeshVertexFactory, "/Engine/Private/LocalVertexFactory.ush",
     Super::ShouldCompilePermutation(Parameters) &&                                                      EVertexFactoryFlags::UsedWithMaterials |
     IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5) &&                              EVertexFactoryFlags::SupportsDynamicLighting |
     Parameters.MaterialParameters.MaterialDomain == MD_Volume &&                                        EVertexFactoryFlags::SupportsPositionOnly
     IsPCPlatform(Parameters.Platform);                                                               );

     return Cond || Parameters.MaterialParameters.bIsSpecialEngineMaterial;
 }


                                                                                                     基于 FLocalVertexFactory 还是 FVertexFactpry 去派生？
private:
   FSingleVolumeMeshData* VertexBuffer;
};
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                                                                                                      // 使用自定义的VertexBuffer初始化顶点工厂
class FSingleVolumeMeshVertexFactory : public FLocalVertexFactory                                     // 因为我们重写了顶点工厂，所以无法使用"FStaticMeshVertexBuffers.InitFromDynamicVertex"来方便的构建顶点工厂
{                                                                                                     void FSingleVolumeMeshVertexFactory::Init(
                                                                                                         FSingleVolumeMeshData* InVertexBuffer, FRHICommandListBase& RHICmdList)
 DECLARE_VERTEX_FACTORY_TYPE(FSingleVolumeMeshVertexFactory);                                         {
                                                                                                         VertexBuffer = InVertexBuffer;
 typedef FLocalVertexFactory Super;                                                                      VertexBuffer->UpdateRHI(RHICmdList);
public:                                                                                                   // 初始化顶点工厂资源
 FSingleVolumeMeshVertexFactory(ERHIFeatureLevel::Type InFeatureLevel)                                       // VertexBufferAttributeStream，一次设置位置，发现，UV和顶点颜色属性到各自缓冲
   : Super(InFeatureLevel, "FSingleVolumeMeshVertexFactory")                                                FDataType VertexData;
 {                                                                                                           FStaticMeshVertexBuffers& Buffers = VertexBuffer->VertexBuffers;
   VertexBuffer = nullptr;                                                                                   Buffers.PositionVertexBuffer.BindPositionVertexBuffer(this, VertexData);
 }                                                                                                           Buffers.StaticMeshVertexBuffer.BindTangentVertexBuffer(this, VertexData);
                                                                                                             Buffers.StaticMeshVertexBuffer.BindPackedTexCoordVertexBuffer(this, VertexData);
 ~FSingleVolumeMeshVertexFactory()                                                                           Buffers.StaticMeshVertexBuffer.BindLightMapVertexBuffer(this, VertexData, 0);
 {                                                                                                           Buffers.ColorVertexBuffer.BindColorVertexBuffer(this, VertexData);
   ReleaseResource();
 }                                                                                                            SetData(VertexData);
 void Init(FSingleVolumeMeshData* InVertexBuffer, FRHICommandListBase& RHICmdList);                           InitResource(RHICmdList);
 bool HasIncompatibleFeatureLevel(ERHIFeatureLevel::Type InFeatureLevel)                              }
   return InFeatureLevel != GetFeatureLevel();
 static bool ShouldCompilePermutation(const FVertexFactoryShaderPermutationParameters& Parameters)
   bool Cond =                                                                                        IMPLEMENT_VERTEX_FACTORY_TYPE(FSingleVolumeMeshVertexFactory, "/Engine/Private/LocalVertexFactory.ush",
     Super::ShouldCompilePermutation(Parameters) &&                                                      EVertexFactoryFlags::UsedWithMaterials |
     IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5) &&                              EVertexFactoryFlags::SupportsDynamicLighting |
     Parameters.MaterialParameters.MaterialDomain == MD_Volume &&                                        EVertexFactoryFlags::SupportsPositionOnly
     IsPCPlatform(Parameters.Platform);                                                               );
     return Cond || Parameters.MaterialParameters.bIsSpecialEngineMaterial;
   FSingleVolumeMeshData* VertexBuffer;
```

## 第 30 页：Scene Proxy 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 30 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Scene Proxy 的设计
如果有需要可以为顶点工厂绑定额外的参数（逐Elements）

// 这里本应该从 FLocalVertexFactoryShaderParametersBase 派生，但它没有公开！只能从其基类直接派生                         void FSingleVolumeMeshVertexFactoryShaderParameters::GetElementShaderBindings(
class FSingleVolumeMeshVertexFactoryShaderParameters : public FVertexFactoryShaderParameters     const FSceneInterface* Scene,
{                                                                                                const FSceneView* View,
   DECLARE_TYPE_LAYOUT(FSingleVolumeMeshVertexFactoryShaderParameters, NonVirtual);              const FMeshMaterialShader* Shader,
public:                                                                                          const EVertexInputStreamType InputStreamType,
   // 暂时不需要绑定逐Element(Instance)的参数，虚位以待                                                          ERHIFeatureLevel::Type FeatureLevel,
     // 如果你不确定是否应该强制绑定，可以使用额外的修饰符“SPF_Optional”将该项设为可选绑定，以避免一些反射错误                               const FVertexFactory* VertexFactory,
     void Bind(const FShaderParameterMap& ParameterMap){};                                       const FMeshBatchElement& BatchElement,
                                                                                                 FMeshDrawSingleShaderBindings& ShaderBindings,
   // 逐Element传值                                                                                 FVertexInputStreamArray& VertexInputStreams) const
     void GetElementShaderBindings(                                                            {
      const class FSceneInterface* Scene,                                                        // 加一个断言，判断形参中的顶点工厂和我们新设计的顶点工厂是否为同一类
      const FSceneView* View,                                                                      check(VertexFactory->GetType() == &FSingleVolumeMeshVertexFactory::StaticType);
      const FMeshMaterialShader* Shader,
      const EVertexInputStreamType InputStreamType,                                                // 数据需要在SceneProxy的GetDynamicMeshElemnets中写入！
      ERHIFeatureLevel::Type FeatureLevel,                                                           FRHIUniformBuffer* VertexFactoryUniformBuffer = static_cast<FRHIUniformBuffer*>(BatchElement.VertexFactoryUserData);
      const FVertexFactory* VertexFactory,
      const FMeshBatchElement& BatchElement,                                                       /**
      FMeshDrawSingleShaderBindings& ShaderBindings,                                                * FLocalVertexFactoryShaderParametersBase::GetElementShaderBindingsBase没有暴露出来, 因此我们需要自己实现这部分缺失的功能
      FVertexInputStreamArray& VertexInputStreams                                                   * 目前来看，不需要OverrideColorVertexBuffer和SpeedTree这两个功能，因此只需要单独添加第一个
      ) const;                                                                                      * 即在支持ManualVertexFetch或GPUScene时，将VetexFactoryUnifomBuffer传给FLocalVertexFactoryUniformShaderParameters
private:                                                                                            */
   // 如果需要绑定额外的参数，在这里声明:LAYOUT_FIELD()                                                             const auto* LocalVertexFactory = static_cast<const FLocalVertexFactory*>(VertexFactory);
};                                                                                                 if (LocalVertexFactory->SupportsManualVertexFetch(FeatureLevel) || UseGPUScene(GMaxRHIShaderPlatform, FeatureLevel))
                                                                                                   {
                                                                                                      ShaderBindings.Add(Shader->GetUniformBufferParameter<FLocalVertexFactoryUniformShaderParameters>(), VertexFactoryUniformBuffer);
                                                                                                   }
// 实现我们在着色器类中声明的布局
IMPLEMENT_TYPE_LAYOUT(FSingleVolumeMeshVertexFactoryShaderParameters);                             // 如果需要为顶点工厂类绑定新参数，请在这里添加:
// 将着色器类绑定到顶点工厂，并声明着色器频率                                                                           //
IMPLEMENT_VERTEX_FACTORY_PARAMETER_TYPE(FSingleVolumeMeshVertexFactory, SF_Vertex,             }
FSingleVolumeMeshVertexFactoryShaderParameters);
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
// 这里本应该从 FLocalVertexFactoryShaderParametersBase 派生，但它没有公开！只能从其基类直接派生                         void FSingleVolumeMeshVertexFactoryShaderParameters::GetElementShaderBindings(
class FSingleVolumeMeshVertexFactoryShaderParameters : public FVertexFactoryShaderParameters     const FSceneInterface* Scene,
{                                                                                                const FSceneView* View,
   DECLARE_TYPE_LAYOUT(FSingleVolumeMeshVertexFactoryShaderParameters, NonVirtual);              const FMeshMaterialShader* Shader,
public:                                                                                          const EVertexInputStreamType InputStreamType,
   // 暂时不需要绑定逐Element(Instance)的参数，虚位以待                                                          ERHIFeatureLevel::Type FeatureLevel,
     // 如果你不确定是否应该强制绑定，可以使用额外的修饰符“SPF_Optional”将该项设为可选绑定，以避免一些反射错误                               const FVertexFactory* VertexFactory,
     void Bind(const FShaderParameterMap& ParameterMap){};                                       const FMeshBatchElement& BatchElement,
                                                                                                 FMeshDrawSingleShaderBindings& ShaderBindings,
   // 逐Element传值                                                                                 FVertexInputStreamArray& VertexInputStreams) const
     void GetElementShaderBindings(                                                            {
      const class FSceneInterface* Scene,                                                        // 加一个断言，判断形参中的顶点工厂和我们新设计的顶点工厂是否为同一类
      const FSceneView* View,                                                                      check(VertexFactory->GetType() == &FSingleVolumeMeshVertexFactory::StaticType);
      const FMeshMaterialShader* Shader,
      const EVertexInputStreamType InputStreamType,                                                // 数据需要在SceneProxy的GetDynamicMeshElemnets中写入！
      ERHIFeatureLevel::Type FeatureLevel,                                                           FRHIUniformBuffer* VertexFactoryUniformBuffer = static_cast<FRHIUniformBuffer*>(BatchElement.VertexFactoryUserData);
      const FVertexFactory* VertexFactory,
      const FMeshBatchElement& BatchElement,                                                       /**
      FMeshDrawSingleShaderBindings& ShaderBindings,                                                * FLocalVertexFactoryShaderParametersBase::GetElementShaderBindingsBase没有暴露出来, 因此我们需要自己实现这部分缺失的功能
      ) const;                                                                                      * 即在支持ManualVertexFetch或GPUScene时，将VetexFactoryUnifomBuffer传给FLocalVertexFactoryUniformShaderParameters
   // 如果需要绑定额外的参数，在这里声明:LAYOUT_FIELD()                                                             const auto* LocalVertexFactory = static_cast<const FLocalVertexFactory*>(VertexFactory);
};                                                                                                 if (LocalVertexFactory->SupportsManualVertexFetch(FeatureLevel) || UseGPUScene(GMaxRHIShaderPlatform, FeatureLevel))
                                                                                                      ShaderBindings.Add(Shader->GetUniformBufferParameter<FLocalVertexFactoryUniformShaderParameters>(), VertexFactoryUniformBuffer);
// 实现我们在着色器类中声明的布局
IMPLEMENT_TYPE_LAYOUT(FSingleVolumeMeshVertexFactoryShaderParameters);                             // 如果需要为顶点工厂类绑定新参数，请在这里添加:
// 将着色器类绑定到顶点工厂，并声明着色器频率                                                                           //
IMPLEMENT_VERTEX_FACTORY_PARAMETER_TYPE(FSingleVolumeMeshVertexFactory, SF_Vertex,             }
FSingleVolumeMeshVertexFactoryShaderParameters);
```

## 第 31 页：Scene Proxy 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 31 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Scene Proxy 的设计
理解 Primitive View Relevance

FPrimitiveViewRelevance FSingleVolumeSceneProxy::GetViewRelevance(const FSceneView* View) const
{
  FPrimitiveViewRelevance Result;

    Result.bDrawRelevance = IsShown(View);
    Result.bShadowRelevance = IsShadowCast(View) && ShouldRenderInMainPass();
    Result.bDynamicRelevance = true;
    Result.bStaticRelevance = false;
    Result.bRenderInMainPass = ShouldRenderInMainPass();
    Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
    Result.bRenderCustomDepth = ShouldRenderCustomDepth();
    Result.bTranslucentSelfShadow = bCastVolumetricTranslucentShadow;
    MaterialRelevance.SetPrimitiveViewRelevance(Result);
    Result.bVelocityRelevance = DrawsVelocity() && Result.bOpaque && Result.bRenderInMainPass;

    return Result;
}



覆写视图相关性函数，获取基元的视图相关性
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
FPrimitiveViewRelevance FSingleVolumeSceneProxy::GetViewRelevance(const FSceneView* View) const
  FPrimitiveViewRelevance Result;
    Result.bDrawRelevance = IsShown(View);
    Result.bShadowRelevance = IsShadowCast(View) && ShouldRenderInMainPass();
    Result.bDynamicRelevance = true;
    Result.bStaticRelevance = false;
    Result.bRenderInMainPass = ShouldRenderInMainPass();
    Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
    Result.bRenderCustomDepth = ShouldRenderCustomDepth();
    Result.bTranslucentSelfShadow = bCastVolumetricTranslucentShadow;
    MaterialRelevance.SetPrimitiveViewRelevance(Result);
    Result.bVelocityRelevance = DrawsVelocity() && Result.bOpaque && Result.bRenderInMainPass;
    return Result;
```

## 第 32 页：Scene Proxy 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 32 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Scene Proxy 的设计
覆写GetDynamicMeshElements, 实现单体积网格搜集与Mesh Batch的添加

void FSingleVolumeSceneProxy::GetDynamicMeshElements(                                                                                FMeshBatch* FSingleVolumeSceneProxy::CreateMeshBatch(const FSceneView* View, int32 ViewIndex,
  const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily,                                                          const FSceneViewFamily& ViewFamily,
  uint32 VisibilityMap, FMeshElementCollector& Collector) const                                                                        FMeshElementCollector& Collector,
{                                                                                                                                      const FMaterialRenderProxy* MaterialRenderProxy) const
  SCOPE_CYCLE_COUNTER(STAT_VolumeSceneProxy_GetDynamicMeshElements);                                                                 {
  check(IsInRenderingThread());                                                                                                        // 获取视图相关性
                                                                                                                                         const FPrimitiveViewRelevance& ViewRelevance = GetViewRelevance(View);
    if (!MaterialInterface || MaterialInterface->GetMaterial()->MaterialDomain != MD_Volume)
       return;                                                                                                                           // Set up MeshBatch
                                                                                                                                         FMeshBatch& MeshBatch = Collector.AllocateMesh();
    for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
    {                                                                                                                                    MeshBatch.bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;
      const FSceneView* View = Views[ViewIndex];                                                                                         MeshBatch.bUseWireframeSelectionColoring = IsSelected();
                                                                                                                                         MeshBatch.VertexFactory = &VertexFactory;
        // && SingleVolumeRenderExtension->ShouldRenderSingleVolume()                                                                    MeshBatch.MaterialRenderProxy = MaterialRenderProxy;
        if (IsShown(View) && (VisibilityMap & (1<<ViewIndex)))                                                                           MeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative() ? true : false;
        {                                                                                                                                MeshBatch.Type = PT_TriangleList;
           VisibleViews.Add(View);                                                                                                       MeshBatch.DepthPriorityGroup = SDPG_World;
                                                                                                                                         MeshBatch.bCanApplyViewModeOverrides = true;
            FMeshBatch* VolumeMesh = CreateMeshBatch(View, ViewIndex, ViewFamily, Collector, MaterialInterface->GetRenderProxy());       MeshBatch.bUseForMaterial = true;
                                                                                                                                         MeshBatch.CastShadow = ViewRelevance.bShadowRelevance;
            MeshBatchPerView.Add(View, VolumeMesh);                                                                                      MeshBatch.bUseForDepthPass = false;

            // 渲染包围盒                                                                                                                     // Set up MeshBatchElement
            {                                                                                                                            FMeshBatchElement& BatchElement = MeshBatch.Elements[0];
               FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
               RenderBounds(PDI, ViewFamily.EngineShowFlags, GetBounds(), IsSelected());                                                 BatchElement.PrimitiveUniformBuffer = GetUniformBuffer();
            }                                                                                                                            BatchElement.IndexBuffer = &SingleVolumeMeshData.IndexBuffer;
        }                                                                                                                                BatchElement.FirstIndex = 0;
    }                                                                                                                                    BatchElement.MinVertexIndex = 0;
}                                                                                                                                        BatchElement.MaxVertexIndex = SingleVolumeMeshData.NumVertices -1;
                                                                                                                                         BatchElement.NumPrimitives = SingleVolumeMeshData.NumPrimitives;
                                                                                                                                         BatchElement.VertexFactoryUserData = VertexFactory.GetUniformBuffer();

                                                                                                                                         // 可以使用 User Data 传递一些逐Element的数据
                                                                                                                                          // BatchElement.UserData = nullptr;

                                                                                                                                         Collector.AddMesh(ViewIndex, MeshBatch);

                                                                                                                                         return &MeshBatch;
                                                                                                                                     }
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
void FSingleVolumeSceneProxy::GetDynamicMeshElements(                                                                                FMeshBatch* FSingleVolumeSceneProxy::CreateMeshBatch(const FSceneView* View, int32 ViewIndex,
  const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily,                                                          const FSceneViewFamily& ViewFamily,
  uint32 VisibilityMap, FMeshElementCollector& Collector) const                                                                        FMeshElementCollector& Collector,
{                                                                                                                                      const FMaterialRenderProxy* MaterialRenderProxy) const
  SCOPE_CYCLE_COUNTER(STAT_VolumeSceneProxy_GetDynamicMeshElements);                                                                 {
  check(IsInRenderingThread());                                                                                                        // 获取视图相关性
                                                                                                                                         const FPrimitiveViewRelevance& ViewRelevance = GetViewRelevance(View);
    if (!MaterialInterface || MaterialInterface->GetMaterial()->MaterialDomain != MD_Volume)
       return;                                                                                                                           // Set up MeshBatch
                                                                                                                                         FMeshBatch& MeshBatch = Collector.AllocateMesh();
    for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
    {                                                                                                                                    MeshBatch.bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;
      const FSceneView* View = Views[ViewIndex];                                                                                         MeshBatch.bUseWireframeSelectionColoring = IsSelected();
                                                                                                                                         MeshBatch.VertexFactory = &VertexFactory;
        // && SingleVolumeRenderExtension->ShouldRenderSingleVolume()                                                                    MeshBatch.MaterialRenderProxy = MaterialRenderProxy;
        if (IsShown(View) && (VisibilityMap & (1<<ViewIndex)))                                                                           MeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative() ? true : false;
        {                                                                                                                                MeshBatch.Type = PT_TriangleList;
           VisibleViews.Add(View);                                                                                                       MeshBatch.DepthPriorityGroup = SDPG_World;
                                                                                                                                         MeshBatch.bCanApplyViewModeOverrides = true;
            FMeshBatch* VolumeMesh = CreateMeshBatch(View, ViewIndex, ViewFamily, Collector, MaterialInterface->GetRenderProxy());       MeshBatch.bUseForMaterial = true;
                                                                                                                                         MeshBatch.CastShadow = ViewRelevance.bShadowRelevance;
            MeshBatchPerView.Add(View, VolumeMesh);                                                                                      MeshBatch.bUseForDepthPass = false;
            // 渲染包围盒                                                                                                                     // Set up MeshBatchElement
            {                                                                                                                            FMeshBatchElement& BatchElement = MeshBatch.Elements[0];
               FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
               RenderBounds(PDI, ViewFamily.EngineShowFlags, GetBounds(), IsSelected());                                                 BatchElement.PrimitiveUniformBuffer = GetUniformBuffer();
            }                                                                                                                            BatchElement.IndexBuffer = &SingleVolumeMeshData.IndexBuffer;
        }                                                                                                                                BatchElement.FirstIndex = 0;
    }                                                                                                                                    BatchElement.MinVertexIndex = 0;
}                                                                                                                                        BatchElement.MaxVertexIndex = SingleVolumeMeshData.NumVertices -1;
                                                                                                                                         BatchElement.NumPrimitives = SingleVolumeMeshData.NumPrimitives;
                                                                                                                                         BatchElement.VertexFactoryUserData = VertexFactory.GetUniformBuffer();
                                                                                                                                         // 可以使用 User Data 传递一些逐Element的数据
                                                                                                                                          // BatchElement.UserData = nullptr;
                                                                                                                                         Collector.AddMesh(ViewIndex, MeshBatch);
                                                                                                                                         return &MeshBatch;
```

## 第 33 页：Scene Proxy 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 33 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Scene Proxy 的设计
理解 Element的搜集与 Mesh Batch
FMeshBatch* FSingleVolumeSceneProxy::CreateMeshBatch(const FSceneView* View, int32 ViewIndex,
  const FSceneViewFamily& ViewFamily,
  FMeshElementCollector& Collector,
  const FMaterialRenderProxy* MaterialRenderProxy) const
{
  // 获取视图相关性
    const FPrimitiveViewRelevance& ViewRelevance = GetViewRelevance(View);

    // Set up MeshBatch
    FMeshBatch& MeshBatch = Collector.AllocateMesh();

    MeshBatch.bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;
    MeshBatch.bUseWireframeSelectionColoring = IsSelected();
    MeshBatch.VertexFactory = &VertexFactory;
    MeshBatch.MaterialRenderProxy = MaterialRenderProxy;
    MeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative() ? true : false;
    MeshBatch.Type = PT_TriangleList;
    MeshBatch.DepthPriorityGroup = SDPG_World;
    MeshBatch.bCanApplyViewModeOverrides = true;
    MeshBatch.bUseForMaterial = true;
    MeshBatch.CastShadow = ViewRelevance.bShadowRelevance;
    MeshBatch.bUseForDepthPass = false;

    // Set up MeshBatchElement
    FMeshBatchElement& BatchElement = MeshBatch.Elements[0];

    BatchElement.PrimitiveUniformBuffer = GetUniformBuffer();
    BatchElement.IndexBuffer = &SingleVolumeMeshData.IndexBuffer;
    BatchElement.FirstIndex = 0;
    BatchElement.MinVertexIndex = 0;
    BatchElement.MaxVertexIndex = SingleVolumeMeshData.NumVertices -1;
    BatchElement.NumPrimitives = SingleVolumeMeshData.NumPrimitives;
    BatchElement.VertexFactoryUserData = VertexFactory.GetUniformBuffer();

    // 可以使用 User Data 传递一些逐Element的数据
     // BatchElement.UserData = nullptr;

    Collector.AddMesh(ViewIndex, MeshBatch);

    return &MeshBatch;
}




MeshBatch实现了FPrimitiveSceneProxy与渲染结构的分离
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
FMeshBatch* FSingleVolumeSceneProxy::CreateMeshBatch(const FSceneView* View, int32 ViewIndex,
  const FSceneViewFamily& ViewFamily,
  FMeshElementCollector& Collector,
  const FMaterialRenderProxy* MaterialRenderProxy) const
  // 获取视图相关性
    const FPrimitiveViewRelevance& ViewRelevance = GetViewRelevance(View);
    // Set up MeshBatch
    FMeshBatch& MeshBatch = Collector.AllocateMesh();
    MeshBatch.bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;
    MeshBatch.bUseWireframeSelectionColoring = IsSelected();
    MeshBatch.VertexFactory = &VertexFactory;
    MeshBatch.MaterialRenderProxy = MaterialRenderProxy;
    MeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative() ? true : false;
    MeshBatch.Type = PT_TriangleList;
    MeshBatch.DepthPriorityGroup = SDPG_World;
    MeshBatch.bCanApplyViewModeOverrides = true;
    MeshBatch.bUseForMaterial = true;
    MeshBatch.CastShadow = ViewRelevance.bShadowRelevance;
    MeshBatch.bUseForDepthPass = false;
    // Set up MeshBatchElement
    FMeshBatchElement& BatchElement = MeshBatch.Elements[0];
    BatchElement.PrimitiveUniformBuffer = GetUniformBuffer();
    BatchElement.IndexBuffer = &SingleVolumeMeshData.IndexBuffer;
    BatchElement.FirstIndex = 0;
    BatchElement.MinVertexIndex = 0;
    BatchElement.MaxVertexIndex = SingleVolumeMeshData.NumVertices -1;
    BatchElement.NumPrimitives = SingleVolumeMeshData.NumPrimitives;
    BatchElement.VertexFactoryUserData = VertexFactory.GetUniformBuffer();
    // 可以使用 User Data 传递一些逐Element的数据
     // BatchElement.UserData = nullptr;
    Collector.AddMesh(ViewIndex, MeshBatch);
    return &MeshBatch;
```

## 第 34 页：Scene Proxy 的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 34 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
 Scene Proxy 的设计
  理解静态与动态绘制路径




1，UpdateAllPrimitiveSceneInfos阶段：删除，更新CPU静态图元信息，调整视口，释放前一帧颜色信息

2，InitView阶段：

- PreVisibilityFrameSetup:可见性剔除前的相关初始化工作；

- ComputeViewVisibility(): 分配可见光源信息，更新可见的动态网格，调用GatherDynamicMeshElements，执行SetupMeshPass;
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
- ComputeViewVisibility(): 分配可见光源信息，更新可见的动态网格，调用GatherDynamicMeshElements，执行SetupMeshPass;
```

## 第 35 页：渲染管线的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 35 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
渲染管线的设计
扩展场景渲染视图

class FSingleVolumeRendering : public FSceneViewExtensionBase
{


                                                                                                                                                                                               构造函数必须包含FAutoRegister，且必须是第一个参数
public:
  // 第一个参数必须是“FAutoRegister”
  FSingleVolumeRendering(const FAutoRegister& AutoRegister);

     // 是否渲染体积
      bool ShouldRenderSingleVolume() const;



                                                                                                                                                                                               渲染视图的初始化与释放
     // 初始化与释放渲染器
      void Init();
     void Release();

     // 增加减少一个VolumeProxy


                                                                                                                                                                                               渲染管线管理一个SingleVolumeSceneProxy数组，控制元素追加与删减
     void AddVolumeProxy(FSingleVolumeSceneProxy* Proxy);
     void RemoveVolumeProxy(FSingleVolumeSceneProxy* Proxy);

     //~ Begin ISceneViewExtension Interface
     virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}                                             // 在游戏线程创建ViewFamily时调用
      virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}                             // 在游戏线程创建View时调用

                                                                                                                                                                                               按需覆写，它们会在相应的环节中被调用
      virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {}                                     // 当ViewFamily即将渲染在游戏线程调用
      virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override {}                 // PreRenderViewFamily_RenderThread之后，渲染线程准备开始渲染每个View时调用
      virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override; // 开始渲染ViewFamily前调用
      virtual int32 GetPriority() const override { return -1; }
     virtual bool IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const { return true; }
     //~ End ISceneViewExtension Interface

private:
  void InitRendering();                                                                                                                                                                        以渲染委托的方式注册与销毁渲染管线Pass
  void ReleaseRendering();

     void InitDelegate();
     void ReleaseDelegate();


                                                                                                                                                                                               统一渲染入口
     void Render_RenderThread(FPostOpaqueRenderParameters& Parameters, bool PostTranslucent);
     void RenderPostOpaque_RenderThread(FPostOpaqueRenderParameters& Parameters);
     void RenderOverlay_RenderThread(FPostOpaqueRenderParameters& Parameters);

     void RenderSingleVolume(FSingleVolumeSceneProxy* Proxy, bool Translucent, const FPostOpaqueRenderParameters& Parameters, FRDGTexture* RenderTexture, FRDGTexture* DepthRenderTexture );

     void RenderSingleVolumeInternal(
       // Object Data
       FSingleVolumeSceneProxy* Proxy,


                                                                                                                                                                                               需要区分两套代理，以便插入Base Pass之后，
       bool Translucent, bool ApplyEmissionAndTransmittance, bool ApplyDirectLighting, bool ApplyShadowTransmittance,
       uint32 LightType,FLightSceneInfo* LightSceneInfo,FVisibleLightInfo* VisibleLightInfo,
       const FPostOpaqueRenderParameters& Parameters,FRDGTexture* RenderTexture,FRDGTexture* DepthRenderTexture);

     TArray<FSingleVolumeSceneProxy*> SingleVolumeSceneProxies;
                                                                                                                                                                                               Translucent之前或Translucent之后，Post Process之前
     FPostOpaqueRenderDelegate RenderPostOpaqueDelegate;
     FPostOpaqueRenderDelegate RenderOverlayDelegate;
     FDelegateHandle RenderPostOpaqueDelegateHandle;
     FDelegateHandle RenderOverlayDelegateHandle;


     ESingleVolumeDenoiserMethod VolumeDenoiserMethod = ESingleVolumeDenoiserMethod::None;
};
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
class FSingleVolumeRendering : public FSceneViewExtensionBase
  // 第一个参数必须是“FAutoRegister”
  FSingleVolumeRendering(const FAutoRegister& AutoRegister);
     // 是否渲染体积
      bool ShouldRenderSingleVolume() const;
     // 初始化与释放渲染器
      void Init();
     void Release();
     // 增加减少一个VolumeProxy
     void AddVolumeProxy(FSingleVolumeSceneProxy* Proxy);
     void RemoveVolumeProxy(FSingleVolumeSceneProxy* Proxy);
     //~ Begin ISceneViewExtension Interface
     virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}                                             // 在游戏线程创建ViewFamily时调用
      virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}                             // 在游戏线程创建View时调用
      virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {}                                     // 当ViewFamily即将渲染在游戏线程调用
      virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override {}                 // PreRenderViewFamily_RenderThread之后，渲染线程准备开始渲染每个View时调用
      virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override; // 开始渲染ViewFamily前调用
      virtual int32 GetPriority() const override { return -1; }
     virtual bool IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const { return true; }
     //~ End ISceneViewExtension Interface
  void InitRendering();                                                                                                                                                                        以渲染委托的方式注册与销毁渲染管线Pass
  void ReleaseRendering();
     void InitDelegate();
     void ReleaseDelegate();
     void Render_RenderThread(FPostOpaqueRenderParameters& Parameters, bool PostTranslucent);
     void RenderPostOpaque_RenderThread(FPostOpaqueRenderParameters& Parameters);
     void RenderOverlay_RenderThread(FPostOpaqueRenderParameters& Parameters);
     void RenderSingleVolume(FSingleVolumeSceneProxy* Proxy, bool Translucent, const FPostOpaqueRenderParameters& Parameters, FRDGTexture* RenderTexture, FRDGTexture* DepthRenderTexture );
     void RenderSingleVolumeInternal(
       // Object Data
       bool Translucent, bool ApplyEmissionAndTransmittance, bool ApplyDirectLighting, bool ApplyShadowTransmittance,
       const FPostOpaqueRenderParameters& Parameters,FRDGTexture* RenderTexture,FRDGTexture* DepthRenderTexture);
     TArray<FSingleVolumeSceneProxy*> SingleVolumeSceneProxies;
     FPostOpaqueRenderDelegate RenderPostOpaqueDelegate;
     FPostOpaqueRenderDelegate RenderOverlayDelegate;
     FDelegateHandle RenderPostOpaqueDelegateHandle;
     FDelegateHandle RenderOverlayDelegateHandle;
     ESingleVolumeDenoiserMethod VolumeDenoiserMethod = ESingleVolumeDenoiserMethod::None;
```

## 第 36 页：渲染管线的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 36 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
渲染管线的设计
渲染初始化
          FSingleVolumeRuntimeModule::TRenderExtensionPtr FSingleVolumeRuntimeModule::GetRenderExtension()
          {
              static const FName ModuleName = "SingleVolumeRuntime";
              auto& ModuleInterface = FModuleManager::LoadModuleChecked<FSingleVolumeRuntimeModule>(ModuleName);
              // 返回一个渲染器的共享指针
             return ModuleInterface.GetOrCreateRenderExtension();
          }



          FSingleVolumeRuntimeModule::TRenderExtensionPtr FSingleVolumeRuntimeModule::GetOrCreateRenderExtension()
          {
            if (!SingleVolumeRenderExtension.IsValid())
            {
                  // New一个渲染器智能指针出来
                   SingleVolumeRenderExtension = FSceneViewExtensions::NewExtension<FSingleVolumeRendering>();

                  // 执行渲染器的初始化
                   SingleVolumeRenderExtension->Init();
              }

              return SingleVolumeRenderExtension;
          }



          void FSingleVolumeRendering::Init()                                       void FSingleVolumeRendering::InitRendering()
          {                                                                         {
            if (IsInRenderingThread())                                                  // 确保是在渲染线程
            {                                                                           check(IsInRenderingThread());
               InitRendering();                                                         // 先强制释放线程再初始化
            }                                                                           ReleaseRendering();
            else                                                                       {
            {                                                                              InitDelegate();
               ENQUEUE_RENDER_COMMAND(FInitSingleVolumeRenderer)(                       }
                  [this](FRHICommandListImmediate& RHICmdList)                      }
                  {
                     Init();
                  });
            }
          }



          void FSingleVolumeRendering::InitDelegate()
          {
            if (!RenderPostOpaqueDelegateHandle.IsValid())
            {
               const FName RendererModuleName("Renderer");
               IRendererModule* RendererModule = FModuleManager::GetModulePtr<IRendererModule>(RendererModuleName);
               if (RendererModule)
               {
                    // 绑定渲染线程的不同执行方法到委托
                    RenderPostOpaqueDelegate.BindRaw(this, &FSingleVolumeRendering::RenderPostOpaque_RenderThread);
                    RenderOverlayDelegate.BindRaw(this, &FSingleVolumeRendering::RenderOverlay_RenderThread);
                    // 将委托注册Renderer 模块,并返回对应的句柄
                    RenderPostOpaqueDelegateHandle = RendererModule->RegisterPostOpaqueRenderDelegate(RenderPostOpaqueDelegate);
                    RenderOverlayDelegateHandle = RendererModule->RegisterOverlayRenderDelegate(RenderOverlayDelegate);
               }
            }
          }
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
          FSingleVolumeRuntimeModule::TRenderExtensionPtr FSingleVolumeRuntimeModule::GetRenderExtension()
              static const FName ModuleName = "SingleVolumeRuntime";
              auto& ModuleInterface = FModuleManager::LoadModuleChecked<FSingleVolumeRuntimeModule>(ModuleName);
              // 返回一个渲染器的共享指针
             return ModuleInterface.GetOrCreateRenderExtension();
          FSingleVolumeRuntimeModule::TRenderExtensionPtr FSingleVolumeRuntimeModule::GetOrCreateRenderExtension()
            if (!SingleVolumeRenderExtension.IsValid())
                  // New一个渲染器智能指针出来
                   SingleVolumeRenderExtension = FSceneViewExtensions::NewExtension<FSingleVolumeRendering>();
                  // 执行渲染器的初始化
                   SingleVolumeRenderExtension->Init();
              return SingleVolumeRenderExtension;
          void FSingleVolumeRendering::Init()                                       void FSingleVolumeRendering::InitRendering()
          {                                                                         {
            if (IsInRenderingThread())                                                  // 确保是在渲染线程
            {                                                                           check(IsInRenderingThread());
               InitRendering();                                                         // 先强制释放线程再初始化
            }                                                                           ReleaseRendering();
            else                                                                       {
            {                                                                              InitDelegate();
               ENQUEUE_RENDER_COMMAND(FInitSingleVolumeRenderer)(                       }
                  [this](FRHICommandListImmediate& RHICmdList)                      }
                     Init();
          void FSingleVolumeRendering::InitDelegate()
            if (!RenderPostOpaqueDelegateHandle.IsValid())
               const FName RendererModuleName("Renderer");
               IRendererModule* RendererModule = FModuleManager::GetModulePtr<IRendererModule>(RendererModuleName);
               if (RendererModule)
                    // 绑定渲染线程的不同执行方法到委托
                    RenderPostOpaqueDelegate.BindRaw(this, &FSingleVolumeRendering::RenderPostOpaque_RenderThread);
                    RenderOverlayDelegate.BindRaw(this, &FSingleVolumeRendering::RenderOverlay_RenderThread);
                    // 将委托注册Renderer 模块,并返回对应的句柄
                    RenderPostOpaqueDelegateHandle = RendererModule->RegisterPostOpaqueRenderDelegate(RenderPostOpaqueDelegate);
                    RenderOverlayDelegateHandle = RendererModule->RegisterOverlayRenderDelegate(RenderOverlayDelegate);
```

## 第 37 页：渲染管线的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 37 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
渲染管线的设计
在Renderer不同环节插入使用相同封装策略的主渲染入口函数

void FSingleVolumeRendering::RenderPostOpaque_RenderThread(FPostOpaqueRenderParameters& Parameters)
{
    Render_RenderThread(Parameters, false);
}

void FSingleVolumeRendering::RenderOverlay_RenderThread(FPostOpaqueRenderParameters& Parameters)
{
    Render_RenderThread(Parameters, true);
}


class FPostOpaqueRenderParameters
{
public:
   FIntRect ViewportRect;
   FMatrix ViewMatrix;
   FMatrix ProjMatrix;
   FRDGTexture* ColorTexture = nullptr;
   FRDGTexture* DepthTexture = nullptr;
   FRDGTexture* NormalTexture = nullptr;
   FRDGTexture* VelocityTexture = nullptr;
   FRDGTexture* SmallDepthTexture = nullptr;
   FRDGBuilder* GraphBuilder = nullptr;
   FRHIUniformBuffer* ViewUniformBuffer = nullptr;
   TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTexturesUniformParams = nullptr;
   TRDGUniformBufferRef<FMobileSceneTextureUniformParameters> MobileSceneTexturesUniformParams = nullptr;
   const FGlobalDistanceFieldParameterData* GlobalDistanceFieldParams = nullptr;
   void* Uid = nullptr;
   const FViewInfo* View = nullptr;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostOpaqueRender, class FPostOpaqueRenderParameters&);
typedef FOnPostOpaqueRender::FDelegate FPostOpaqueRenderDelegate;
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
void FSingleVolumeRendering::RenderPostOpaque_RenderThread(FPostOpaqueRenderParameters& Parameters)
    Render_RenderThread(Parameters, false);
void FSingleVolumeRendering::RenderOverlay_RenderThread(FPostOpaqueRenderParameters& Parameters)
    Render_RenderThread(Parameters, true);
class FPostOpaqueRenderParameters
   FIntRect ViewportRect;
   FMatrix ViewMatrix;
   FMatrix ProjMatrix;
   FRDGTexture* ColorTexture = nullptr;
   FRDGTexture* DepthTexture = nullptr;
   FRDGTexture* NormalTexture = nullptr;
   FRDGTexture* VelocityTexture = nullptr;
   FRDGTexture* SmallDepthTexture = nullptr;
   FRDGBuilder* GraphBuilder = nullptr;
   FRHIUniformBuffer* ViewUniformBuffer = nullptr;
   TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTexturesUniformParams = nullptr;
   TRDGUniformBufferRef<FMobileSceneTextureUniformParameters> MobileSceneTexturesUniformParams = nullptr;
   const FGlobalDistanceFieldParameterData* GlobalDistanceFieldParams = nullptr;
   void* Uid = nullptr;
   const FViewInfo* View = nullptr;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostOpaqueRender, class FPostOpaqueRenderParameters&);
typedef FOnPostOpaqueRender::FDelegate FPostOpaqueRenderDelegate;
```

## 第 38 页：渲染管线的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 38 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
渲染管线的设计
按谓词针对 Scene Proxies 进行排序

TArray<FSingleVolumeSceneProxy*> OpaqueProxies = SingleVolumeSceneProxies.FilterByPredicate(
   [View, PostTranslucent](const FSingleVolumeSceneProxy* Proxy)
   {
      return Proxy->IsVisible(View) && !Proxy->IsTranslucentProxy() && (Proxy->RenderProxyAfterTranslucent() == PostTranslucent);
   });                                                                                                                              不透明
OpaqueProxies.Sort([ViewMat](const FSingleVolumeSceneProxy& Lhs, const FSingleVolumeSceneProxy& Rhs) -> bool
{                                                                                                                                   （当前语境可忽略）
   const FVector& LeftProxyCenter = Lhs.GetBounds().GetSphere().Center;
   const FVector& RightProxyCenter = Rhs.GetBounds().GetSphere().Center;
   return ViewMat.TransformPosition(LeftProxyCenter).Z < ViewMat.TransformPosition(RightProxyCenter).Z; // front to back
});


TArray<FSingleVolumeSceneProxy*> TranslucentProxies = SingleVolumeSceneProxies.FilterByPredicate(
   [View, PostTranslucent](const FSingleVolumeSceneProxy* Proxy)
   {
      return Proxy->IsVisible(View) && Proxy->IsTranslucentProxy() && (Proxy->RenderProxyAfterTranslucent() == PostTranslucent);
   });                                                                                                                              半透明
TranslucentProxies.Sort([ViewMat](const FSingleVolumeSceneProxy& Lhs, const FSingleVolumeSceneProxy& Rhs) -> bool
{                                                                                                                                   （目标体积介质代理）
   const FVector& LeftProxyCenter = Lhs.GetBounds().GetSphere().Center;
   const FVector& RightProxyCenter = Rhs.GetBounds().GetSphere().Center;
   return ViewMat.TransformPosition(LeftProxyCenter).Z > ViewMat.TransformPosition(RightProxyCenter).Z; // back to front
});
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
TArray<FSingleVolumeSceneProxy*> OpaqueProxies = SingleVolumeSceneProxies.FilterByPredicate(
   [View, PostTranslucent](const FSingleVolumeSceneProxy* Proxy)
      return Proxy->IsVisible(View) && !Proxy->IsTranslucentProxy() && (Proxy->RenderProxyAfterTranslucent() == PostTranslucent);
   });                                                                                                                              不透明
OpaqueProxies.Sort([ViewMat](const FSingleVolumeSceneProxy& Lhs, const FSingleVolumeSceneProxy& Rhs) -> bool
{                                                                                                                                   （当前语境可忽略）
   const FVector& LeftProxyCenter = Lhs.GetBounds().GetSphere().Center;
   const FVector& RightProxyCenter = Rhs.GetBounds().GetSphere().Center;
   return ViewMat.TransformPosition(LeftProxyCenter).Z < ViewMat.TransformPosition(RightProxyCenter).Z; // front to back
TArray<FSingleVolumeSceneProxy*> TranslucentProxies = SingleVolumeSceneProxies.FilterByPredicate(
   [View, PostTranslucent](const FSingleVolumeSceneProxy* Proxy)
      return Proxy->IsVisible(View) && Proxy->IsTranslucentProxy() && (Proxy->RenderProxyAfterTranslucent() == PostTranslucent);
   });                                                                                                                              半透明
TranslucentProxies.Sort([ViewMat](const FSingleVolumeSceneProxy& Lhs, const FSingleVolumeSceneProxy& Rhs) -> bool
{                                                                                                                                   （目标体积介质代理）
   const FVector& LeftProxyCenter = Lhs.GetBounds().GetSphere().Center;
   const FVector& RightProxyCenter = Rhs.GetBounds().GetSphere().Center;
   return ViewMat.TransformPosition(LeftProxyCenter).Z > ViewMat.TransformPosition(RightProxyCenter).Z; // back to front
```

## 第 39 页：渲染管线的设计

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 39 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
渲染管线的设计
添加并组织绘制Pass，以经典的“双Pass方案”为基础

if (!TranslucentProxies.IsEmpty())                                                                                                       void AddClearDepthStencilPass(FRDGBuilder& GraphBuilder, FRDGTextureRef Texture, ERenderTargetLoadAction DepthLoadAction,
                                                                                                                                         ERenderTargetLoadAction StencilLoadAction)
{                                                                                                                                        {
     // 埋个点，只监听透明渲染代理的渲染计数                                                                                                                  auto* PassParameters = GraphBuilder.AllocParameters<FRenderTargetParameters>();
     SCOPE_CYCLE_COUNTER(STAT_TranslucentProxies_RT);                                                                                       PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(Texture, DepthLoadAction, StencilLoadAction,
                                                                                                                                         FExclusiveDepthStencil::DepthWrite_StencilWrite);
                                                                                                                                            GraphBuilder.AddPass(RDG_EVENT_NAME("ClearDepthStencil (%s)", Texture->Name), PassParameters, ERDGPassFlags::Raster,
    // Step1：创建2张RDG Texture,用于写入颜色和写入深度                                                                                                 [](FRHICommandList&) {});
    FRDGTexture* VolumeRenderTexture = nullptr;                                                                                          }
    FRDGTextureDesc TexDesc = Parameters.ColorTexture->Desc;
    TexDesc.Format = PF_FloatRGBA;
    TexDesc.ClearValue = FClearValueBinding(FLinearColor::Transparent);
    VolumeRenderTexture = GraphBuilder.CreateTexture(TexDesc, TEXT(“VolumeRenderTexture”));                                             // 用于指定是否要覆盖缓冲区或累积
                                                                                                                                        enum class ERenderTargetLoadAction : uint8
    AddClearRenderTargetPass(GraphBuilder, VolumeRenderTexture);                                                                        {
                                                                                                                                           // 不保留任何现有内容，直接覆盖.
    FRDGTextureRef DepthRenderTexture = nullptr;                                                                                           ENoAction,

                                                                                                                                                                                                    RenderGraphUtils.cpp找到更多RDG实用程序
    DepthRenderTexture = GraphBuilder.CreateTexture(
    FRDGTextureDesc::Create2D(Parameters.DepthTexture->Desc.Extent,                                                                          // 保留现有内容，以加法进行累积

                                                                                                                                                                                                    尽可能使用这些实用程序能控制样板代码，统一编程风格
      PF_DepthStencil,                                                                                                                       ELoad,
      FClearValueBinding::DepthFar,
      TexCreate_DepthStencilTargetable | TexCreate_ShaderResource,                                                                           // 将被清空为资源上指定的快速清楚值
      1),                                                                                                                                    EClear,
      TEXT(“VolumeDepth”));
                                                                                                                                             Num,
    AddClearDepthStencilPass(GraphBuilder, DepthRenderTexture, ERenderTargetLoadAction::EClear, ERenderTargetLoadAction::ENoAction);         NumBits = 2,
                                                                                                                                        };
     // Step2：执行第一个Pass，轮询每一个符合过滤条件的Scene Proxy, 进行绘制
    for (FSingleVolumeSceneProxy* Proxy : TranslucentProxies)
     {

                                                                                                                                       RenderSingleVolume 定义了主渲染Pass，这是一个Mesh Pass
        RenderSingleVolume(Proxy, true, Parameters, VolumeRenderTexture, DepthRenderTexture);
     }

    // Step3：主渲染Pass结束之后，需要新增一个Pass将颜色与深度混合进场景纹理组
    SingleVolumeComposite::CompositeFullScreen(

                                                                                                                                       CompositeFullScreen 定义了渲染结果的混合，这是一个Global Pass
    GraphBuilder,
    VolumeRenderTextureAfterDenoiser,
    Parameters.ColorTexture,
    DepthRenderTexture,
    Parameters.DepthTexture,
    View);
}
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
if (!TranslucentProxies.IsEmpty())                                                                                                       void AddClearDepthStencilPass(FRDGBuilder& GraphBuilder, FRDGTextureRef Texture, ERenderTargetLoadAction DepthLoadAction,
{                                                                                                                                        {
     // 埋个点，只监听透明渲染代理的渲染计数                                                                                                                  auto* PassParameters = GraphBuilder.AllocParameters<FRenderTargetParameters>();
     SCOPE_CYCLE_COUNTER(STAT_TranslucentProxies_RT);                                                                                       PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(Texture, DepthLoadAction, StencilLoadAction,
                                                                                                                                         FExclusiveDepthStencil::DepthWrite_StencilWrite);
                                                                                                                                            GraphBuilder.AddPass(RDG_EVENT_NAME("ClearDepthStencil (%s)", Texture->Name), PassParameters, ERDGPassFlags::Raster,
    // Step1：创建2张RDG Texture,用于写入颜色和写入深度                                                                                                 [](FRHICommandList&) {});
    FRDGTexture* VolumeRenderTexture = nullptr;                                                                                          }
    FRDGTextureDesc TexDesc = Parameters.ColorTexture->Desc;
    TexDesc.Format = PF_FloatRGBA;
    TexDesc.ClearValue = FClearValueBinding(FLinearColor::Transparent);
    VolumeRenderTexture = GraphBuilder.CreateTexture(TexDesc, TEXT(“VolumeRenderTexture”));                                             // 用于指定是否要覆盖缓冲区或累积
                                                                                                                                        enum class ERenderTargetLoadAction : uint8
    AddClearRenderTargetPass(GraphBuilder, VolumeRenderTexture);                                                                        {
                                                                                                                                           // 不保留任何现有内容，直接覆盖.
    FRDGTextureRef DepthRenderTexture = nullptr;                                                                                           ENoAction,
    DepthRenderTexture = GraphBuilder.CreateTexture(
    FRDGTextureDesc::Create2D(Parameters.DepthTexture->Desc.Extent,                                                                          // 保留现有内容，以加法进行累积
      FClearValueBinding::DepthFar,
      TexCreate_DepthStencilTargetable | TexCreate_ShaderResource,                                                                           // 将被清空为资源上指定的快速清楚值
      TEXT(“VolumeDepth”));
    AddClearDepthStencilPass(GraphBuilder, DepthRenderTexture, ERenderTargetLoadAction::EClear, ERenderTargetLoadAction::ENoAction);         NumBits = 2,
     // Step2：执行第一个Pass，轮询每一个符合过滤条件的Scene Proxy, 进行绘制
    for (FSingleVolumeSceneProxy* Proxy : TranslucentProxies)
        RenderSingleVolume(Proxy, true, Parameters, VolumeRenderTexture, DepthRenderTexture);
    // Step3：主渲染Pass结束之后，需要新增一个Pass将颜色与深度混合进场景纹理组
    SingleVolumeComposite::CompositeFullScreen(
    GraphBuilder,
    View);
```

## 第 40 页：void FSingleVolumeRendering::RenderSingleVolumeInternal( FSingleVolumeSceneProxy* Proxy, bool Translucent, bool ApplyEmissionAndTransmittanc

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 40 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                                                                                                   void FSingleVolumeRendering::RenderSingleVolumeInternal( FSingleVolumeSceneProxy* Proxy, bool Translucent, bool ApplyEmissionAndTransmittance, bool ApplyDirectLighting, bool ApplyShadowTransmittance, uint32 LightType,
                                                                                                                     FLightSceneInfo* LightSceneInfo,
                                                                                                                     FVisibleLightInfo* VisibleLightInfo,
                                                                                                                     // Scene Data




网格绘制管线
                                                                                                                     const FPostOpaqueRenderParameters& Parameters,
                                                                                                                     // Output
                                                                                                                     FRDGTexture* RenderTexture,
                                                                                                                     FRDGTexture* DepthRenderTexture)
                                                                                                                   {
                                                                                                                     const FSceneView* View = static_cast<FSceneView*>(Parameters.Uid);

                                                                                                                       FRDGBuilder& GraphBuilder = *Parameters.GraphBuilder;




主绘制Pass：内外两层封装设计                                                                                                                                                                                                                                                                                                               传参
                                                                                                                       // 为PassParameter分配内存
                                                                                                                       FSingleVolumeShaderPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FSingleVolumeShaderPS::FParameters>();
                                                                                                                       // 传参
                                                                                                                       SetupRenderPassParameters(Proxy, GraphBuilder,PassParameters, ApplyEmissionAndTransmittance,ApplyDirectLighting, ApplyShadowTransmittance,LightType,LightSceneInfo,VisibleLightInfo,Parameters);

                                                                                                                        // RenderTarget与RenderTargets.DepthStencil的处理
                                                                                                                        bool bWriteDepth = DepthRenderTexture != nullptr;
                                                                                                                       if (RenderTexture)
                                                                                                                       {
                                                                                                                          PassParameters->RenderTargets[0] = FRenderTargetBinding(RenderTexture, ERenderTargetLoadAction::ELoad);
void FSingleVolumeRendering::RenderSingleVolume(                                                                          if (bWriteDepth)


                                                                                                                                                                                                                                                                                                                               RTs
  // Object Data                                                                                                          {
                                                                                                                             PassParameters->RenderTargets.DepthStencil =
  FSingleVolumeSceneProxy* Proxy,                                                                                              FDepthStencilBinding(DepthRenderTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthWrite_StencilNop);
  bool Translucent,                                                                                                       }
  // Scene Data                                                                                                        }
                                                                                                                       else
  const FPostOpaqueRenderParameters& Parameters,                                                                       {
  // Output                                                                                                               PassParameters->RenderTargets[0] = FRenderTargetBinding(Parameters.ColorTexture, ERenderTargetLoadAction::ELoad);
                                                                                                                          PassParameters->RenderTargets.DepthStencil =
  FRDGTexture* RenderTexture,                                                                                                FDepthStencilBinding(Parameters.DepthTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthWrite_StencilNop);
  FRDGTexture* DepthRenderTexture)                                                                                     }
{                                                                                                                      GraphBuilder.AddPass(
  const FSceneView* View = static_cast<const FSceneView*>(Parameters.View);                                             Translucent ? RDG_EVENT_NAME(“SingleVolume Translucent Rendering”) : RDG_EVENT_NAME(“SingleVolume Opaque Rendering”),
  const FSceneViewFamily* ViewFamily = View->Family;                                                                    PassParameters,
                                                                                                                        ERDGPassFlags::Raster,
  const FScene* Scene = (FScene*)ViewFamily->Scene;

                                                                                                                                                                                                                                                                                                                        AddPass
                                                                                                                        [this, &InView = *View, ViewportRect = Parameters.ViewportRect, Proxy, bWriteDepth, FirstLight = ApplyEmissionAndTransmittance](FRHICommandListImmediate& RHICmdList)
                                                                                                                        {
                                                                                                                           // 添加绘制事件以便在RDG中直观的调试
 if (!Proxy || !Proxy->GetMaterial() || !Proxy->IsVisible(View) || !Proxy->GetDensityRenderResource() || !Proxy-          SCOPED_DRAW_EVENTF(RHICmdList, StatSingleVolume, TEXT(“SingleVolume Main Pass %s”), *Proxy->GetOwnerName().ToString());
>GetDensityRenderResource()->IsInitialized())                                                                              SCOPED_GPU_STAT(RHICmdList, StatSingleVolume);
    return;

                                                                  区分引擎联合版与独立版
                                                                                                                                RHICmdList.SetViewport(ViewportRect.Min.X, ViewportRect.Min.Y, 0.0f, ViewportRect.Max.X, ViewportRect.Max.Y, 1.0f);
                                                                                                                                RHICmdList.SetScissorRect(false, 0,0,0,0);
    // 渲染器依赖引擎版的分支
                                                                                                                                FRHITextureViewCache TexCache;
     // ***                                                                                                                     DrawDynamicMeshPass(InView, RHICmdList,
                                                                                                                                 [&](FDynamicPassMeshDrawListContext* DynamicMeshPassContext)
                                                                                                                                 {
    // 渲染器的独立版支持分支                                                                                                                 const FBoxSphereBounds LocalBoxSphereBounds = Proxy->GetLocalBounds();
     RenderSingleVolumeInternal(                                                                                                   FParticipationMediumElementData ShaderElementData;
                                                                                                                                   ShaderElementData.LocalBoundsOrigin = FVector4f(FVector3f(LocalBoxSphereBounds.Origin), 1.0);
    // Object Data

                                                                                                                                                                                                                                                                                         DrawDynamicMeshPass
                                                                                                                                   ShaderElementData.LocalBoundsExtent = FVector4f(FVector3f(LocalBoxSphereBounds.BoxExtent), 1.0);
    Proxy,                                                                                                                         ShaderElementData.RenderFlagsData = Proxy->GetRenderFlagsData();
    Translucent,                                                                                                                   ShaderElementData.RaymarchData = Proxy->GetRaymarchData();
                                                                                                                                   ShaderElementData.ScatterData = Proxy->GetScatterData();
    // Light Data                                                                                                                  ShaderElementData.LightingData = Proxy->GetLightingData();
    true,                                                                                                                          ShaderElementData.EmissionData = Proxy->GetEmissionData();
                                                                                                                                   ShaderElementData.WindFieldData = Proxy->GetWindFieldData();
    true,                                                                                                                          ShaderElementData.CustomFloatData1 = Proxy->GetCustomFloatData1();
    true,                                                                                                                          ShaderElementData.CustomFloatData2 = Proxy->GetCustomFloatData2();
    0,
                                                                                                                                         // 统一以SRV，UAV方式传递数据，以便后续集成VDB
    nullptr,                                                                                                                             FTexture* Density = Proxy->GetDensityRenderResource();
    nullptr,                                                                                                                             FTextureRHIRef DensityRHI = Density ? Density->GetTextureRHI() : nullptr;
                                                                                                                                         ShaderElementData.DensityBufferSRV = DensityRHI ? FShaderResourceViewRHIRef(TexCache.GetOrCreateSRV(RHICmdList, DensityRHI, FRHITextureSRVCreateInfo())) : GBlackTextureWithSRV->ShaderResourceViewRHI;
    // Scene Data
    Parameters,                                                                                                                          FSingleVolumeMeshProcessor PassMeshProcessor(
                                                                                                                                          InView.Family->Scene->GetRenderScene(), &InView,DynamicMeshPassContext,
    // Output                                                                                                                             Proxy->IsSignedDistance(),Proxy->IsSparseVolumeTexture(), Proxy->UseCloudVolumeMod(), Proxy->UseFogVolumeMod(), Proxy->UseStandardVolumeMod(),Proxy->IsBlackBodyMaterial(),


                                                                                                                                                                                                                                                                                                 MeshPassProcessor
    RenderTexture,                                                                                                                        bWriteDepth, FirstLight,
    DepthRenderTexture                                                                                                                    MoveTemp(ShaderElementData));

    );                                                                                                                                   // 获取当前View 下的 MeshBatch，执行 Add Mesh Batch 方法
                                                                                                                                         if (FMeshBatch* VolumeMesh = Proxy->GetMeshFromView(&InView))
                                                                                                                                         {


                                                                                                                                                                                                                                                                                                           AddMeshBatch
    // Disable any depth test / write after first lighting pass                                                                             const uint64 DefaultBatchElementMask = ~0ull;
    DepthRenderTexture = nullptr;                                                                                                           PassMeshProcessor.AddMeshBatch(*VolumeMesh,DefaultBatchElementMask,Proxy);
                                                                                                                                         }
}                                                                                                                                    }
                                                                                                                                );
                                                                                                                            }
                                                                                                                       );
                                                                                                                   }
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                                                                                                                   void FSingleVolumeRendering::RenderSingleVolumeInternal( FSingleVolumeSceneProxy* Proxy, bool Translucent, bool ApplyEmissionAndTransmittance, bool ApplyDirectLighting, bool ApplyShadowTransmittance, uint32 LightType,
                                                                                                                     // Scene Data
                                                                                                                     const FPostOpaqueRenderParameters& Parameters,
                                                                                                                     // Output
                                                                                                                     FRDGTexture* RenderTexture,
                                                                                                                     FRDGTexture* DepthRenderTexture)
                                                                                                                     const FSceneView* View = static_cast<FSceneView*>(Parameters.Uid);
                                                                                                                       FRDGBuilder& GraphBuilder = *Parameters.GraphBuilder;
                                                                                                                       // 为PassParameter分配内存
                                                                                                                       FSingleVolumeShaderPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FSingleVolumeShaderPS::FParameters>();
                                                                                                                       // 传参
                                                                                                                       SetupRenderPassParameters(Proxy, GraphBuilder,PassParameters, ApplyEmissionAndTransmittance,ApplyDirectLighting, ApplyShadowTransmittance,LightType,LightSceneInfo,VisibleLightInfo,Parameters);
                                                                                                                        // RenderTarget与RenderTargets.DepthStencil的处理
                                                                                                                        bool bWriteDepth = DepthRenderTexture != nullptr;
                                                                                                                       if (RenderTexture)
                                                                                                                          PassParameters->RenderTargets[0] = FRenderTargetBinding(RenderTexture, ERenderTargetLoadAction::ELoad);
void FSingleVolumeRendering::RenderSingleVolume(                                                                          if (bWriteDepth)
  // Object Data                                                                                                          {
                                                                                                                             PassParameters->RenderTargets.DepthStencil =
  FSingleVolumeSceneProxy* Proxy,                                                                                              FDepthStencilBinding(DepthRenderTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthWrite_StencilNop);
  bool Translucent,                                                                                                       }
  // Scene Data                                                                                                        }
                                                                                                                       else
  const FPostOpaqueRenderParameters& Parameters,                                                                       {
  // Output                                                                                                               PassParameters->RenderTargets[0] = FRenderTargetBinding(Parameters.ColorTexture, ERenderTargetLoadAction::ELoad);
                                                                                                                          PassParameters->RenderTargets.DepthStencil =
  FRDGTexture* RenderTexture,                                                                                                FDepthStencilBinding(Parameters.DepthTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthWrite_StencilNop);
  FRDGTexture* DepthRenderTexture)                                                                                     }
{                                                                                                                      GraphBuilder.AddPass(
  const FSceneView* View = static_cast<const FSceneView*>(Parameters.View);                                             Translucent ? RDG_EVENT_NAME(“SingleVolume Translucent Rendering”) : RDG_EVENT_NAME(“SingleVolume Opaque Rendering”),
  const FSceneViewFamily* ViewFamily = View->Family;                                                                    PassParameters,
                                                                                                                        ERDGPassFlags::Raster,
  const FScene* Scene = (FScene*)ViewFamily->Scene;
                                                                                                                        [this, &InView = *View, ViewportRect = Parameters.ViewportRect, Proxy, bWriteDepth, FirstLight = ApplyEmissionAndTransmittance](FRHICommandListImmediate& RHICmdList)
                                                                                                                           // 添加绘制事件以便在RDG中直观的调试
 if (!Proxy || !Proxy->GetMaterial() || !Proxy->IsVisible(View) || !Proxy->GetDensityRenderResource() || !Proxy-          SCOPED_DRAW_EVENTF(RHICmdList, StatSingleVolume, TEXT(“SingleVolume Main Pass %s”), *Proxy->GetOwnerName().ToString());
>GetDensityRenderResource()->IsInitialized())                                                                              SCOPED_GPU_STAT(RHICmdList, StatSingleVolume);
    return;
                                                                                                                                RHICmdList.SetViewport(ViewportRect.Min.X, ViewportRect.Min.Y, 0.0f, ViewportRect.Max.X, ViewportRect.Max.Y, 1.0f);
                                                                                                                                RHICmdList.SetScissorRect(false, 0,0,0,0);
    // 渲染器依赖引擎版的分支
                                                                                                                                FRHITextureViewCache TexCache;
     // ***                                                                                                                     DrawDynamicMeshPass(InView, RHICmdList,
                                                                                                                                 [&](FDynamicPassMeshDrawListContext* DynamicMeshPassContext)
    // 渲染器的独立版支持分支                                                                                                                 const FBoxSphereBounds LocalBoxSphereBounds = Proxy->GetLocalBounds();
     RenderSingleVolumeInternal(                                                                                                   FParticipationMediumElementData ShaderElementData;
                                                                                                                                   ShaderElementData.LocalBoundsOrigin = FVector4f(FVector3f(LocalBoxSphereBounds.Origin), 1.0);
    // Object Data
                                                                                                                                   ShaderElementData.LocalBoundsExtent = FVector4f(FVector3f(LocalBoxSphereBounds.BoxExtent), 1.0);
    Proxy,                                                                                                                         ShaderElementData.RenderFlagsData = Proxy->GetRenderFlagsData();
    Translucent,                                                                                                                   ShaderElementData.RaymarchData = Proxy->GetRaymarchData();
                                                                                                                                   ShaderElementData.ScatterData = Proxy->GetScatterData();
    // Light Data                                                                                                                  ShaderElementData.LightingData = Proxy->GetLightingData();
    true,                                                                                                                          ShaderElementData.EmissionData = Proxy->GetEmissionData();
                                                                                                                                   ShaderElementData.WindFieldData = Proxy->GetWindFieldData();
    true,                                                                                                                          ShaderElementData.CustomFloatData1 = Proxy->GetCustomFloatData1();
    true,                                                                                                                          ShaderElementData.CustomFloatData2 = Proxy->GetCustomFloatData2();
                                                                                                                                         // 统一以SRV，UAV方式传递数据，以便后续集成VDB
    nullptr,                                                                                                                             FTexture* Density = Proxy->GetDensityRenderResource();
    nullptr,                                                                                                                             FTextureRHIRef DensityRHI = Density ? Density->GetTextureRHI() : nullptr;
                                                                                                                                         ShaderElementData.DensityBufferSRV = DensityRHI ? FShaderResourceViewRHIRef(TexCache.GetOrCreateSRV(RHICmdList, DensityRHI, FRHITextureSRVCreateInfo())) : GBlackTextureWithSRV->ShaderResourceViewRHI;
    // Scene Data
                                                                                                                                          InView.Family->Scene->GetRenderScene(), &InView,DynamicMeshPassContext,
    // Output                                                                                                                             Proxy->IsSignedDistance(),Proxy->IsSparseVolumeTexture(), Proxy->UseCloudVolumeMod(), Proxy->UseFogVolumeMod(), Proxy->UseStandardVolumeMod(),Proxy->IsBlackBodyMaterial(),
    DepthRenderTexture                                                                                                                    MoveTemp(ShaderElementData));
    );                                                                                                                                   // 获取当前View 下的 MeshBatch，执行 Add Mesh Batch 方法
                                                                                                                                         if (FMeshBatch* VolumeMesh = Proxy->GetMeshFromView(&InView))
    // Disable any depth test / write after first lighting pass                                                                             const uint64 DefaultBatchElementMask = ~0ull;
    DepthRenderTexture = nullptr;                                                                                                           PassMeshProcessor.AddMeshBatch(*VolumeMesh,DefaultBatchElementMask,Proxy);
}                                                                                                                                    }
```

## 第 41 页：网格绘制管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 41 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
网格绘制管线
理解MeshPassProcessor(网格管道处理器) ：从MeshBatch和SceneProxy过滤，分拣信息，整合目标Pass的绘制数据



Pass过滤：MeshBatch可能会参与到不同Pass的渲染，不同的Pass需要的信息有相同的部分，但更多情况下并不相同，去掉冗余


AddMeshBatch：从MaterialRenderProxy获取FMaterial与FMaterialShaderMap，依据FMaterial设置管线“BlendMode, ShadingModel，   Pass抽取->
MeshDrawPolicy，RasterizerFillMode, RasterizeCullMode” 等，这些数据会与MeshBatchElements和VertexFactory,
PrimitiveSceneProxy等一同传入Process函数                                                                          材质抽取->
Process：通过ShaderType和VertexFactory二级过滤从FMaterialShaderMap拣出FMaterialShader，设置                              顶点工厂抽取
MeshMaterialShaderElementData, 自此数据已经全部分拣准备好，送入BuildMeshDrawCommands构建MeshDrawCommand


BuildMeshDrawCommands : 创建一个MeshDrawCommand, 为其设置渲染管线状态，对着色器数据进行Binding，执行VertexStream，遍
历MeshElement获取ElementData




AddMeshBatch主要负责Material过滤，Process会进行下一级信息细化（Shader抽取，MeshMaterialData设置）
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
理解MeshPassProcessor(网格管道处理器) ：从MeshBatch和SceneProxy过滤，分拣信息，整合目标Pass的绘制数据
AddMeshBatch：从MaterialRenderProxy获取FMaterial与FMaterialShaderMap，依据FMaterial设置管线“BlendMode, ShadingModel，   Pass抽取->
PrimitiveSceneProxy等一同传入Process函数                                                                          材质抽取->
```

## 第 42 页：class FSingleVolumeMeshProcessor : public FMeshPassProcessor

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 42 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                               class FSingleVolumeMeshProcessor : public FMeshPassProcessor




网格绘制管线
                                               {
                                               public:
                                                 FSingleVolumeMeshProcessor(
                                                   const FScene* Scene,
                                                   const FSceneView* InView,
                                                   FMeshPassDrawListContext* InDrawListContext,
                                                   bool IsSignedDistance, bool IsSparseVolumeTexture,
                                                   bool IsCloudVolume, bool IsFogVolume, bool IsStandardVolume,



设计针对单体积网格的“网格管道处理器”
                                                   bool IsBlackBody,
                                                   bool WriteDepth,
                                                   bool FirstLight,
                                                   FParticipationMediumElementData&& ShaderElementData)
                                                   : FMeshPassProcessor(Scene, Scene->GetFeatureLevel(), InView, InDrawListContext)
                                                   , PMShaderElementData(ShaderElementData)
                                                   , bSignedDistance(IsSignedDistance)
                                                   , bSparseVolumeTexture(IsSparseVolumeTexture)
                                                   , bCloudVolume(IsCloudVolume)
                * 前七个参数分别是:                        , bFogVolume(IsFogVolume)
                * 颜色写入蒙版-CW_RGBA                   , bStandardVolume(IsStandardVolume)
                * 颜色混合蒙版-BO_Add                    , bBlackBody(IsBlackBody)
                * 颜色源混合因子-BF_One : F =           {
                (1,1,1)                            if (FirstLight)
                * 颜色目标混合因子-                        {
                BF_InverseSource1Alpha :                    // 预乘Alpha混合
                F=(1-a_{src},1-a_{src},1-                  PassDrawRenderState.SetBlendState(TStaticBlendState<CW_RGBA, BO_Add, BF_One, BF_InverseSource1Alpha, BO_Add, BF_One, BF_InverseSource1Alpha>::GetRHI());
                a_{src})                           }
                F=(1−asrc,1−asrc,1−asrc)           else
                * Alpha混合运算-BO_Add                 {
                * Alpha源混合因子-BF_One : F                     // 仅添加光贡献
                =1                                         PassDrawRenderState.SetBlendState(TStaticBlendState<CW_RGBA, BO_Add, BF_One, BF_One, BO_Add, BF_Zero, BF_One>::GetRHI());
                * Alpha目标混合因子 -                    }
                F_InverseSource1Alpha : F=1-
                a_{src},F=1−asrc                        /**
                                                         * 设置深度&模板状态
                                                         */
                                                        if (WriteDepth)
                                                        {
                                                           PassDrawRenderState.SetDepthStencilState(TStaticDepthStencilState<true, CF_DepthNearOrEqual>::GetRHI());
                                                        }
                                                        else
                                                        {
                                                           PassDrawRenderState.SetDepthStencilState(TStaticDepthStencilState<false, CF_DepthNearOrEqual>::GetRHI());
                                                        }
                                                    }

                                                        // 覆写一个AddMeshBatch方法


                                                                                                                                                                                                  处理MaterialRenderProxy
                                                        virtual void AddMeshBatch(const FMeshBatch& RESTRICT MeshBatch, uint64 BatchElementMask, const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy, int32 StaticMeshId = -1) override final；


                                               private:

                                                    template<typename VertexShaderType, typename PixelShaderType>
                                                    void Process(
                                                      const FMeshBatch& MeshBatch, uint64 BatchElementMask,
                                                      const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
                                                      const FMaterialRenderProxy& RESTRICT MaterialRenderProxy,
                                                      const FMaterial& RESTRICT MaterialResource,


                                                                                                                                                                                                  分拣FShader
                                                      int32 StaticMeshId,
                                                      ERasterizerFillMode MeshFillMode,ERasterizerCullMode MeshCullMode)；

                                                    FMeshPassProcessorRenderState PassDrawRenderState;
                                                    FParticipationMediumElementData PMShaderElementData;
                                                    bool bSignedDistance;
                                                    bool bSparseVolumeTexture;
                                                    bool bCloudVolume;
                                                    bool bFogVolume;
                                                    bool bStandardVolume;
                                                    bool bBlackBody;
                                               };
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                                               class FSingleVolumeMeshProcessor : public FMeshPassProcessor
                                                   const FScene* Scene,
                                                   const FSceneView* InView,
                                                   FMeshPassDrawListContext* InDrawListContext,
                                                   bool IsSignedDistance, bool IsSparseVolumeTexture,
                                                   bool IsCloudVolume, bool IsFogVolume, bool IsStandardVolume,
                                                   bool IsBlackBody,
                                                   bool WriteDepth,
                                                   bool FirstLight,
                                                   : FMeshPassProcessor(Scene, Scene->GetFeatureLevel(), InView, InDrawListContext)
                                                   , PMShaderElementData(ShaderElementData)
                                                   , bSignedDistance(IsSignedDistance)
                                                   , bSparseVolumeTexture(IsSparseVolumeTexture)
                                                   , bCloudVolume(IsCloudVolume)
                * 前七个参数分别是:                        , bFogVolume(IsFogVolume)
                * 颜色写入蒙版-CW_RGBA                   , bStandardVolume(IsStandardVolume)
                * 颜色混合蒙版-BO_Add                    , bBlackBody(IsBlackBody)
                * 颜色源混合因子-BF_One : F =           {
                (1,1,1)                            if (FirstLight)
                * 颜色目标混合因子-                        {
                BF_InverseSource1Alpha :                    // 预乘Alpha混合
                F=(1-a_{src},1-a_{src},1-                  PassDrawRenderState.SetBlendState(TStaticBlendState<CW_RGBA, BO_Add, BF_One, BF_InverseSource1Alpha, BO_Add, BF_One, BF_InverseSource1Alpha>::GetRHI());
                a_{src})                           }
                F=(1−asrc,1−asrc,1−asrc)           else
                * Alpha混合运算-BO_Add                 {
                * Alpha源混合因子-BF_One : F                     // 仅添加光贡献
                =1                                         PassDrawRenderState.SetBlendState(TStaticBlendState<CW_RGBA, BO_Add, BF_One, BF_One, BO_Add, BF_Zero, BF_One>::GetRHI());
                * Alpha目标混合因子 -                    }
                F_InverseSource1Alpha : F=1-
                a_{src},F=1−asrc                        /**
                                                        if (WriteDepth)
                                                           PassDrawRenderState.SetDepthStencilState(TStaticDepthStencilState<true, CF_DepthNearOrEqual>::GetRHI());
                                                        else
                                                           PassDrawRenderState.SetDepthStencilState(TStaticDepthStencilState<false, CF_DepthNearOrEqual>::GetRHI());
                                                        // 覆写一个AddMeshBatch方法
                                                        virtual void AddMeshBatch(const FMeshBatch& RESTRICT MeshBatch, uint64 BatchElementMask, const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy, int32 StaticMeshId = -1) override final；
                                                    void Process(
                                                      const FMeshBatch& MeshBatch, uint64 BatchElementMask,
                                                      const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
                                                      const FMaterialRenderProxy& RESTRICT MaterialRenderProxy,
                                                      const FMaterial& RESTRICT MaterialResource,
                                                    FMeshPassProcessorRenderState PassDrawRenderState;
                                                    FParticipationMediumElementData PMShaderElementData;
                                                    bool bSignedDistance;
                                                    bool bSparseVolumeTexture;
                                                    bool bCloudVolume;
                                                    bool bFogVolume;
                                                    bool bStandardVolume;
                                                    bool bBlackBody;
```

## 第 43 页：网格绘制管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 43 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
网格绘制管线
重写AddMeshBatch方法（二级处理）：Material的过滤，获取FMaterial，光栅化状态设置，将数据送入下一环节
virtual void AddMeshBatch(
  const FMeshBatch& RESTRICT MeshBatch,
  uint64 BatchElementMask,
  const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
  int32 StaticMeshId = -1) override final

                                                                                                             从MeshBatch的MaterialRenderProxy获取FMaterial
{
  const FMaterialRenderProxy* MaterialRenderProxy = MeshBatch.MaterialRenderProxy;
  const FMaterial* Material = MaterialRenderProxy->GetMaterialNoFallback(FeatureLevel);

    if (Material && Material->GetMaterialDomain() == MD_Volume && Material->GetRenderingThreadShaderMap())   对MeshBatch内容进行二级处理，即“大象无形-三参量模型” 的第二级过滤
    {

                                                                                                             设置管线状态
       const ERasterizerFillMode MeshFillMode = FM_Solid;
       const ERasterizerCullMode MeshCullMode = MeshBatch.ReverseCulling ? CM_CW : CM_CCW;

        #define PROCESS_SHADER(shader) {Process<FSingleVolumeShaderVS, ##shader>(MeshBatch, BatchElementMask, PrimitiveSceneProxy, *MaterialRenderProxy, *Material, StaticMeshId, MeshFillMode, MeshCullMode);}

         // 使用网格距离场做为形状剖面
        if (bSignedDistance)
         {
            // 当前仅为云支持网格距离场大型
                 if (bCloudVolume && !bFogVolume && !bStandardVolume && !bBlackBody)
               PROCESS_SHADER(FSingleCloudVolumeShadingPS_Sdf)
         }
         // 使用SVT提供形状剖面
        else if(bSparseVolumeTexture)
         {
            // 仅有标准体积能够支持VDB，需要区分是否包含黑体
                 if(!bCloudVolume && !bFogVolume && bStandardVolume && !bBlackBody)
               PROCESS_SHADER(FSingleStandardVolumeShaderPS_SVT)
            else if(!bCloudVolume && !bFogVolume && bStandardVolume && bBlackBody)
               PROCESS_SHADER(FSingleStandardVolumeShaderPS_SVT)
         }
         // 使用常规密度水平集做为形状剖面
        else
         {
            // 常规云，云介质无需考虑黑体辐射
                 if (bCloudVolume && !bFogVolume && !bStandardVolume && !bBlackBody)
               PROCESS_SHADER(FSingleCloudVolumeShadingPS_LevelSet)
            // 常规雾，雾/干冰/水蒸气均不考虑黑体辐射
                 else if (!bCloudVolume && bFogVolume && !bStandardVolume && !bBlackBody)
               PROCESS_SHADER(FSingleFogVolumeShadingPS_LevelSet)
            // 常规标准体积，不确定介质类型，可能存在黑体辐射
                 else if (!bCloudVolume && !bFogVolume && bStandardVolume && !bBlackBody)
               PROCESS_SHADER(FSingleStandardVolumeShadingPS_LevelSet)
            else if (!bCloudVolume && !bFogVolume && bStandardVolume && bBlackBody)
               PROCESS_SHADER(FSingleStandardVolumeShadingPS_LevelSet)
         }
    }
}
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
virtual void AddMeshBatch(
  const FMeshBatch& RESTRICT MeshBatch,
  const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
  int32 StaticMeshId = -1) override final
  const FMaterialRenderProxy* MaterialRenderProxy = MeshBatch.MaterialRenderProxy;
  const FMaterial* Material = MaterialRenderProxy->GetMaterialNoFallback(FeatureLevel);
    if (Material && Material->GetMaterialDomain() == MD_Volume && Material->GetRenderingThreadShaderMap())   对MeshBatch内容进行二级处理，即“大象无形-三参量模型” 的第二级过滤
       const ERasterizerFillMode MeshFillMode = FM_Solid;
       const ERasterizerCullMode MeshCullMode = MeshBatch.ReverseCulling ? CM_CW : CM_CCW;
        #define PROCESS_SHADER(shader) {Process<FSingleVolumeShaderVS, ##shader>(MeshBatch, BatchElementMask, PrimitiveSceneProxy, *MaterialRenderProxy, *Material, StaticMeshId, MeshFillMode, MeshCullMode);}
         // 使用网格距离场做为形状剖面
        if (bSignedDistance)
            // 当前仅为云支持网格距离场大型
                 if (bCloudVolume && !bFogVolume && !bStandardVolume && !bBlackBody)
               PROCESS_SHADER(FSingleCloudVolumeShadingPS_Sdf)
         // 使用SVT提供形状剖面
        else if(bSparseVolumeTexture)
            // 仅有标准体积能够支持VDB，需要区分是否包含黑体
                 if(!bCloudVolume && !bFogVolume && bStandardVolume && !bBlackBody)
               PROCESS_SHADER(FSingleStandardVolumeShaderPS_SVT)
            else if(!bCloudVolume && !bFogVolume && bStandardVolume && bBlackBody)
               PROCESS_SHADER(FSingleStandardVolumeShaderPS_SVT)
         // 使用常规密度水平集做为形状剖面
        else
            // 常规云，云介质无需考虑黑体辐射
                 if (bCloudVolume && !bFogVolume && !bStandardVolume && !bBlackBody)
               PROCESS_SHADER(FSingleCloudVolumeShadingPS_LevelSet)
            // 常规雾，雾/干冰/水蒸气均不考虑黑体辐射
                 else if (!bCloudVolume && bFogVolume && !bStandardVolume && !bBlackBody)
               PROCESS_SHADER(FSingleFogVolumeShadingPS_LevelSet)
            // 常规标准体积，不确定介质类型，可能存在黑体辐射
                 else if (!bCloudVolume && !bFogVolume && bStandardVolume && !bBlackBody)
               PROCESS_SHADER(FSingleStandardVolumeShadingPS_LevelSet)
            else if (!bCloudVolume && !bFogVolume && bStandardVolume && bBlackBody)
               PROCESS_SHADER(FSingleStandardVolumeShadingPS_LevelSet)
```

## 第 44 页：网格绘制管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 44 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
网格绘制管线
重写Process方法（三级处理）：分拣PassShaders, 初始化MeshMaterialShaderElementData
template<typename VertexShaderType, typename PixelShaderType>
void Process(
  const FMeshBatch& MeshBatch,
  uint64 BatchElementMask,
  const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
  const FMaterialRenderProxy& RESTRICT MaterialRenderProxy,
  const FMaterial& RESTRICT MaterialResource,
  int32 StaticMeshId,

                                                                                                                                     初始化需要Binding的MeshMaterialShaderElementData，这些着色器实
  ERasterizerFillMode MeshFillMode,
  ERasterizerCullMode MeshCullMode)

                                                                                                                                     参随后被送入BuildMeshDrawCommands，在那里依据ShaderType分类
{
  // 初始化网格材质数据

                                                                                                                                     回调GetShaderBindings函数绑定到对应的着色器参量
    PMShaderElementData.InitializeMeshMaterialData(ViewIfDynamicMeshCommand, PrimitiveSceneProxy, MeshBatch, StaticMeshId, false);

    // 获取顶点工厂
      const FVertexFactory* VertexFactory = MeshBatch.VertexFactory;

    // 二级索引获取FShader
    TMeshProcessorShaders<VertexShaderType, PixelShaderType> PassShaders;

                                                                                                                                     依据顶点工厂和着色器类筛选FMaterialShaders
    FMaterialShaderTypes ShaderTypes;
    ShaderTypes.AddShaderType<VertexShaderType>();
    ShaderTypes.AddShaderType<PixelShaderType>();
    FMaterialShaders Shaders;

                                                                                                                                     即“大象无形-三参量模型” 的最后一级过滤
    if (!MaterialResource.TryGetShaders(ShaderTypes, VertexFactory->GetType(), Shaders))
    {
       return;
    }
    Shaders.TryGetVertexShader(PassShaders.VertexShader);
    Shaders.TryGetPixelShader(PassShaders.PixelShader);

    // 计算SortKey并调用"BuildMeshDrawCommands"构建网格绘制命令
     const FMeshDrawCommandSortKey SortKey = CalculateMeshStaticSortKey(PassShaders.VertexShader, PassShaders.PixelShader);
    BuildMeshDrawCommands(
       MeshBatch,
       BatchElementMask,
       PrimitiveSceneProxy,

                                                                                                                                     将渲染资源，绑定的着色器参数，Shader，管线状态一同送入
       MaterialRenderProxy,
       MaterialResource,

                                                                                                                                     BuildMeshDrawCommands生成该Pass使用的紧凑且CPU访问友好的
       PassDrawRenderState,
       PassShaders,

                                                                                                                                     MeshDrawCommand
       MeshFillMode,
       MeshCullMode,
       SortKey,
       EMeshPassFeatures::Default,
       PMShaderElementData
       );
}
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
void Process(
  const FMeshBatch& MeshBatch,
  const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
  const FMaterialRenderProxy& RESTRICT MaterialRenderProxy,
  const FMaterial& RESTRICT MaterialResource,
  // 初始化网格材质数据
    PMShaderElementData.InitializeMeshMaterialData(ViewIfDynamicMeshCommand, PrimitiveSceneProxy, MeshBatch, StaticMeshId, false);
    // 获取顶点工厂
      const FVertexFactory* VertexFactory = MeshBatch.VertexFactory;
    // 二级索引获取FShader
    TMeshProcessorShaders<VertexShaderType, PixelShaderType> PassShaders;
    FMaterialShaderTypes ShaderTypes;
    ShaderTypes.AddShaderType<VertexShaderType>();
    ShaderTypes.AddShaderType<PixelShaderType>();
    FMaterialShaders Shaders;
    if (!MaterialResource.TryGetShaders(ShaderTypes, VertexFactory->GetType(), Shaders))
       return;
    Shaders.TryGetVertexShader(PassShaders.VertexShader);
    Shaders.TryGetPixelShader(PassShaders.PixelShader);
    // 计算SortKey并调用"BuildMeshDrawCommands"构建网格绘制命令
     const FMeshDrawCommandSortKey SortKey = CalculateMeshStaticSortKey(PassShaders.VertexShader, PassShaders.PixelShader);
       EMeshPassFeatures::Default,
```

## 第 45 页：网格绘制管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 45 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
网格绘制管线
理解MeshDrawCommand




只包含了该Pass需要去绘制的数据，一种更紧凑，更简洁，更有利于CPU访问的数据结构：包含了一个DrawCall所需的最少资源集合
```

## 第 46 页：网格绘制管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 46 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
网格绘制管线
Processor创建与AddMeshBatch调用链




                              SetupMeshPass : 遍历所有的Mesh Pass，调用
                              FPassProcessorManager::CreateMeshPassProcessor创建对应类型的Mesh Pass Processor

                              DispatchPassSetup : 数据打包填充进FMeshDrawCommandPassSetupTaskContext
                              TaskContext，交换内存，正确分配Pass设置

                              AnyThreadTask : 并行任务，调用GenerateDynamicMeshDrawCommands

                              GenerateDynamicMeshDrawCommands : 将搜集到的静态MeshBatch和动态MeshBatch分
                              别生成对应Pass的MeshDrawCommand。该函数并没有区分Pass类别，因此具体的转译工作
                              是通过回调MeshPassProcessor的AddMeshBatch方法完成的。每一类MeshPassProcessor
                              都有自己的AddMeshBatch方法，通过派生自定义网格处理器类和覆写AddMeshBatch方法实
                              现不同的转译主体


                              BuildMeshDrawCommands : 这个是所有MeshPassProcessor通用的，将1个MeshBatch翻译
                              为1个或多个MeshDrawCommands
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                              FPassProcessorManager::CreateMeshPassProcessor创建对应类型的Mesh Pass Processor
```

## 第 47 页：class FSingleVolumeShaderPS : public FMeshMaterialShader

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 47 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                                                                           class FSingleVolumeShaderPS : public FMeshMaterialShader



网格绘制管线
                                                                                           {
                                                                                             // 为该MeshMaterialShader声明内联的着色器参数布局
                                                                                             DECLARE_INLINE_TYPE_LAYOUT(FSingleVolumeShaderPS, NonVirtual);

                                                                                                // 着色器参数结构体
                                                                                                BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)


MeshPass着色器类的声明与定义
                                                                                                   // 引入一个完成构造的UniformBuffer成员
                                                                                                   SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
                                                                                                  // 引入一个RDG Uniform Buffer 成员
                                                                                                   SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSingleVolumeShaderParams, SingleVolumeUniformBuffer)
                                                                                                   // 光栅化管线所绑定的Render Target槽
                                                                                                   RENDER_TARGET_BINDING_SLOTS()
                                                                                                END_SHADER_PARAMETER_STRUCT()
class FSingleVolumeShaderVS : public FMeshMaterialShader
{                                                                                               // C++ Shader参数和 hlsl Shader参数一一对应起来
  DECLARE_SHADER_TYPE(FSingleVolumeShaderVS, MeshMaterial);                                      FSingleVolumeShaderPS() = default;
                                                                                                FSingleVolumeShaderPS(const FMeshMaterialShaderType::CompiledShaderInitializerType& Initializer)
 FSingleVolumeShaderVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)           : FMeshMaterialShader(Initializer)
  : FMeshMaterialShader(Initializer){}                                                          {
 FSingleVolumeShaderVS() = default;                                                                // C++参数与HLSL参数的Binding
                                                                                                   DensityBufferSRV.Bind(Initializer.ParameterMap, TEXT(“ShapeProfile”));
public:                                                                                         }

 static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters&      public:
Parameters)                                                                                   // 逐MeshBatch的着色器参数绑定，该函数会在BuildMeshDrawCommands中依ShaderType进行分类回调，参考MeshMaterialShader中的写法
 {                                                                                            void GetShaderBindings(
   return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5) &&              const FScene* Scene,
     Parameters.MaterialParameters.MaterialDomain == MD_Volume &&                             ERHIFeatureLevel::Type FeatureLevel,
     FMeshMaterialShader::ShouldCompilePermutation(Parameters) &&                             const FPrimitiveSceneProxy* PrimitiveSceneProxy,
     SingleVolumeShaders::IsSupportedVertexFactoryType(Parameters.VertexFactoryType);         const FMaterialRenderProxy& MaterialRenderProxy,
 }                                                                                            const FMaterial& Material,
                                                                                              const FMeshPassProcessorRenderState& DrawRenderState,
   static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters&       const FParticipationMediumElementData& ShaderElementData,
Parameters, FShaderCompilerEnvironment& OutEnvironment)                                       FMeshDrawSingleShaderBindings& ShaderBindings)const
   {                                                                                        {
     FMeshMaterialShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);           FMeshMaterialShader::GetShaderBindings(Scene, FeatureLevel, PrimitiveSceneProxy, MaterialRenderProxy, Material, DrawRenderState, ShaderElementData, ShaderBindings);
   }
};                                                                                                  // 在这里追加额外的Bindings项
                                                                                                    ShaderBindings.Add(DensityBufferSRV, ShaderElementData.DensityBufferSRV);
                                                                                                }

                                                                                                // Parameter Map 编译排序

顶点着色器类 & 像素着色器类（Shader Frequency）
                                                                                                 static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
                                                                                                {
                                                                                                   return FSingleVolumeShaderVS::ShouldCompilePermutation(Parameters);
                                                                                                }

从FMeshMaeterialShader派生（Shader Type）                                                             // 着色器编译环境，在这里关联着色器中的宏状态
                                                                                                 static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
                                                                                                {
                                                                                                   FMeshMaterialShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
                                                                                                }

                                                                                           private:
                                                                                             // 着色器类的参数布局，有两种类型：FShaderParameters 与 FShaderResourceParameters
                                                                                             LAYOUT_FIELD(FShaderResourceParameter, DensityBufferSRV);

                                                                                           };
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                                                                                           class FSingleVolumeShaderPS : public FMeshMaterialShader
                                                                                             // 为该MeshMaterialShader声明内联的着色器参数布局
                                                                                             DECLARE_INLINE_TYPE_LAYOUT(FSingleVolumeShaderPS, NonVirtual);
                                                                                                // 着色器参数结构体
                                                                                                BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
                                                                                                   // 引入一个完成构造的UniformBuffer成员
                                                                                                   SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
                                                                                                  // 引入一个RDG Uniform Buffer 成员
                                                                                                   SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSingleVolumeShaderParams, SingleVolumeUniformBuffer)
                                                                                                   // 光栅化管线所绑定的Render Target槽
                                                                                                   RENDER_TARGET_BINDING_SLOTS()
                                                                                                END_SHADER_PARAMETER_STRUCT()
class FSingleVolumeShaderVS : public FMeshMaterialShader
{                                                                                               // C++ Shader参数和 hlsl Shader参数一一对应起来
  DECLARE_SHADER_TYPE(FSingleVolumeShaderVS, MeshMaterial);                                      FSingleVolumeShaderPS() = default;
                                                                                                FSingleVolumeShaderPS(const FMeshMaterialShaderType::CompiledShaderInitializerType& Initializer)
 FSingleVolumeShaderVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)           : FMeshMaterialShader(Initializer)
  : FMeshMaterialShader(Initializer){}                                                          {
 FSingleVolumeShaderVS() = default;                                                                // C++参数与HLSL参数的Binding
                                                                                                   DensityBufferSRV.Bind(Initializer.ParameterMap, TEXT(“ShapeProfile”));
public:                                                                                         }
 static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters&      public:
Parameters)                                                                                   // 逐MeshBatch的着色器参数绑定，该函数会在BuildMeshDrawCommands中依ShaderType进行分类回调，参考MeshMaterialShader中的写法
 {                                                                                            void GetShaderBindings(
   return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5) &&              const FScene* Scene,
     Parameters.MaterialParameters.MaterialDomain == MD_Volume &&                             ERHIFeatureLevel::Type FeatureLevel,
     FMeshMaterialShader::ShouldCompilePermutation(Parameters) &&                             const FPrimitiveSceneProxy* PrimitiveSceneProxy,
     SingleVolumeShaders::IsSupportedVertexFactoryType(Parameters.VertexFactoryType);         const FMaterialRenderProxy& MaterialRenderProxy,
 }                                                                                            const FMaterial& Material,
                                                                                              const FMeshPassProcessorRenderState& DrawRenderState,
   static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters&       const FParticipationMediumElementData& ShaderElementData,
Parameters, FShaderCompilerEnvironment& OutEnvironment)                                       FMeshDrawSingleShaderBindings& ShaderBindings)const
   {                                                                                        {
     FMeshMaterialShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);           FMeshMaterialShader::GetShaderBindings(Scene, FeatureLevel, PrimitiveSceneProxy, MaterialRenderProxy, Material, DrawRenderState, ShaderElementData, ShaderBindings);
};                                                                                                  // 在这里追加额外的Bindings项
                                                                                                    ShaderBindings.Add(DensityBufferSRV, ShaderElementData.DensityBufferSRV);
                                                                                                // Parameter Map 编译排序
                                                                                                 static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
                                                                                                   return FSingleVolumeShaderVS::ShouldCompilePermutation(Parameters);
从FMeshMaeterialShader派生（Shader Type）                                                             // 着色器编译环境，在这里关联着色器中的宏状态
                                                                                                 static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
                                                                                                   FMeshMaterialShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
                                                                                             // 着色器类的参数布局，有两种类型：FShaderParameters 与 FShaderResourceParameters
                                                                                             LAYOUT_FIELD(FShaderResourceParameter, DensityBufferSRV);
```

## 第 48 页：网格绘制管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 48 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
网格绘制管线
着色器参数的输入类型，上传更新与绑定类型


输入类型
●   Shader Parameters：静态Material Instance Constant, 动态Material Instance Dynamic，底层被设计为每个Material实例独立的UniformBuffer, 使用FUniformExpressionCache
    数据结构来描述
●   Material Parameter Collection：多材质共享，底层被设计为全局的UniformBuffer，每个Shader引用的MPC索引由FUniformExpressionCache记录
●   顶点或像素着色器输入：管线层数据，这里的数据输入方式更灵活，可以自行选择Binding机制
●   全局输入：多个Shader或多个Pass共享的数据一般用过UniformBuffer输入
●   Vertex Input （由Input Stream负责，不包含在Shader Binding范畴）




绑定类型
●   Uniform Buffer：分离式设计，记录实例分配的引用。例如ViewUniformBuffer, PrimitiveUniformBuffer, MaterialUniformBuffer
●   Sampler：分离式设计，记录实际分配的引用
●   UVA, SRV：分离式设计，记录实例分配的引用

●   Loose Data（松散数据）：直接Binding，多数为不轻易改变的纹理或常量

●   Vertex Input：严格来说顶点输入由Input Stream负责，不包含在Shader Binding范畴
```

## 第 49 页：网格绘制管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 49 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
网格绘制管线
统一缓冲区（UniformBuffer）与着色器参数结构的区别

UniformBuffer                                                                                           #define BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT \
                                                                                                          BEGIN_UNIFORM_BUFFER_STRUCT



 BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FShaderUniformParams,)                                            #define BEGIN_UNIFORM_BUFFER_STRUCT(StructTypeName, PrefixKeywords) \
                                                                                                          DECLARE_UNIFORM_BUFFER_STRUCT(StructTypeName, PrefixKeywords) \
  SHADER_PARAMETER_RDG_TEXTURE(Texture2D, Texture)                                                        INTERNAL_SHADER_PARAMETER_STRUCT_BEGIN(StructTypeName,PrefixKeywords,{},
  SHADER_PARAMETER_SAMPLER(SamplerState, TextureSampler)                                                  INTERNAL_BEGIN_UNIFORM_BUFFER_STRUCT, INTERNAL_UNIFORM_BUFFER_STRUCT_GET_STRUCT_METADATA(StructTypeName), INTERNAL_UNIFORM_BUFFER_STRUCT_CREATE_UNIFORM_BUFFER)
  SHADER_PARAMETER_TEXTURE(Texture3D, VolumeTexture)
  SHADER_PARAMETER_SAMPLER(SamplerState, VolumeTextureSampler)
  SHADER_PARAMETER_SRV(Buffer, Buffer)                                                                  #define END_GLOBAL_SHADER_PARAMETER_STRUCT \
  SHADER_PARAMETER(float, Scaler)                                                                         END_UNIFORM_BUFFER_STRUCT
  SHADER_PARAMETER(int, Int)
 END_GLOBAL_SHADER_PARAMETER_STRUCT()
                                                                                                        #define END_UNIFORM_BUFFER_STRUCT() \
                                                                                                          END_SHADER_PARAMETER_STRUCT()


                                                                                                        #define IMPLEMENT_STATIC_UNIFORM_BUFFER_STRUCT(StructTypeName,ShaderVariableName,StaticSlotName) \
 IMPLEMENT_STATIC_UNIFORM_BUFFER_STRUCT(FShaderUniformParams, "ShaderUniformParams", DeferredDecals);     IMPLEMENT_STATIC_UNIFORM_BUFFER_STRUCT_EX(StructTypeName, ShaderVariableName, StaticSlotName, Static)




ShaderParameterStruct                                                                                   #define BEGIN_SHADER_PARAMETER_STRUCT(StructTypeName, PrefixKeywords) \
                                                                                                          INTERNAL_SHADER_PARAMETER_STRUCT_BEGIN(StructTypeName, PrefixKeywords, {},
                                                                                                          INTERNAL_SHADER_PARAMETER_GET_STRUCT_METADATA(StructTypeName), INTERNAL_SHADER_PARAMETER_STRUCT_CREATE_UNIFORM_BUFFER)
 BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
  // Uniform Buffer
                                                                                                        #define SHADER_PARAMETER_RDG_UNIFORM_BUFFER(StructType, MemberName) \
  SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)                                         INTERNAL_SHADER_PARAMETER_EXPLICIT(UBMT_RDG_UNIFORM_BUFFER, TShaderParameterTypeInfo<TRDGUniformBufferBinding<StructType>>,
  SHADER_PARAMETER_STRUCT_INCLUDE(FShaderParametersStruct, ShaderParameterStruct)                         TRDGUniformBufferBinding<StructType>,MemberName,,,EShaderPrecisionModifier::Float,TEXT(#StructType),false)
  SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FShaderUniformParams, ShaderUniformBuffer)
  // Shader Parameter Member
  SHADER_PARAMETER(float, Scaler)
                                                                                                        #define RENDER_TARGET_BINDING_SLOTS() \
  SHADER_PARAMETER(int, Int)
                                                                                                          INTERNAL_SHADER_PARAMETER_EXPLICIT(UBMT_RENDER_TARGET_BINDING_SLOTS, FRenderTargetBindingSlots::FTypeInfo, FRenderTargetBindingSlots,RenderTargets,,,EShaderPrecisionModifier::Float,TEXT(""),false)
  // Binding RenderTarget, for Raster Pass
  RENDER_TARGET_BINDING_SLOTS()
 END_SHADER_PARAMETER_STRUCT()                                                                          #define END_SHADER_PARAMETER_STRUCT() \
                                                                                                            zzLastMemberId; \
                                                                                                          public: \
                                                                                                            static TArray<FShaderParametersMetadata::FMember> zzGetMembers() { \
                                                                                                              TArray<FShaderParametersMetadata::FMember> Members; \
                                                                                                              zzFuncPtr(*LastFunc)(zzLastMemberId, TArray<FShaderParametersMetadata::FMember>*); \
                                                                                                              LastFunc = zzAppendMemberGetPrev; \
                                                                                                              zzFuncPtr Ptr = (zzFuncPtr)LastFunc; \
                                                                                                              do \
                                                                                                              {\
                                                                                                                 Ptr = reinterpret_cast<zzMemberFunc>(Ptr)(zzFirstMemberId(), &Members); \
                                                                                                              } while (Ptr); \
                                                                                                              Algo::Reverse(Members); \
                                                                                                              return Members; \
                                                                                                            }\
                                                                                                          } GCC_ALIGN(SHADER_PARAMETER_STRUCT_ALIGNMENT);
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
UniformBuffer                                                                                           #define BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT \
 BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FShaderUniformParams,)                                            #define BEGIN_UNIFORM_BUFFER_STRUCT(StructTypeName, PrefixKeywords) \
                                                                                                          DECLARE_UNIFORM_BUFFER_STRUCT(StructTypeName, PrefixKeywords) \
  SHADER_PARAMETER_RDG_TEXTURE(Texture2D, Texture)                                                        INTERNAL_SHADER_PARAMETER_STRUCT_BEGIN(StructTypeName,PrefixKeywords,{},
  SHADER_PARAMETER_SAMPLER(SamplerState, TextureSampler)                                                  INTERNAL_BEGIN_UNIFORM_BUFFER_STRUCT, INTERNAL_UNIFORM_BUFFER_STRUCT_GET_STRUCT_METADATA(StructTypeName), INTERNAL_UNIFORM_BUFFER_STRUCT_CREATE_UNIFORM_BUFFER)
  SHADER_PARAMETER_TEXTURE(Texture3D, VolumeTexture)
  SHADER_PARAMETER_SAMPLER(SamplerState, VolumeTextureSampler)
  SHADER_PARAMETER_SRV(Buffer, Buffer)                                                                  #define END_GLOBAL_SHADER_PARAMETER_STRUCT \
  SHADER_PARAMETER(float, Scaler)                                                                         END_UNIFORM_BUFFER_STRUCT
  SHADER_PARAMETER(int, Int)
 END_GLOBAL_SHADER_PARAMETER_STRUCT()
                                                                                                        #define END_UNIFORM_BUFFER_STRUCT() \
                                                                                                          END_SHADER_PARAMETER_STRUCT()
                                                                                                        #define IMPLEMENT_STATIC_UNIFORM_BUFFER_STRUCT(StructTypeName,ShaderVariableName,StaticSlotName) \
 IMPLEMENT_STATIC_UNIFORM_BUFFER_STRUCT(FShaderUniformParams, "ShaderUniformParams", DeferredDecals);     IMPLEMENT_STATIC_UNIFORM_BUFFER_STRUCT_EX(StructTypeName, ShaderVariableName, StaticSlotName, Static)
ShaderParameterStruct                                                                                   #define BEGIN_SHADER_PARAMETER_STRUCT(StructTypeName, PrefixKeywords) \
                                                                                                          INTERNAL_SHADER_PARAMETER_STRUCT_BEGIN(StructTypeName, PrefixKeywords, {},
                                                                                                          INTERNAL_SHADER_PARAMETER_GET_STRUCT_METADATA(StructTypeName), INTERNAL_SHADER_PARAMETER_STRUCT_CREATE_UNIFORM_BUFFER)
 BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
  // Uniform Buffer
                                                                                                        #define SHADER_PARAMETER_RDG_UNIFORM_BUFFER(StructType, MemberName) \
  SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)                                         INTERNAL_SHADER_PARAMETER_EXPLICIT(UBMT_RDG_UNIFORM_BUFFER, TShaderParameterTypeInfo<TRDGUniformBufferBinding<StructType>>,
  SHADER_PARAMETER_STRUCT_INCLUDE(FShaderParametersStruct, ShaderParameterStruct)                         TRDGUniformBufferBinding<StructType>,MemberName,,,EShaderPrecisionModifier::Float,TEXT(#StructType),false)
  SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FShaderUniformParams, ShaderUniformBuffer)
  // Shader Parameter Member
  SHADER_PARAMETER(float, Scaler)
                                                                                                        #define RENDER_TARGET_BINDING_SLOTS() \
  SHADER_PARAMETER(int, Int)
                                                                                                          INTERNAL_SHADER_PARAMETER_EXPLICIT(UBMT_RENDER_TARGET_BINDING_SLOTS, FRenderTargetBindingSlots::FTypeInfo, FRenderTargetBindingSlots,RenderTargets,,,EShaderPrecisionModifier::Float,TEXT(""),false)
  // Binding RenderTarget, for Raster Pass
  RENDER_TARGET_BINDING_SLOTS()
 END_SHADER_PARAMETER_STRUCT()                                                                          #define END_SHADER_PARAMETER_STRUCT() \
                                                                                                            zzLastMemberId; \
                                                                                                            static TArray<FShaderParametersMetadata::FMember> zzGetMembers() { \
                                                                                                              TArray<FShaderParametersMetadata::FMember> Members; \
                                                                                                              zzFuncPtr(*LastFunc)(zzLastMemberId, TArray<FShaderParametersMetadata::FMember>*); \
                                                                                                              LastFunc = zzAppendMemberGetPrev; \
                                                                                                              zzFuncPtr Ptr = (zzFuncPtr)LastFunc; \
                                                                                                                 Ptr = reinterpret_cast<zzMemberFunc>(Ptr)(zzFirstMemberId(), &Members); \
                                                                                                              } while (Ptr); \
                                                                                                              Algo::Reverse(Members); \
                                                                                                              return Members; \
                                                                                                          } GCC_ALIGN(SHADER_PARAMETER_STRUCT_ALIGNMENT);
```

## 第 50 页：网格绘制管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 50 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
网格绘制管线
着色器参数Binding策略
                                                                                    void GetShaderBindings(
// Participating media Uniform Buffer
                                                                                      const FScene* Scene,
BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FSingleVolumeShaderParams,)
                                                                                      ERHIFeatureLevel::Type FeatureLevel,
   // Scene Data
                                                                                      const FPrimitiveSceneProxy* PrimitiveSceneProxy,
   SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneDepthTexture)
                                                                                      const FMaterialRenderProxy& MaterialRenderProxy,
   SHADER_PARAMETER_SAMPLER(SamplerState, LinearTexSampler)
                                                                                      const FMaterial& Material,
   // Misc
                                                                                      const FMeshPassProcessorRenderState& DrawRenderState,
   SHADER_PARAMETER_TEXTURE(Texture3D, DetailNoiseTex)                                                                                                                                  // C++ Shader参数和 hlsl Shader参数一一对应起来
                                                                                      const FParticipationMediumElementData& ShaderElementData,
   SHADER_PARAMETER_SAMPLER(SamplerState, DetailNoiseTexSampler)                                                                                                                        FSingleVolumeShaderPS() = default;
                                                                                      FMeshDrawSingleShaderBindings& ShaderBindings)const
   SHADER_PARAMETER(float, DistanceMipLevel)                                                                                                                                            FSingleVolumeShaderPS(const
                                                                                    {
   // Light Data                                                                                                                                                                        FMeshMaterialShaderType::CompiledShaderInitializerType& Initializer)
                                                                                      FMeshMaterialShader::GetShaderBindings(Scene, FeatureLevel, PrimitiveSceneProxy,
   SHADER_PARAMETER(int, bApplyEmissionAndTransmittance)                                                                                                                                   : FMeshMaterialShader(Initializer)
                                                                                      MaterialRenderProxy, Material, DrawRenderState, ShaderElementData, ShaderBindings);
   SHADER_PARAMETER(int, bApplyDirectLighting)                                                                                                                                          {
   SHADER_PARAMETER(int, bApplyShadowTransmittance)                                                                                                                                        DensityBufferSRV.Bind(Initializer.ParameterMap, TEXT("ShapeProfile"));
                                                                                        // 在这里追加额外的Bindings
   SHADER_PARAMETER(int, LightType)                                                                                                                                                        LocalBoundsOrigin.Bind(Initializer.ParameterMap, TEXT("LocalBoundsOrigin"));
                                                                                        ShaderBindings.Add(DensityBufferSRV, ShaderElementData.DensityBufferSRV);
   // SVT                                                                                                                                                                                  LocalBoundsExtent.Bind(Initializer.ParameterMap, TEXT("LocalBoundsExtent"));
                                                                                        ShaderBindings.Add(LocalBoundsOrigin, ShaderElementData.LocalBoundsOrigin);
   SHADER_PARAMETER_SAMPLER(SamplerState, TileDataTextureSampler)                                                                                                                          RenderFlagsData.Bind(Initializer.ParameterMap, TEXT("RenderFlagsData"));
                                                                                        ShaderBindings.Add(LocalBoundsExtent, ShaderElementData.LocalBoundsExtent);
   SHADER_PARAMETER_TEXTURE(Texture3D<uint>, SparseVolumeTexturePageTable)                                                                                                                 RaymarchData.Bind(Initializer.ParameterMap, TEXT("RaymarchData"));
                                                                                        ShaderBindings.Add(RenderFlagsData, ShaderElementData.RenderFlagsData);
   SHADER_PARAMETER_TEXTURE(Texture3D, SparseVolumeTextureA)                                                                                                                               ScatterData.Bind(Initializer.ParameterMap, TEXT("ScatterData"));
                                                                                        ShaderBindings.Add(RaymarchData, ShaderElementData.RaymarchData);
   SHADER_PARAMETER_TEXTURE(Texture3D, SparseVolumeTextureB)                                                                                                                               LightingData.Bind(Initializer.ParameterMap, TEXT("LightingData"));
                                                                                        ShaderBindings.Add(ScatterData, ShaderElementData.ScatterData);
   SHADER_PARAMETER_SRV(ByteAddressBuffer, StreamingInfoBuffer)                                                                                                                            EmissionData.Bind(Initializer.ParameterMap, TEXT("EmissionData"));
                                                                                        ShaderBindings.Add(LightingData,ShaderElementData.LightingData);
   SHADER_PARAMETER(FUintVector4, PackedSVTUniforms0)                                                                                                                                      WindFieldData.Bind(Initializer.ParameterMap, TEXT("WindFieldData"));
                                                                                        ShaderBindings.Add(EmissionData, ShaderElementData.EmissionData);
   SHADER_PARAMETER(FUintVector4, PackedSVTUniforms1)                                                                                                                                      CustomFloatData1.Bind(Initializer.ParameterMap, TEXT("CustomFloatData1"));
                                                                                        ShaderBindings.Add(WindFieldData, ShaderElementData.WindFieldData);
   SHADER_PARAMETER(FVector3f, SparseVolumeTextureResolution)                                                                                                                              CustomFloatData2.Bind(Initializer.ParameterMap, TEXT("CustomFloatData2"));
                                                                                        ShaderBindings.Add(CustomFloatData1,ShaderElementData.CustomFloatData1);
   SHADER_PARAMETER(int32, MipLevel)                                                                                                                                                    }
                                                                                        ShaderBindings.Add(CustomFloatData2,ShaderElementData.CustomFloatData2);
   SHADER_PARAMETER(int, SVTChannel)
                                                                                    }
END_GLOBAL_SHADER_PARAMETER_STRUCT()


                                                                                          属性常量
                                                                                                                struct FParticipationMediumElementData : public FMeshMaterialShaderElementData

资源配置
                                                                                                                {
                                                                                                                   FVector4f LocalBoundsOrigin;                                                                                          float4 LocalBoundsOrigin;
                                                                                                                   FVector4f LocalBoundsExtent;                                                                                          float4 LocalBoundsExtent;
                                                                                                                   FIntVector4 RenderFlagsData; // x: NoiseMod, y: SamplePerPixel, z: Ambient, w: 1                                      int4 RenderFlagsData;
// 创建 Uniform Buffer                                                                                               FVector4f RaymarchData;        // x: ViewSteps, y: ShadowSteps, z: DensityMultiplier, w: ShadowMultiplier             float4 RaymarchData;
BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)                                                                        FVector4f ScatterData;     // x: MsScatterOrder, y: MsScatterFactor, z: MsExtinctionFactor, w: MsPhaseFactor          float4 ScatterData;
   // Scene Data                                                                                                   FVector4f LightingData;    // x: AnisotropyPrime, y: AnisotropySecondary, z: Weight, w: ShadowThreshold               float4 LightingData;
   SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)                                                 FVector4f EmissionData;      // x: Strength, y: Kelvin Temperature, z: BlackBody Intensity, w: Dimension              float4 EmissionData;
   // Uniform Buffer                                                                                               FVector4f WindFieldData;      // xyz: Scroll w: Wind Strength                                                         float4 WindFieldData;
   SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSingleVolumeShaderParams, SingleVolumeUniformBuffer)                       FVector4f CustomFloatData1;      // xyz: Color1, w: ColorAdd                                                          float4 CustomFloatData1;
   // Binding RenderTarget, for Raster Pass                                                                        FVector4f CustomFloatData2;      // xyz: Color2, w: ColorMul                                                          float4 CustomFloatData2;
   RENDER_TARGET_BINDING_SLOTS()                                                                                   FShaderResourceViewRHIRef DensityBufferSRV;
END_SHADER_PARAMETER_STRUCT()                                                                                   };                                                                                                                       Texture3D<float> ShapeProfile;
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                                                                                    void GetShaderBindings(
// Participating media Uniform Buffer
                                                                                      const FScene* Scene,
BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FSingleVolumeShaderParams,)
                                                                                      ERHIFeatureLevel::Type FeatureLevel,
   // Scene Data
                                                                                      const FPrimitiveSceneProxy* PrimitiveSceneProxy,
   SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneDepthTexture)
                                                                                      const FMaterialRenderProxy& MaterialRenderProxy,
   SHADER_PARAMETER_SAMPLER(SamplerState, LinearTexSampler)
                                                                                      const FMaterial& Material,
   // Misc
                                                                                      const FMeshPassProcessorRenderState& DrawRenderState,
   SHADER_PARAMETER_TEXTURE(Texture3D, DetailNoiseTex)                                                                                                                                  // C++ Shader参数和 hlsl Shader参数一一对应起来
                                                                                      const FParticipationMediumElementData& ShaderElementData,
   SHADER_PARAMETER_SAMPLER(SamplerState, DetailNoiseTexSampler)                                                                                                                        FSingleVolumeShaderPS() = default;
                                                                                      FMeshDrawSingleShaderBindings& ShaderBindings)const
   SHADER_PARAMETER(float, DistanceMipLevel)                                                                                                                                            FSingleVolumeShaderPS(const
   // Light Data                                                                                                                                                                        FMeshMaterialShaderType::CompiledShaderInitializerType& Initializer)
                                                                                      FMeshMaterialShader::GetShaderBindings(Scene, FeatureLevel, PrimitiveSceneProxy,
   SHADER_PARAMETER(int, bApplyEmissionAndTransmittance)                                                                                                                                   : FMeshMaterialShader(Initializer)
                                                                                      MaterialRenderProxy, Material, DrawRenderState, ShaderElementData, ShaderBindings);
   SHADER_PARAMETER(int, bApplyDirectLighting)                                                                                                                                          {
   SHADER_PARAMETER(int, bApplyShadowTransmittance)                                                                                                                                        DensityBufferSRV.Bind(Initializer.ParameterMap, TEXT("ShapeProfile"));
                                                                                        // 在这里追加额外的Bindings
   SHADER_PARAMETER(int, LightType)                                                                                                                                                        LocalBoundsOrigin.Bind(Initializer.ParameterMap, TEXT("LocalBoundsOrigin"));
                                                                                        ShaderBindings.Add(DensityBufferSRV, ShaderElementData.DensityBufferSRV);
   // SVT                                                                                                                                                                                  LocalBoundsExtent.Bind(Initializer.ParameterMap, TEXT("LocalBoundsExtent"));
                                                                                        ShaderBindings.Add(LocalBoundsOrigin, ShaderElementData.LocalBoundsOrigin);
   SHADER_PARAMETER_SAMPLER(SamplerState, TileDataTextureSampler)                                                                                                                          RenderFlagsData.Bind(Initializer.ParameterMap, TEXT("RenderFlagsData"));
                                                                                        ShaderBindings.Add(LocalBoundsExtent, ShaderElementData.LocalBoundsExtent);
   SHADER_PARAMETER_TEXTURE(Texture3D<uint>, SparseVolumeTexturePageTable)                                                                                                                 RaymarchData.Bind(Initializer.ParameterMap, TEXT("RaymarchData"));
                                                                                        ShaderBindings.Add(RenderFlagsData, ShaderElementData.RenderFlagsData);
   SHADER_PARAMETER_TEXTURE(Texture3D, SparseVolumeTextureA)                                                                                                                               ScatterData.Bind(Initializer.ParameterMap, TEXT("ScatterData"));
                                                                                        ShaderBindings.Add(RaymarchData, ShaderElementData.RaymarchData);
   SHADER_PARAMETER_TEXTURE(Texture3D, SparseVolumeTextureB)                                                                                                                               LightingData.Bind(Initializer.ParameterMap, TEXT("LightingData"));
                                                                                        ShaderBindings.Add(ScatterData, ShaderElementData.ScatterData);
   SHADER_PARAMETER_SRV(ByteAddressBuffer, StreamingInfoBuffer)                                                                                                                            EmissionData.Bind(Initializer.ParameterMap, TEXT("EmissionData"));
                                                                                        ShaderBindings.Add(LightingData,ShaderElementData.LightingData);
   SHADER_PARAMETER(FUintVector4, PackedSVTUniforms0)                                                                                                                                      WindFieldData.Bind(Initializer.ParameterMap, TEXT("WindFieldData"));
                                                                                        ShaderBindings.Add(EmissionData, ShaderElementData.EmissionData);
   SHADER_PARAMETER(FUintVector4, PackedSVTUniforms1)                                                                                                                                      CustomFloatData1.Bind(Initializer.ParameterMap, TEXT("CustomFloatData1"));
                                                                                        ShaderBindings.Add(WindFieldData, ShaderElementData.WindFieldData);
   SHADER_PARAMETER(FVector3f, SparseVolumeTextureResolution)                                                                                                                              CustomFloatData2.Bind(Initializer.ParameterMap, TEXT("CustomFloatData2"));
                                                                                        ShaderBindings.Add(CustomFloatData1,ShaderElementData.CustomFloatData1);
   SHADER_PARAMETER(int32, MipLevel)                                                                                                                                                    }
                                                                                        ShaderBindings.Add(CustomFloatData2,ShaderElementData.CustomFloatData2);
   SHADER_PARAMETER(int, SVTChannel)
END_GLOBAL_SHADER_PARAMETER_STRUCT()
                                                                                                                struct FParticipationMediumElementData : public FMeshMaterialShaderElementData
                                                                                                                   FVector4f LocalBoundsOrigin;                                                                                          float4 LocalBoundsOrigin;
                                                                                                                   FVector4f LocalBoundsExtent;                                                                                          float4 LocalBoundsExtent;
                                                                                                                   FIntVector4 RenderFlagsData; // x: NoiseMod, y: SamplePerPixel, z: Ambient, w: 1                                      int4 RenderFlagsData;
// 创建 Uniform Buffer                                                                                               FVector4f RaymarchData;        // x: ViewSteps, y: ShadowSteps, z: DensityMultiplier, w: ShadowMultiplier             float4 RaymarchData;
BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)                                                                        FVector4f ScatterData;     // x: MsScatterOrder, y: MsScatterFactor, z: MsExtinctionFactor, w: MsPhaseFactor          float4 ScatterData;
   // Scene Data                                                                                                   FVector4f LightingData;    // x: AnisotropyPrime, y: AnisotropySecondary, z: Weight, w: ShadowThreshold               float4 LightingData;
   SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)                                                 FVector4f EmissionData;      // x: Strength, y: Kelvin Temperature, z: BlackBody Intensity, w: Dimension              float4 EmissionData;
   // Uniform Buffer                                                                                               FVector4f WindFieldData;      // xyz: Scroll w: Wind Strength                                                         float4 WindFieldData;
   SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSingleVolumeShaderParams, SingleVolumeUniformBuffer)                       FVector4f CustomFloatData1;      // xyz: Color1, w: ColorAdd                                                          float4 CustomFloatData1;
   // Binding RenderTarget, for Raster Pass                                                                        FVector4f CustomFloatData2;      // xyz: Color2, w: ColorMul                                                          float4 CustomFloatData2;
   RENDER_TARGET_BINDING_SLOTS()                                                                                   FShaderResourceViewRHIRef DensityBufferSRV;
END_SHADER_PARAMETER_STRUCT()                                                                                   };                                                                                                                       Texture3D<float> ShapeProfile;
```

## 第 51 页：网格绘制管线

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 51 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
网格绘制管线
着色器类的模板化
template<bool IsSignedDistance, bool IsVDB, bool UseCloudVolume, bool UseFogVolume, bool UseStandardVolume, bool IsBlackBody>
class TSingleVolumeShaderPS : public FSingleVolumeShaderPS
{
  DECLARE_SHADER_TYPE(TSingleVolumeShaderPS, MeshMaterial);

     TSingleVolumeShaderPS() = default;
     TSingleVolumeShaderPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
       : FSingleVolumeShaderPS(Initializer)
     {
       BindForLegacyShaderParameters<FParameters>(this, Initializer.PermutationId, Initializer.ParameterMap, false);
     }

public:
 static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
 {
   return FSingleVolumeShaderPS::ShouldCompilePermutation(Parameters);
 }

     static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
     {
       FSingleVolumeShaderPS::ModifyCompilationEnvironment(Parameters, OutEnvironment);
       OutEnvironment.SetDefine(TEXT("USE_SIGNED_DISTANCE"), IsSignedDistance);
       OutEnvironment.SetDefine(TEXT("USE_VDB_GRID"), IsVDB);
       OutEnvironment.SetDefine(TEXT("USE_CLOUD_VOLUME"), UseCloudVolume);
       OutEnvironment.SetDefine(TEXT("USE_FOG_VOLUME"), UseFogVolume);
       OutEnvironment.SetDefine(TEXT("USE_STANDARD_VOLUME"), UseStandardVolume);
       OutEnvironment.SetDefine(TEXT("USE_BLACKBOY"), IsBlackBody);
     }
};



typedef TSingleVolumeShaderPS<true, false, true, false, false, false> FSingleCloudVolumeShadingPS_Sdf;
typedef TSingleVolumeShaderPS<false, true, false, false, true, true> FSingleStandardVolumeShaderPS_SVT;
typedef TSingleVolumeShaderPS<false, false, true, false, false, false> FSingleCloudVolumeShadingPS_LevelSet;
typedef TSingleVolumeShaderPS<false, false,false, true, false, false> FSingleFogVolumeShadingPS_LevelSet;
typedef TSingleVolumeShaderPS<false, false, false, false, true, true> FSingleStandardVolumeShadingPS_LevelSet;



IMPLEMENT_MATERIAL_SHADER_TYPE(template<>, FSingleCloudVolumeShadingPS_Sdf, TEXT("/Plugin/SingleVolume/Private/CloudVolumeShadingPipeline.usf"), TEXT("RenderMultiScatteringVolumePS"), SF_Pixel);
IMPLEMENT_MATERIAL_SHADER_TYPE(template<>, FSingleStandardVolumeShaderPS_SVT, TEXT("/Plugin/SingleVolume/Private/StandardVolumeShadingPipeline.usf"), TEXT("RenderPrincipledScatteringVolumePS"), SF_Pixel);
IMPLEMENT_MATERIAL_SHADER_TYPE(template<>, FSingleCloudVolumeShadingPS_LevelSet, TEXT("/Plugin/SingleVolume/Private/CloudVolumeShadingPipeline.usf"), TEXT("RenderMultiScatteringVolumePS"), SF_Pixel);
IMPLEMENT_MATERIAL_SHADER_TYPE(template<>, FSingleFogVolumeShadingPS_LevelSet, TEXT("/Plugin/SingleVolume/Private/FogVolumeShadingPipeline.usf"), TEXT("RenderSingleScatteringVolumePS"), SF_Pixel);
IMPLEMENT_MATERIAL_SHADER_TYPE(template<>, FSingleStandardVolumeShadingPS_LevelSet, TEXT("/Plugin/SingleVolume/Private/StandardVolumeShadingPipeline.usf"), TEXT("RenderPrincipledScatteringVolumePS"), SF_Pixel);
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
template<bool IsSignedDistance, bool IsVDB, bool UseCloudVolume, bool UseFogVolume, bool UseStandardVolume, bool IsBlackBody>
class TSingleVolumeShaderPS : public FSingleVolumeShaderPS
  DECLARE_SHADER_TYPE(TSingleVolumeShaderPS, MeshMaterial);
     TSingleVolumeShaderPS() = default;
     TSingleVolumeShaderPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
       : FSingleVolumeShaderPS(Initializer)
       BindForLegacyShaderParameters<FParameters>(this, Initializer.PermutationId, Initializer.ParameterMap, false);
 static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
   return FSingleVolumeShaderPS::ShouldCompilePermutation(Parameters);
     static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
       FSingleVolumeShaderPS::ModifyCompilationEnvironment(Parameters, OutEnvironment);
       OutEnvironment.SetDefine(TEXT("USE_SIGNED_DISTANCE"), IsSignedDistance);
       OutEnvironment.SetDefine(TEXT("USE_VDB_GRID"), IsVDB);
       OutEnvironment.SetDefine(TEXT("USE_CLOUD_VOLUME"), UseCloudVolume);
       OutEnvironment.SetDefine(TEXT("USE_FOG_VOLUME"), UseFogVolume);
       OutEnvironment.SetDefine(TEXT("USE_STANDARD_VOLUME"), UseStandardVolume);
       OutEnvironment.SetDefine(TEXT("USE_BLACKBOY"), IsBlackBody);
typedef TSingleVolumeShaderPS<true, false, true, false, false, false> FSingleCloudVolumeShadingPS_Sdf;
typedef TSingleVolumeShaderPS<false, true, false, false, true, true> FSingleStandardVolumeShaderPS_SVT;
typedef TSingleVolumeShaderPS<false, false, true, false, false, false> FSingleCloudVolumeShadingPS_LevelSet;
typedef TSingleVolumeShaderPS<false, false,false, true, false, false> FSingleFogVolumeShadingPS_LevelSet;
typedef TSingleVolumeShaderPS<false, false, false, false, true, true> FSingleStandardVolumeShadingPS_LevelSet;
IMPLEMENT_MATERIAL_SHADER_TYPE(template<>, FSingleCloudVolumeShadingPS_Sdf, TEXT("/Plugin/SingleVolume/Private/CloudVolumeShadingPipeline.usf"), TEXT("RenderMultiScatteringVolumePS"), SF_Pixel);
IMPLEMENT_MATERIAL_SHADER_TYPE(template<>, FSingleStandardVolumeShaderPS_SVT, TEXT("/Plugin/SingleVolume/Private/StandardVolumeShadingPipeline.usf"), TEXT("RenderPrincipledScatteringVolumePS"), SF_Pixel);
IMPLEMENT_MATERIAL_SHADER_TYPE(template<>, FSingleCloudVolumeShadingPS_LevelSet, TEXT("/Plugin/SingleVolume/Private/CloudVolumeShadingPipeline.usf"), TEXT("RenderMultiScatteringVolumePS"), SF_Pixel);
IMPLEMENT_MATERIAL_SHADER_TYPE(template<>, FSingleFogVolumeShadingPS_LevelSet, TEXT("/Plugin/SingleVolume/Private/FogVolumeShadingPipeline.usf"), TEXT("RenderSingleScatteringVolumePS"), SF_Pixel);
IMPLEMENT_MATERIAL_SHADER_TYPE(template<>, FSingleStandardVolumeShadingPS_LevelSet, TEXT("/Plugin/SingleVolume/Private/StandardVolumeShadingPipeline.usf"), TEXT("RenderPrincipledScatteringVolumePS"), SF_Pixel);
```

## 第 52 页：全屏混合管线 void SingleVolumeComposite::CompositeFullScreen(FRDGBuilder& GraphBuilder,

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 52 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
全屏混合管线                                                                                                                                            void SingleVolumeComposite::CompositeFullScreen(FRDGBuilder& GraphBuilder,



                                                                                                                                                  {
                                                                                                                                                    FRDGTexture* InputTexture, FRDGTexture* OutTexture,
                                                                                                                                                    FRDGTexture* InDepthTexture, FRDGTexture* OutDepthTexture,
                                                                                                                                                    const FSceneView* View, bool ForceClear, bool ForceRegularAlpha)




设计用于混合的“全屏绘制管道”
                                                                                                                                                    static uint32 LastFrame = 0;

                                                                                                                                                      // 安全措施，不管用户输入多少都限定在 0， 1， 2
                                                                                                                                                      int DebugDisplayMode = CVarSingleVolumeCompositeDebugMode.GetValueOnRenderThread();
                                                                                                                                                      DebugDisplayMode = FMath::Clamp(DebugDisplayMode, 0,2);

                                                                                                                                                      // 清空 BackBuffer 的条件
                                                                                                                                                         bool Clear = false;
                                                                                                                                                      if (LastFrame != View->Family->FrameNumber)
// 这个GlobalShader参考UE内置体积云的写法                                                                                                                         {
class FSingleVolumeCompositePS : public FGlobalShader                                                                                                    LastFrame = View->Family->FrameNumber;
{                                                                                                                                                        Clear = DebugDisplayMode > 0;
public:                                                                                                                                               }
   DECLARE_GLOBAL_SHADER(FSingleVolumeCompositePS);                                                                                                   Clear |= ForceClear;
   SHADER_USE_PARAMETER_STRUCT(FSingleVolumeCompositePS, FGlobalShader);
                                                                                                                                                      FIntRect ViewPort(FIntPoint(0, 0), OutTexture->Desc.Extent);
     class FDisplayMethod : SHADER_PERMUTATION_INT("DEBUG_DISPLAY", 3);
     class FUseDepth : SHADER_PERMUTATION_BOOL("USE_DEPTH");                                                                                          // 由于参数较少，这里我们不再单独封装函数
     using FPermutationDomain = TShaderPermutationDomain<FDisplayMethod, FUseDepth>;                                                                   // 调用“GraphBuilder.AllocParameters”为着色器参数分配内存
                                                                                                                                                       FSingleVolumeCompositePS::FParameters* PassParameters = GraphBuilder.AllocParameters<FSingleVolumeCompositePS::FParameters>();
     static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)                                                       PassParameters->ViewUniformBuffer = View->ViewUniformBuffer;
     {                                                                                                                                                PassParameters->InputTexture = InputTexture;
       return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);                                                                    PassParameters->InputSampler = TStaticSamplerState<SF_Point, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
     }                                                                                                                                                PassParameters->DepthTexture = InDepthTexture;
                                                                                                                                                      PassParameters->RenderTargets[0] = FRenderTargetBinding(OutTexture, Clear ? ERenderTargetLoadAction::EClear : ERenderTargetLoadAction::ELoad);
     static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)       if (InDepthTexture)
     {                                                                                                                                                {
       FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);                                                                           PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(
     }                                                                                                                                                      OutDepthTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthWrite_StencilNop);
                                                                                                                                                      }
     BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
      SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, ViewUniformBuffer)                                                                    // 变体设置
      SHADER_PARAMETER_RDG_TEXTURE(Texture2D, InputTexture)                                                                                            FSingleVolumeCompositePS::FPermutationDomain PermutationVector;
      SHADER_PARAMETER_SAMPLER(SamplerState, InputSampler)                                                                                            PermutationVector.Set<FSingleVolumeCompositePS::FDisplayMethod>(DebugDisplayMode);
      SHADER_PARAMETER_RDG_TEXTURE(Texture2D, DepthTexture)                                                                                           PermutationVector.Set<FSingleVolumeCompositePS::FUseDepth>(InDepthTexture != nullptr);
      // ...
      RENDER_TARGET_BINDING_SLOTS()                                                                                                                   // 设置着色器图
     END_SHADER_PARAMETER_STRUCT()                                                                                                                     FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
};                                                                                                                                                    TShaderMapRef<FSingleVolumeCompositePS> PixelShader(GlobalShaderMap, PermutationVector);

IMPLEMENT_SHADER_TYPE(, FSingleVolumeCompositePS, TEXT("/Plugin/SingleVolume/Private/SingleVolumeComposite.usf"), TEXT("MainPS"), SF_Pixel)           // 设置渲染管线状态, ForceRegularAlpha默认为 true，深度模板状态设定为仅写入最近深度
                                                                                                                                                       FRHIBlendState* BlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_One, BF_InverseSourceAlpha, BO_ReverseSubtract, BF_DestAlpha, BF_One>::GetRHI();
                                                                                                                                                      if (ForceRegularAlpha)
                                                                                                                                                      {
#define SHADER_USE_PARAMETER_STRUCT(ShaderClass, ShaderParentClass) \                                                                                    BlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_One, BF_InverseSourceAlpha, BO_Add, BF_One, BF_InverseSourceAlpha>::GetRHI();
  SHADER_USE_PARAMETER_STRUCT_INTERNAL(ShaderClass, ShaderParentClass, true) \                                                                        }
  \                                                                                                                                                   FRHIDepthStencilState* DepthStencilState = TStaticDepthStencilState<true, CF_DepthNearOrEqual>::GetRHI();
  static inline const FShaderParametersMetadata* GetRootParametersMetadata() { return FParameters::FTypeInfo::GetStructMetadata(); }
                                                                                                                                                      // 添加一个全屏Pass
                                                                                                                                                      FPixelShaderUtils::AddFullscreenPass(
                                                                                                                                                         GraphBuilder,
#define SHADER_USE_PARAMETER_STRUCT_INTERNAL(ShaderClass, ShaderParentClass, bShouldBindEverything) \
                                                                                                                                                         GlobalShaderMap,
  ShaderClass(const ShaderMetaType::CompiledShaderInitializerType& Initializer) \
                                                                                                                                                         RDG_EVENT_NAME("SingleVolumeRendering.Composite %dx%d (PS)", ViewPort.Width(), ViewPort.Height()),
    : ShaderParentClass(Initializer) \
                                                                                                                                                         PixelShader,
  {\
                                                                                                                                                         PassParameters,
    BindForLegacyShaderParameters<FParameters>(this, Initializer.PermutationId, Initializer.ParameterMap, bShouldBindEverything); \
                                                                                                                                                         ViewPort,
  }\
                                                                                                                                                         BlendState,
  \
                                                                                                                                                         nullptr,
  ShaderClass() \
                                                                                                                                                         InDepthTexture ? DepthStencilState : nullptr);
  {}\
                                                                                                                                                  }




充分复用RenderGraphUtils.cpp中的实用程序可以帮助我们减少样板代
码和提高开发速度与稳健性
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
全屏混合管线                                                                                                                                            void SingleVolumeComposite::CompositeFullScreen(FRDGBuilder& GraphBuilder,
                                                                                                                                                    FRDGTexture* InputTexture, FRDGTexture* OutTexture,
                                                                                                                                                    FRDGTexture* InDepthTexture, FRDGTexture* OutDepthTexture,
                                                                                                                                                    const FSceneView* View, bool ForceClear, bool ForceRegularAlpha)
                                                                                                                                                    static uint32 LastFrame = 0;
                                                                                                                                                      // 安全措施，不管用户输入多少都限定在 0， 1， 2
                                                                                                                                                      int DebugDisplayMode = CVarSingleVolumeCompositeDebugMode.GetValueOnRenderThread();
                                                                                                                                                      DebugDisplayMode = FMath::Clamp(DebugDisplayMode, 0,2);
                                                                                                                                                      // 清空 BackBuffer 的条件
                                                                                                                                                         bool Clear = false;
                                                                                                                                                      if (LastFrame != View->Family->FrameNumber)
// 这个GlobalShader参考UE内置体积云的写法                                                                                                                         {
class FSingleVolumeCompositePS : public FGlobalShader                                                                                                    LastFrame = View->Family->FrameNumber;
{                                                                                                                                                        Clear = DebugDisplayMode > 0;
public:                                                                                                                                               }
   DECLARE_GLOBAL_SHADER(FSingleVolumeCompositePS);                                                                                                   Clear |= ForceClear;
   SHADER_USE_PARAMETER_STRUCT(FSingleVolumeCompositePS, FGlobalShader);
                                                                                                                                                      FIntRect ViewPort(FIntPoint(0, 0), OutTexture->Desc.Extent);
     class FDisplayMethod : SHADER_PERMUTATION_INT("DEBUG_DISPLAY", 3);
     class FUseDepth : SHADER_PERMUTATION_BOOL("USE_DEPTH");                                                                                          // 由于参数较少，这里我们不再单独封装函数
     using FPermutationDomain = TShaderPermutationDomain<FDisplayMethod, FUseDepth>;                                                                   // 调用“GraphBuilder.AllocParameters”为着色器参数分配内存
                                                                                                                                                       FSingleVolumeCompositePS::FParameters* PassParameters = GraphBuilder.AllocParameters<FSingleVolumeCompositePS::FParameters>();
     static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)                                                       PassParameters->ViewUniformBuffer = View->ViewUniformBuffer;
     {                                                                                                                                                PassParameters->InputTexture = InputTexture;
       return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);                                                                    PassParameters->InputSampler = TStaticSamplerState<SF_Point, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
     }                                                                                                                                                PassParameters->DepthTexture = InDepthTexture;
                                                                                                                                                      PassParameters->RenderTargets[0] = FRenderTargetBinding(OutTexture, Clear ? ERenderTargetLoadAction::EClear : ERenderTargetLoadAction::ELoad);
     static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)       if (InDepthTexture)
     {                                                                                                                                                {
       FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);                                                                           PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(
     }                                                                                                                                                      OutDepthTexture, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthWrite_StencilNop);
     BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
      SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, ViewUniformBuffer)                                                                    // 变体设置
      SHADER_PARAMETER_RDG_TEXTURE(Texture2D, InputTexture)                                                                                            FSingleVolumeCompositePS::FPermutationDomain PermutationVector;
      SHADER_PARAMETER_SAMPLER(SamplerState, InputSampler)                                                                                            PermutationVector.Set<FSingleVolumeCompositePS::FDisplayMethod>(DebugDisplayMode);
      SHADER_PARAMETER_RDG_TEXTURE(Texture2D, DepthTexture)                                                                                           PermutationVector.Set<FSingleVolumeCompositePS::FUseDepth>(InDepthTexture != nullptr);
      // ...
      RENDER_TARGET_BINDING_SLOTS()                                                                                                                   // 设置着色器图
     END_SHADER_PARAMETER_STRUCT()                                                                                                                     FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
};                                                                                                                                                    TShaderMapRef<FSingleVolumeCompositePS> PixelShader(GlobalShaderMap, PermutationVector);
IMPLEMENT_SHADER_TYPE(, FSingleVolumeCompositePS, TEXT("/Plugin/SingleVolume/Private/SingleVolumeComposite.usf"), TEXT("MainPS"), SF_Pixel)           // 设置渲染管线状态, ForceRegularAlpha默认为 true，深度模板状态设定为仅写入最近深度
                                                                                                                                                       FRHIBlendState* BlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_One, BF_InverseSourceAlpha, BO_ReverseSubtract, BF_DestAlpha, BF_One>::GetRHI();
                                                                                                                                                      if (ForceRegularAlpha)
#define SHADER_USE_PARAMETER_STRUCT(ShaderClass, ShaderParentClass) \                                                                                    BlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_One, BF_InverseSourceAlpha, BO_Add, BF_One, BF_InverseSourceAlpha>::GetRHI();
  SHADER_USE_PARAMETER_STRUCT_INTERNAL(ShaderClass, ShaderParentClass, true) \                                                                        }
  \                                                                                                                                                   FRHIDepthStencilState* DepthStencilState = TStaticDepthStencilState<true, CF_DepthNearOrEqual>::GetRHI();
  static inline const FShaderParametersMetadata* GetRootParametersMetadata() { return FParameters::FTypeInfo::GetStructMetadata(); }
                                                                                                                                                      // 添加一个全屏Pass
                                                                                                                                                      FPixelShaderUtils::AddFullscreenPass(
                                                                                                                                                         GraphBuilder,
#define SHADER_USE_PARAMETER_STRUCT_INTERNAL(ShaderClass, ShaderParentClass, bShouldBindEverything) \
  ShaderClass(const ShaderMetaType::CompiledShaderInitializerType& Initializer) \
                                                                                                                                                         RDG_EVENT_NAME("SingleVolumeRendering.Composite %dx%d (PS)", ViewPort.Width(), ViewPort.Height()),
    : ShaderParentClass(Initializer) \
    BindForLegacyShaderParameters<FParameters>(this, Initializer.PermutationId, Initializer.ParameterMap, bShouldBindEverything); \
  ShaderClass() \
                                                                                                                                                         InDepthTexture ? DepthStencilState : nullptr);
```

## 第 53 页：引入统计数据监控

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 53 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
引入统计数据监控
使用引擎统计数据系统添加监控点


// 添加SingleVolume的时间统计监控组，归属到Advanced下
DECLARE_STATS_GROUP(TEXT(“SingleVolumes”), STATGROUP_SingleVolume, STATCAT_Advanced);
// 声明时间监控点的插入点和文本名称
// 埋入RenderThread恰当位置，监控Proxies的渲染
DECLARE_CYCLE_STAT(TEXT("TotalProxies"), STAT_VolumeRenderer_RT, STATGROUP_SingleVolume);
DECLARE_CYCLE_STAT(TEXT("OpaqueProxies"), STAT_OpaqueProxies_RT, STATGROUP_SingleVolume);
DECLARE_CYCLE_STAT(TEXT("TranslucentProxies"), STAT_TranslucentProxies_RT, STATGROUP_SingleVolume);
// 埋入Scene Proxy的GetDynamicMeshElements，统计Collect过程用时
DECLARE_CYCLE_STAT(TEXT("GetDynamicMeshElements"), STAT_VolumeSceneProxy_GetDynamicMeshElements, STATGROUP_SingleVolume)




DECLARE_GPU_STAT_NAMED(StatSingleVolume, TEXT("Single Volume Rendering"));


SCOPED_DRAW_EVENTF(RHICmdList, StatSingleVolume, TEXT("SingleVolume Main Pass %s"), *Proxy->GetOwnerName().ToString());
SCOPED_GPU_STAT(RHICmdList, StatSingleVolume);




具体参见：统计数据系统概述
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
// 添加SingleVolume的时间统计监控组，归属到Advanced下
DECLARE_STATS_GROUP(TEXT(“SingleVolumes”), STATGROUP_SingleVolume, STATCAT_Advanced);
// 声明时间监控点的插入点和文本名称
// 埋入RenderThread恰当位置，监控Proxies的渲染
DECLARE_CYCLE_STAT(TEXT("TotalProxies"), STAT_VolumeRenderer_RT, STATGROUP_SingleVolume);
DECLARE_CYCLE_STAT(TEXT("OpaqueProxies"), STAT_OpaqueProxies_RT, STATGROUP_SingleVolume);
DECLARE_CYCLE_STAT(TEXT("TranslucentProxies"), STAT_TranslucentProxies_RT, STATGROUP_SingleVolume);
// 埋入Scene Proxy的GetDynamicMeshElements，统计Collect过程用时
DECLARE_CYCLE_STAT(TEXT("GetDynamicMeshElements"), STAT_VolumeSceneProxy_GetDynamicMeshElements, STATGROUP_SingleVolume)
DECLARE_GPU_STAT_NAMED(StatSingleVolume, TEXT("Single Volume Rendering"));
SCOPED_DRAW_EVENTF(RHICmdList, StatSingleVolume, TEXT("SingleVolume Main Pass %s"), *Proxy->GetOwnerName().ToString());
SCOPED_GPU_STAT(RHICmdList, StatSingleVolume);
```

## 第 54 页：VRE 的高效 GPU 渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 54 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效 GPU 渲染
      Part3
```

## 第 55 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 55 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
光线步进方式回顾

               distance in ray box       distance to ray box




                             entry pos
```

## 第 56 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 56 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
阴影步进选型




     Linear shadow distribution   Nonlinear shadow distribution   Cone shadow distribution
```

## 第 57 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 57 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
几种常见RayBox算法的实现
// ProtoType
float2 IntersectAABB_Proto(float3 RayOrigin, float3 RayDir, float3 BoundsMin, float3 BoundsMax, float RayTMin, float RayTMax)
{
   float3 InvRayDir = float3(1,1,1) / RayDir;

    float3 t0 = (BoundsMin - RayOrigin) * InvRayDir;
    float3 t1 = (BoundsMax - RayOrigin) * InvRayDir;

    float3 tMin = min(t0, t1);
    float3 tMax = max(t0, t1);

    float tN = max(max(tMin.x, tMin.y), tMin.z);
    float tF = min(min(tMax.x,tMax.y), tMax.z);

    tF = min(tF, RayTMax);

    float dstToBox = max(tN, 0.0);
    float dstInBox = max(tF - tN, 0.0);

    return float2(dstToBox, dstInBox);
}


// Ryan Brucks                                                                                            // Normal
float2 IntersectAABB_Ryan(float3 RayOrigin, float3 RayDir, float LocalSceneDepth, float MaxSteps)         bool IntersectAABB_Norm(in float3 RayOrigin, in float3 RayDir, in float3 BoundsMin, in float3 BoundsMax, inout float RayTMin, inout float RayTMax)
{                                                                                                         {
   float3 InvRayDir = float3(1,1,1) / RayDir;                                                                float3 InvRayDir = float3(1,1,1) / RayDir;

    float3 t0 = (float3(0,0,0) - RayOrigin) * InvRayDir;                                                      float3 t0 = (BoundsMin - RayOrigin) * InvRayDir;
    float3 t1 = (float3(1,1,1) - RayOrigin) * InvRayDir;                                                      float3 t1 = (BoundsMax - RayOrigin) * InvRayDir;

    float3 tMin = min(t0, t1);                                                                                float3 tMin = min(t0, t1);
    float3 tMax = max(t0, t1);                                                                                float3 tMax = max(t0, t1);

    float tN = max(tMin.x, max(tMin.y, tMin.z));                                                              float tN = max(max(tMin.x, tMin.y), tMin.z);
    float tF = min(tMax.x, min(tMax.y, tMax.z));                                                              float tF = min(min(tMax.x,tMax.y), tMax.z);

    float PlaneOffset = 1.0 - frac( ( tN - length(RayOrigin-0.5) ) * MaxSteps );                              bool Hit = tN <= tF;
    tN += (PlaneOffset / MaxSteps);
                                                                                                              RayTMin = max(tN, RayTMin);
    tF = min(tF, LocalSceneDepth);                                                                            RayTMax = min(tF, RayTMax);
    tN = max(tN, 0.0);
                                                                                                              return Hit;
    float dstToBox = max(tN, 0.0);                                                                        }
    float dstInBox = max(tF - tN, 0.0);

    return float2(dstToBox, dstInBox);
}
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
// ProtoType
float2 IntersectAABB_Proto(float3 RayOrigin, float3 RayDir, float3 BoundsMin, float3 BoundsMax, float RayTMin, float RayTMax)
   float3 InvRayDir = float3(1,1,1) / RayDir;
    float3 t0 = (BoundsMin - RayOrigin) * InvRayDir;
    float3 t1 = (BoundsMax - RayOrigin) * InvRayDir;
    float3 tMin = min(t0, t1);
    float3 tMax = max(t0, t1);
    float tN = max(max(tMin.x, tMin.y), tMin.z);
    float tF = min(min(tMax.x,tMax.y), tMax.z);
    tF = min(tF, RayTMax);
    float dstToBox = max(tN, 0.0);
    float dstInBox = max(tF - tN, 0.0);
    return float2(dstToBox, dstInBox);
// Ryan Brucks                                                                                            // Normal
float2 IntersectAABB_Ryan(float3 RayOrigin, float3 RayDir, float LocalSceneDepth, float MaxSteps)         bool IntersectAABB_Norm(in float3 RayOrigin, in float3 RayDir, in float3 BoundsMin, in float3 BoundsMax, inout float RayTMin, inout float RayTMax)
{                                                                                                         {
   float3 InvRayDir = float3(1,1,1) / RayDir;                                                                float3 InvRayDir = float3(1,1,1) / RayDir;
    float3 t0 = (float3(0,0,0) - RayOrigin) * InvRayDir;                                                      float3 t0 = (BoundsMin - RayOrigin) * InvRayDir;
    float3 t1 = (float3(1,1,1) - RayOrigin) * InvRayDir;                                                      float3 t1 = (BoundsMax - RayOrigin) * InvRayDir;
    float3 tMin = min(t0, t1);                                                                                float3 tMin = min(t0, t1);
    float3 tMax = max(t0, t1);                                                                                float3 tMax = max(t0, t1);
    float tN = max(tMin.x, max(tMin.y, tMin.z));                                                              float tN = max(max(tMin.x, tMin.y), tMin.z);
    float tF = min(tMax.x, min(tMax.y, tMax.z));                                                              float tF = min(min(tMax.x,tMax.y), tMax.z);
    float PlaneOffset = 1.0 - frac( ( tN - length(RayOrigin-0.5) ) * MaxSteps );                              bool Hit = tN <= tF;
    tN += (PlaneOffset / MaxSteps);
                                                                                                              RayTMin = max(tN, RayTMin);
    tF = min(tF, LocalSceneDepth);                                                                            RayTMax = min(tF, RayTMax);
    tN = max(tN, 0.0);
                                                                                                              return Hit;
    float dstToBox = max(tN, 0.0);                                                                        }
    float dstInBox = max(tF - tN, 0.0);
    return float2(dstToBox, dstInBox);
```

## 第 58 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 58 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
形状剖面：来自Shader Graph的输入
                         /**
                          * 转流Albedo,控制散射色
                          */
                         half3 SampleScatteringColor(in FPixelMaterialInputs PixelMaterialInputs)
                         {
                            half3 Scattering = 1.0;
                            #if STRATA_ENABLED
                              FStrataBSDF BSDF = PixelMaterialInputs.FrontMaterial.InlinedBSDF;
                              Scattering = VOLUMETRICFOGCLOUD_ALBEDO(BSDF);
                            #else
                            #if !MATERIAL_SHADINGMODEL_UNLIT
                              Scattering = GetMaterialBaseColorRaw(PixelMaterialInputs).rgb;
                            #endif
                            #endif
                            return Scattering;

                                                                                                    无论是否需要转
                         }

                                                                                                    流来自Shader
                                                                                                    Graph的数据这
                         /**
                          * 转流自发光
                          */
                                                                                                    三个似乎都是必
                                                                                                       须的？
                         half3 SampleEmissiveColor(in FPixelMaterialInputs PixelMaterialInputs)
                         {
                            #if STRATA_ENABLED
                              FStrataBSDF BSDF = PixelMaterialInputs.FrontMaterial.InlinedBSDF;
                              half3 EmissiveColor = BSDF_GETEMISSIVE(BSDF);
                            #else
                              half3 EmissiveColor = GetMaterialEmissiveRaw(PixelMaterialInputs);
                            #endif
                            return clamp(EmissiveColor, 0.0f, 65000.0f);
                         }


                         /**
                          * 接收用户在ShaderGraph中自定义的σ_t
                          */
                         float SampleExtinctionCoefficients(in FPixelMaterialInputs
                         PixelMaterialInputs)
                         {
                            float Extinction = 0.0f;
                            #if STRATA_ENABLED
                               FStrataBSDF BSDF = PixelMaterialInputs.FrontMaterial.InlinedBSDF;
                               Extinction = VOLUMETRICFOGCLOUD_EXTINCTION(BSDF).r;
                            #else
                            #if !MATERIAL_SHADINGMODEL_UNLIT
                               Extinction = GetMaterialSubsurfaceDataRaw(PixelMaterialInputs).r;
                            #endif
                            #endif
                            return clamp(Extinction, 0.0f, 65000.0f);
                         }
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                         half3 SampleScatteringColor(in FPixelMaterialInputs PixelMaterialInputs)
                            half3 Scattering = 1.0;
                            #if STRATA_ENABLED
                              FStrataBSDF BSDF = PixelMaterialInputs.FrontMaterial.InlinedBSDF;
                              Scattering = VOLUMETRICFOGCLOUD_ALBEDO(BSDF);
                            #else
                            #if !MATERIAL_SHADINGMODEL_UNLIT
                              Scattering = GetMaterialBaseColorRaw(PixelMaterialInputs).rgb;
                            #endif
                            #endif
                            return Scattering;
                         half3 SampleEmissiveColor(in FPixelMaterialInputs PixelMaterialInputs)
                            #if STRATA_ENABLED
                              FStrataBSDF BSDF = PixelMaterialInputs.FrontMaterial.InlinedBSDF;
                              half3 EmissiveColor = BSDF_GETEMISSIVE(BSDF);
                            #else
                              half3 EmissiveColor = GetMaterialEmissiveRaw(PixelMaterialInputs);
                            #endif
                            return clamp(EmissiveColor, 0.0f, 65000.0f);
                         float SampleExtinctionCoefficients(in FPixelMaterialInputs
                            float Extinction = 0.0f;
                            #if STRATA_ENABLED
                               FStrataBSDF BSDF = PixelMaterialInputs.FrontMaterial.InlinedBSDF;
                               Extinction = VOLUMETRICFOGCLOUD_EXTINCTION(BSDF).r;
                            #else
                            #if !MATERIAL_SHADINGMODEL_UNLIT
                               Extinction = GetMaterialSubsurfaceDataRaw(PixelMaterialInputs).r;
                            #endif
                            #endif
                            return clamp(Extinction, 0.0f, 65000.0f);
```

## 第 59 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 59 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
形状剖面：密度水平集和网格距离场模式



形状剖面

/**
 * 使用形状剖面来表征体积轮廓
 * 目前定义2种数据编码方式：
 * 密度的窄带水平集
 * 有符号距离场
 */
float SampleShapeProfile(float3 SamplePos, uint Mip)
{

                                                                                                        密度混合器
   float ShapeDensity = 1.0;

    #if USE_SIGNED_DISTANCE
      ShapeDensity = ShapeProfile.SampleLevel(SingleVolumeParams.LinearTexSampler, SamplePos, Mip).r;
      ShapeDensity = - (ShapeDensity – SDF_THREHOLD);
    #else
      ShapeDensity = ShapeProfile.SampleLevel(SingleVolumeParams.LinearTexSampler, SamplePos, Mip).r;
    #endif

    return ShapeDensity;
}
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
float SampleShapeProfile(float3 SamplePos, uint Mip)
   float ShapeDensity = 1.0;
    #if USE_SIGNED_DISTANCE
      ShapeDensity = ShapeProfile.SampleLevel(SingleVolumeParams.LinearTexSampler, SamplePos, Mip).r;
      ShapeDensity = - (ShapeDensity – SDF_THREHOLD);
    #else
      ShapeDensity = ShapeProfile.SampleLevel(SingleVolumeParams.LinearTexSampler, SamplePos, Mip).r;
    #endif
    return ShapeDensity;
```

## 第 60 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 60 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
形状剖面：Sparse Volume Texture 的采样

 float SampleSparseVolumeTextureDensity(float3 P)
 {
    float3 VolumeResolution = SingleVolumeParams.SparseVolumeTextureResolution;
    float3 RcpVolumeResolution = 1.0f / VolumeResolution;
    float Density = 0.0f;

      // 根据现在的包围盒最小边长进行映射变换
     float3 LocalBoundsExtent = GetLocalBoundsExtent();
     float3 LocalBoundsMin = GetLocalBoundsOrigin() - LocalBoundsExtent;
     float3 LocalBoundsMax = GetLocalBoundsOrigin() + LocalBoundsExtent;

     float SVTVolumeExtentMax = max(max(VolumeResolution.x, VolumeResolution.y), VolumeResolution.z) / 2.0;
     float LocalBoundsExtentMin = min(min(LocalBoundsExtent.x, LocalBoundsExtent.y), LocalBoundsExtent.z);
     float3 SVTVolumeExtent = VolumeResolution / (int(SVTVolumeExtentMax / LocalBoundsExtentMin) + 1) / 2.0;

     float3 SVTBoundsMin = GetLocalBoundsOrigin() - SVTVolumeExtent;
     float3 SVTBoundsMax = GetLocalBoundsOrigin() + SVTVolumeExtent;

     if (P.x < (SVTBoundsMin.x) || P.y < (SVTBoundsMin.y) || P.z < (SVTBoundsMin.z) || P.x > (SVTBoundsMax.x) || P.y > (SVTBoundsMax.y) || P.z > (SVTBoundsMax.z))
         return Density;

     float3 UVW = (P - SVTBoundsMin) / (SVTBoundsMax - SVTBoundsMin);

     const FSparseVolumeTextureUniforms SVTUniforms = SparseVolumeTextureUnpackUniforms(SingleVolumeParams.PackedSVTUniforms0, SingleVolumeParams.PackedSVTUniforms1, SingleVolumeParams.StreamingInfoBuffer);
     const float3 VoxelUVW = SparseVolumeTextureSamplePageTable(SingleVolumeParams.SparseVolumeTexturePageTable, SVTUniforms, UVW, SVTADDRESSMODE_CLAMP, SVTADDRESSMODE_CLAMP, SVTADDRESSMODE_CLAMP, SingleVolumeParams.MipLevel);
     const float3 UVWTileSize = float(SPARSE_VOLUME_TILE_RES_PADDED) * SVTUniforms.TileDataTexelSize;

     if (any(VoxelUVW >= UVWTileSize)) // skip on null tile
      {
         const int PhysicalTileDataIndex = SingleVolumeParams.SVTChannel < 4 ? 0 : 1; // 0-8 A.rgba B.rgba
         const float4 VoxelData = SparseVolumeTextureSamplePhysicalTileData(SingleVolumeParams.SparseVolumeTextureA, SingleVolumeParams.SparseVolumeTextureB, SingleVolumeParams.TileDataTextureSampler, VoxelUVW, PhysicalTileDataIndex);

         Density = VoxelData[SingleVolumeParams.SVTChannel % 4];
     }

     return Density;
 }




参考 VisualizeSparseVolumeTexture.usf
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
 float SampleSparseVolumeTextureDensity(float3 P)
    float3 VolumeResolution = SingleVolumeParams.SparseVolumeTextureResolution;
    float3 RcpVolumeResolution = 1.0f / VolumeResolution;
    float Density = 0.0f;
      // 根据现在的包围盒最小边长进行映射变换
     float3 LocalBoundsExtent = GetLocalBoundsExtent();
     float3 LocalBoundsMin = GetLocalBoundsOrigin() - LocalBoundsExtent;
     float3 LocalBoundsMax = GetLocalBoundsOrigin() + LocalBoundsExtent;
     float SVTVolumeExtentMax = max(max(VolumeResolution.x, VolumeResolution.y), VolumeResolution.z) / 2.0;
     float LocalBoundsExtentMin = min(min(LocalBoundsExtent.x, LocalBoundsExtent.y), LocalBoundsExtent.z);
     float3 SVTVolumeExtent = VolumeResolution / (int(SVTVolumeExtentMax / LocalBoundsExtentMin) + 1) / 2.0;
     float3 SVTBoundsMin = GetLocalBoundsOrigin() - SVTVolumeExtent;
     float3 SVTBoundsMax = GetLocalBoundsOrigin() + SVTVolumeExtent;
     if (P.x < (SVTBoundsMin.x) || P.y < (SVTBoundsMin.y) || P.z < (SVTBoundsMin.z) || P.x > (SVTBoundsMax.x) || P.y > (SVTBoundsMax.y) || P.z > (SVTBoundsMax.z))
         return Density;
     float3 UVW = (P - SVTBoundsMin) / (SVTBoundsMax - SVTBoundsMin);
     const FSparseVolumeTextureUniforms SVTUniforms = SparseVolumeTextureUnpackUniforms(SingleVolumeParams.PackedSVTUniforms0, SingleVolumeParams.PackedSVTUniforms1, SingleVolumeParams.StreamingInfoBuffer);
     const float3 VoxelUVW = SparseVolumeTextureSamplePageTable(SingleVolumeParams.SparseVolumeTexturePageTable, SVTUniforms, UVW, SVTADDRESSMODE_CLAMP, SVTADDRESSMODE_CLAMP, SVTADDRESSMODE_CLAMP, SingleVolumeParams.MipLevel);
     const float3 UVWTileSize = float(SPARSE_VOLUME_TILE_RES_PADDED) * SVTUniforms.TileDataTexelSize;
     if (any(VoxelUVW >= UVWTileSize)) // skip on null tile
         const int PhysicalTileDataIndex = SingleVolumeParams.SVTChannel < 4 ? 0 : 1; // 0-8 A.rgba B.rgba
         const float4 VoxelData = SparseVolumeTextureSamplePhysicalTileData(SingleVolumeParams.SparseVolumeTextureA, SingleVolumeParams.SparseVolumeTextureB, SingleVolumeParams.TileDataTextureSampler, VoxelUVW, PhysicalTileDataIndex);
         Density = VoxelData[SingleVolumeParams.SVTChannel % 4];
     return Density;
```

## 第 61 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 61 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
一跟射线的旅行
                                                                       // 紧凑的光线行进上下文结构
                                                                       struct FRayMarchingCompactContext
                                                                       {

               采样结构
                                                                         // Local-space Ray
                                                                         float3 LocalRayOrigin;
                                                                         float3 LocalRayDirection;
                                                                         float LocalRayTMin;
                /**
                                                                         float LocalRayTMax;
                 * 定义每一次步进的采样结构
                 */
                                                                            // World-Space Ray
                struct FSingleVolumeSampleContext
                                                                            float3 WorldRayOrigin;
                {
                                                                            float3 WorldRayDirection;
                   float3 UVW;
                                                                            float LocalToWorldScale;
                   uint MipLevel;
                };
                                                                            // Ray-Step Info
                                                                            float Jitter;
                                                                            float StepSize;

                                                                            // Sample Info
                                                                            uint MipLevel;

                                                                            // Lighting Flags
                                                                            int bApplyEmissionAndTransmittance;
                                                                            int bApplyDirectLighting;
                                                                            int bApplyShadowTransmittance;
                                                                       };



 RayBox下的通用射线结构
                                                                       VRE积分过程所需要的最少中间量，即每一次
                                                                       步进至少需要管理哪些数据
                                                    /**
  /**                                                * 射线两个端点
   * 定义射线结构                                          */
   * 通过调整数据类型，可以直接支持VDB数据                           struct Segments
   */                                               {
  struct VolumeRay                                     float3 Start;
  {                                                    float3 End;
     float3 Origin;                                 };
     float3 Direction;
     float TMin;

                                                    定义一个射线段
     float TMax;
  };
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                                                                       // 紧凑的光线行进上下文结构
                                                                       struct FRayMarchingCompactContext
                                                                         // Local-space Ray
                                                                         float3 LocalRayOrigin;
                                                                         float3 LocalRayDirection;
                                                                         float LocalRayTMin;
                                                                         float LocalRayTMax;
                                                                            // World-Space Ray
                struct FSingleVolumeSampleContext
                                                                            float3 WorldRayOrigin;
                                                                            float3 WorldRayDirection;
                   float3 UVW;
                                                                            float LocalToWorldScale;
                   uint MipLevel;
                                                                            // Ray-Step Info
                                                                            float Jitter;
                                                                            float StepSize;
                                                                            // Sample Info
                                                                            uint MipLevel;
                                                                            // Lighting Flags
                                                                            int bApplyEmissionAndTransmittance;
                                                                            int bApplyDirectLighting;
                                                                            int bApplyShadowTransmittance;
   * 通过调整数据类型，可以直接支持VDB数据                           struct Segments
   */                                               {
  struct VolumeRay                                     float3 Start;
  {                                                    float3 End;
     float3 Origin;                                 };
     float3 Direction;
     float TMin;
     float TMax;
```

## 第 62 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 62 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
从深度重建世界坐标与本地坐标, 并填充光线行进上下文结构

                                                                                                                                   FRayMarchingCompactContext CreateSingleVolumeRayMarchingContext(
                                                                                                                                     // Local-Space Ray
float DeviceZ = SingleVolumeParams.SceneDepthTexture.Load(int3(SvPosition.xy, 0)).x; // backbuffer depth                             float3 LocalRayOrigin,
                                                                                                                                     float3 LocalRayDirection,
                                                                                                                                     float LocalRayTMin,
#if HAS_INVERTED_Z_BUFFER                                                                                                            float LocalRayTMax,
                                                                                                                                     // World-Space Ray
  DeviceZ = max(0.000000000001, DeviceZ);                                                                                            float3 WorldRayOrigin,
                                                                                                                                     float3 WorldRayDirection,
#endif // HAS_INVERTED_Z_BUFFER                                                                                                      // Ray-Step Info
                                                                                                                                     float Jitter,
                                                                                                                                     float StepSize,
float4x4 WorldToLocal = LWCHackToFloat(GetPrimitiveData(MaterialParameters.PrimitiveId).WorldToLocal);                               uint MipLevel,
                                                                                                                                     // Lighting Flags
float4x4 LocalToWorld = LWCHackToFloat(GetPrimitiveData(MaterialParameters.PrimitiveId).LocalToWorld);                               int bApplyEmissionAndTransmittance,
                                                                                                                                     int bApplyDirectLighting,
                                                                                                                                     int bApplyShadowTransmittance,
float3 LocalBoundsMin = GetLocalBoundsOrigin() - GetLocalBoundsExtent();                                                             // Matrix
                                                                                                                                     float4x4 LocalToWorld,
float3 LocalBoundsMax = GetLocalBoundsOrigin() + GetLocalBoundsExtent();                                                           )
                                                                                                                                     float4x4 WorldToLocal

                                                                                                                                   {
                                                                                                                                     FRayMarchingCompactContext Context = (FRayMarchingCompactContext)0;
float3 WorldRayOrigin = LWCHackToFloat(LWCSubtract(View.TranslatedWorldCameraOrigin, PrimaryView.PreViewTranslation));
float3 WorldRayEnd = LWCHackToFloat(SvPositionToWorld(float4(SvPosition.xy + View.TemporalAAJitter.xy, DeviceZ, 1)));                  // Local-space Ray
                                                                                                                                       Context.LocalRayOrigin = LocalRayOrigin;
float3 WordRayDirection = WorldRayEnd - WorldRayOrigin;                                                                                Context.LocalRayDirection = LocalRayDirection;
                                                                                                                                       Context.LocalRayTMin = LocalRayTMin;
float WorldRayLength = length(WordRayDirection);                                                                                       Context.LocalRayTMax = LocalRayTMax;

WordRayDirection /= WorldRayLength;                                                                                                    // World-Space Ray
                                                                                                                                       Context.WorldRayOrigin = WorldRayOrigin;
                                                                                                                                       Context.WorldRayDirection = WorldRayDirection;
float3 LocalRayOrigin = mul(float4(WorldRayOrigin, 1.0), WorldToLocal).xyz;                                                            // Ray-Step Info
float3 LocalRayEnd = mul(float4(WorldRayEnd, 1.0),WorldToLocal).xyz;                                                                   Context.Jitter = Jitter;
                                                                                                                                       Context.StepSize = StepSize;
float3 LocalRayDirection = LocalRayEnd - LocalRayOrigin;                                                                               Context.MipLevel = MipLevel;
float LocalRayLength = length(LocalRayDirection);                                                                                      // Lighting Flags
LocalRayDirection /= LocalRayLength;                                                                                                   Context.bApplyEmissionAndTransmittance = bApplyEmissionAndTransmittance;
                                                                                                                                       Context.bApplyDirectLighting = bApplyDirectLighting;
                                                                                                                                       Context.bApplyShadowTransmittance = bApplyShadowTransmittance;

float2 RayBoxInfo = IntersectAABB_Proto(LocalRayOrigin, LocalRayDirection, LocalBoundsMin, LocalBoundsMax, 0.0, LocalRayLength);       // Transform Local-Scale Properties into World-Scale
                                                                                                                                       float3 LocalRayHit = LocalRayOrigin + LocalRayDirection * LocalRayTMax;
                                                                                                                                       float3 WorldRayHit = mul(float4(LocalRayHit, 1.0), LocalToWorld).xyz;
float DistToBox = RayBoxInfo.x;                                                                                                        float WorldRayTMax = length(WorldRayHit - WorldRayOrigin);
                                                                                                                                       Context.LocalToWorldScale = WorldRayTMax / LocalRayTMax;
float DistInBox = RayBoxInfo.y;
                                                                                                                                       return Context;
float StepSize = DistInBox / VIEW_STEPS;                                                                                           }
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
                                                                                                                                     // Local-Space Ray
float DeviceZ = SingleVolumeParams.SceneDepthTexture.Load(int3(SvPosition.xy, 0)).x; // backbuffer depth                             float3 LocalRayOrigin,
                                                                                                                                     float3 LocalRayDirection,
                                                                                                                                     float LocalRayTMin,
#if HAS_INVERTED_Z_BUFFER                                                                                                            float LocalRayTMax,
                                                                                                                                     // World-Space Ray
  DeviceZ = max(0.000000000001, DeviceZ);                                                                                            float3 WorldRayOrigin,
                                                                                                                                     float3 WorldRayDirection,
#endif // HAS_INVERTED_Z_BUFFER                                                                                                      // Ray-Step Info
                                                                                                                                     float Jitter,
                                                                                                                                     float StepSize,
float4x4 WorldToLocal = LWCHackToFloat(GetPrimitiveData(MaterialParameters.PrimitiveId).WorldToLocal);                               uint MipLevel,
                                                                                                                                     // Lighting Flags
float4x4 LocalToWorld = LWCHackToFloat(GetPrimitiveData(MaterialParameters.PrimitiveId).LocalToWorld);                               int bApplyEmissionAndTransmittance,
                                                                                                                                     int bApplyDirectLighting,
                                                                                                                                     int bApplyShadowTransmittance,
float3 LocalBoundsMin = GetLocalBoundsOrigin() - GetLocalBoundsExtent();                                                             // Matrix
float3 LocalBoundsMax = GetLocalBoundsOrigin() + GetLocalBoundsExtent();                                                           )
                                                                                                                                     FRayMarchingCompactContext Context = (FRayMarchingCompactContext)0;
float3 WorldRayOrigin = LWCHackToFloat(LWCSubtract(View.TranslatedWorldCameraOrigin, PrimaryView.PreViewTranslation));
float3 WorldRayEnd = LWCHackToFloat(SvPositionToWorld(float4(SvPosition.xy + View.TemporalAAJitter.xy, DeviceZ, 1)));                  // Local-space Ray
                                                                                                                                       Context.LocalRayOrigin = LocalRayOrigin;
float3 WordRayDirection = WorldRayEnd - WorldRayOrigin;                                                                                Context.LocalRayDirection = LocalRayDirection;
                                                                                                                                       Context.LocalRayTMin = LocalRayTMin;
float WorldRayLength = length(WordRayDirection);                                                                                       Context.LocalRayTMax = LocalRayTMax;
WordRayDirection /= WorldRayLength;                                                                                                    // World-Space Ray
                                                                                                                                       Context.WorldRayOrigin = WorldRayOrigin;
                                                                                                                                       Context.WorldRayDirection = WorldRayDirection;
float3 LocalRayOrigin = mul(float4(WorldRayOrigin, 1.0), WorldToLocal).xyz;                                                            // Ray-Step Info
float3 LocalRayEnd = mul(float4(WorldRayEnd, 1.0),WorldToLocal).xyz;                                                                   Context.Jitter = Jitter;
                                                                                                                                       Context.StepSize = StepSize;
float3 LocalRayDirection = LocalRayEnd - LocalRayOrigin;                                                                               Context.MipLevel = MipLevel;
float LocalRayLength = length(LocalRayDirection);                                                                                      // Lighting Flags
LocalRayDirection /= LocalRayLength;                                                                                                   Context.bApplyEmissionAndTransmittance = bApplyEmissionAndTransmittance;
                                                                                                                                       Context.bApplyDirectLighting = bApplyDirectLighting;
                                                                                                                                       Context.bApplyShadowTransmittance = bApplyShadowTransmittance;
float2 RayBoxInfo = IntersectAABB_Proto(LocalRayOrigin, LocalRayDirection, LocalBoundsMin, LocalBoundsMax, 0.0, LocalRayLength);       // Transform Local-Scale Properties into World-Scale
                                                                                                                                       float3 LocalRayHit = LocalRayOrigin + LocalRayDirection * LocalRayTMax;
                                                                                                                                       float3 WorldRayHit = mul(float4(LocalRayHit, 1.0), LocalToWorld).xyz;
float DistToBox = RayBoxInfo.x;                                                                                                        float WorldRayTMax = length(WorldRayHit - WorldRayOrigin);
                                                                                                                                       Context.LocalToWorldScale = WorldRayTMax / LocalRayTMax;
float DistInBox = RayBoxInfo.y;
                                                                                                                                       return Context;
float StepSize = DistInBox / VIEW_STEPS;                                                                                           }
```

## 第 63 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 63 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
Volume Rendering Equation

                        遮挡物反射光                     当前采样点透光率* 散射光                    散射系数                     自发光*               黑体辐射*



                                                      𝑆𝑆
      𝐿𝐿 𝑥𝑥, 𝜔𝜔 = 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑠𝑠 𝐿𝐿 𝑥𝑥𝑠𝑠 , 𝜔𝜔   + � 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑡𝑡 𝐿𝐿𝑠𝑠 𝑥𝑥𝑡𝑡 , 𝜔𝜔 𝜎𝜎𝑠𝑠 𝑥𝑥𝑡𝑡 + 𝐿𝐿𝑒𝑒 𝑥𝑥𝑡𝑡 , 𝜔𝜔 ∗ 𝜎𝜎𝑎𝑎 𝑥𝑥𝑡𝑡 + 𝐿𝐿𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵 𝑑𝑑𝑡𝑡
                                                     0




                     𝐿𝐿𝐿𝐿𝐿𝐿𝐿𝐿𝐿𝐿𝐿                                                                                        𝑥𝑥𝑡𝑡
     𝐿𝐿𝑠𝑠 𝑥𝑥𝑡𝑡 , 𝜔𝜔 = � 𝑃𝑃 𝜔𝜔, 𝐿𝐿 𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 𝐿𝐿𝑖𝑖 𝑥𝑥, 𝐿𝐿                              𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑡𝑡 = 𝑒𝑒𝑒𝑒𝑒𝑒 − � 𝜎𝜎𝑡𝑡 𝑥𝑥 𝑑𝑑𝑡𝑡
                                                                                                                      𝑥𝑥
                       𝐿𝐿=0




     𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 = 𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆 𝑥𝑥, 𝐿𝐿 ∗ 𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿            𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 = 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝐿𝐿
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
      𝐿𝐿 𝑥𝑥, 𝜔𝜔 = 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑠𝑠 𝐿𝐿 𝑥𝑥𝑠𝑠 , 𝜔𝜔   + � 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑡𝑡 𝐿𝐿𝑠𝑠 𝑥𝑥𝑡𝑡 , 𝜔𝜔 𝜎𝜎𝑠𝑠 𝑥𝑥𝑡𝑡 + 𝐿𝐿𝑒𝑒 𝑥𝑥𝑡𝑡 , 𝜔𝜔 ∗ 𝜎𝜎𝑎𝑎 𝑥𝑥𝑡𝑡 + 𝐿𝐿𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵 𝑑𝑑𝑡𝑡
     𝐿𝐿𝑠𝑠 𝑥𝑥𝑡𝑡 , 𝜔𝜔 = � 𝑃𝑃 𝜔𝜔, 𝐿𝐿 𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 𝐿𝐿𝑖𝑖 𝑥𝑥, 𝐿𝐿                              𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑡𝑡 = 𝑒𝑒𝑒𝑒𝑒𝑒 − � 𝜎𝜎𝑡𝑡 𝑥𝑥 𝑑𝑑𝑡𝑡
                       𝐿𝐿=0
     𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 = 𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆 𝑥𝑥, 𝐿𝐿 ∗ 𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿            𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 = 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝐿𝐿
```

## 第 64 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 64 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
针对体积渲染方程的不同变种实现




Cloud Volume                     Fog Volume                             Standard Volume
积分过程参考UE内置体积云                    积分过程采用“本地空间步进”的方式，单散射                  积分算法实现最接近VRE原生数学模式

坐标系由基于双层球壳替换为基于RayBox            Raymarching过程尽可能清爽，不考虑黑体辐射             隐藏了不同体积介质的差异性

数据结构保留PMPC与PMC, 其余替换为“采样结        数据源兼容Shader Graph与“Shape Profile + 密   数据源使用Sparse Volume Texture 管理的稀疏体
构”与“光线行进的紧凑上下文结构”                度调试器”双模式，优先使用Shader Graph模式            素

数据源使用“Shape Profile + 密度调试器”，支
持网格距离场与密度体积纹理两种数据的解码
```

## 第 65 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 65 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
着色器部署：/Shaders/Private



 SingleVolumeShared.ush : 共享数据和着色器函数，包括HLSL端着色器参数和对应函数，Master节点PIN的数据识别函数，各类密度调试器函数

 RayBoxSetupForRaymarchingUtils.ush: 几种AABB包围盒相交测试函数

 SingleVolumeLightingUtils.ush：体积照明函数，如HG方程，PMC, PMPC, 普朗克曲线等

 SingleVolumeRaymarchingUtils.ush: 采样坐标结构，光线行进紧凑上下文，透光率方程，内散射方程

 SingleVolumeVertexFactory.ush：轻量化顶点工厂着色器标头文件

 SingleVolumeVertexShader.usf：几种体积模式通用的Vertex Shader

 CloudVolumeShadingPipeline.usf：针对体积云的VRE多散射积分，参考UE内置体积云实现

 FogVolumeShadingPipleline.usf：针对体积雾的VRE单散射积分，参考UE内置体积雾，但采用直接步进而非体素化的方式去做

 StandardVolumeShadingPipeline.usf： 针对VDB的标准体积，算法实现上最贴近VRE的原生数学模型

 SingleVolumeComposite.usf：Global Pass使用的着色器文件，用于混合颜色与深度
```

## 第 66 页：VRE 的高效GPU渲染

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 66 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VRE 的高效GPU渲染
渲染结果对比




渲染器       单体积渲染器                               单体积渲染器                               非均质渲染器

Tracker   Cloud Volume                         Fog Volume                           Standard Volume

数据源       VolumeTexture / Density Level Sets   VolumeTexture / Density Level Sets   OpenVDB / SparseVolumeTexture

压缩状态      Normal                               Normal                               SVT / 8bit unorm
```

## 第 67 页：Single Volume 插件

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 67 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Single Volume 插件
Runtime + Editor 两个模块


                        渲染器

                              功能性组件：程序化网格组件，流体组件


                              渲染类对象：云对象，流体雾对象，统一的单体积容器
                              类，参与介质材料组件，云模板等




                        编辑器


                              功能性组件：JFA
51个CPP文件，11个Shader文件


                              工具类：MeshToSDF, VDBToVT, NoiseGenerator,

                                    VolumeTextureViwer
```

## 第 68 页：Single Volume 插件

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 68 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Single Volume 插件
云模板预设


UCLASS(Blueprintable, BlueprintType)
class SINGLEVOLUMERUNTIME_API USingleVolumeCloudPreset : public UDataAsset
{
  GENERATED_BODY()

public:
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData, meta = (multiline = "true"))
 FText Description;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData, meta=(ClampMin = "0", UIMin   Extras : Cloud Library
= "0", ClampMax = "2", UIMax = "2", ToolTip = "0:Norm, 1:Adaptive, 2:Non"))
  int32 DetailPattern = 1;

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
 UTexture* ShapeProfile = nullptr;

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
 int32 ViewSteps = 64;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
  int32 ShadowSteps = 8;

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
 float DensityMultiplier = 1.0;

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
 float ShadowMultiplier = 1.0;

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
 FVector Resolution = FVector::ZeroVector;

#if WITH_EDITOR
   virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```cpp
UCLASS(Blueprintable, BlueprintType)
class SINGLEVOLUMERUNTIME_API USingleVolumeCloudPreset : public UDataAsset
  GENERATED_BODY()
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData, meta = (multiline = "true"))
 FText Description;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData, meta=(ClampMin = "0", UIMin   Extras : Cloud Library
= "0", ClampMax = "2", UIMax = "2", ToolTip = "0:Norm, 1:Adaptive, 2:Non"))
  int32 DetailPattern = 1;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
 UTexture* ShapeProfile = nullptr;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
 int32 ViewSteps = 64;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
  int32 ShadowSteps = 8;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
 float DensityMultiplier = 1.0;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
 float ShadowMultiplier = 1.0;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudData)
 FVector Resolution = FVector::ZeroVector;
#if WITH_EDITOR
   virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
```

## 第 69 页：实时气态流体解算器模块

> 来源：`在UE5外部模块中扩展复杂渲染管线实践【UnrealCircle2024-演讲版】.pdf` 第 69 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
实时气态流体解算器模块
    敬请期待！
```
