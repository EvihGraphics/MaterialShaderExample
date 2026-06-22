# 在 UE5 中打造多层体积天空 - Markdown 转译

## 资料身份

- 原始文件：`在UE5中打造多层体积天空-终稿【最终版】.pdf`
- 页数：80
- 用途：UE 体积、Shader、材质和插件化渲染背景
- 转译方式：按页提取文本，并对可识别的代码／接口片段生成代码块。
- 原始 PDF 未包含在本指导包中，以适配仓库忽略 PDF 的策略。

## 使用约束

- 本 Markdown 是 Agent 可检索版本，不替代原 PDF 的视觉排版。
- 架构图、流程图和多栏代码应结合页码理解。
- 代码转录中无法完全确认的行不得直接复制进生产代码，必须回到真实 UE5.7 源码或实际工程验证。

## 页码索引

- [第 1 页：在UE5中打造多层](#第-1-页在ue5中打造多层)
- [第 2 页：腾讯生态发展部](#第-2-页腾讯生态发展部)
- [第 3 页：今年的分享议题](#第-3-页今年的分享议题)
- [第 4 页：历史方案回顾与](#第-4-页历史方案回顾与)
- [第 5 页：云建模的经典历史方案回顾](#第-5-页云建模的经典历史方案回顾)
- [第 6 页：幻灯片中可能用到的排版](#第-6-页幻灯片中可能用到的排版)
- [第 7 页：多层体积云](#第-7-页多层体积云)
- [第 8 页：多层云海的实现](#第-8-页多层云海的实现)
- [第 9 页：云海的分层分布](#第-9-页云海的分层分布)
- [第 10 页：多层云海的实现](#第-10-页多层云海的实现)
- [第 11 页：多层云海的实现](#第-11-页多层云海的实现)
- [第 12 页：多层云海的实现](#第-12-页多层云海的实现)
- [第 13 页：Sub-Layer 内的归一化高度：](#第-13-页sublayer内的归一化高度)
- [第 14 页：多层云海的实现](#第-14-页多层云海的实现)
- [第 15 页：Photo](#第-15-页photo)
- [第 16 页：Shift : 0.3 Shift : 1.0](#第-16-页shift03shift10)
- [第 17 页：低频大型](#第-17-页低频大型)
- [第 18 页：中频侵蚀](#第-18-页中频侵蚀)
- [第 19 页：高频细节](#第-19-页高频细节)
- [第 20 页：第 20 页](#第-20-页第20页)
- [第 21 页：程序化造型的优缺点](#第-21-页程序化造型的优缺点)
- [第 22 页：准确控制体积云大型](#第-22-页准确控制体积云大型)
- [第 23 页：准确控制水平分布](#第-23-页准确控制水平分布)
- [第 24 页：准确控制云的垂直分布](#第-24-页准确控制云的垂直分布)
- [第 25 页：Cloud Layout 生成与编辑](#第-25-页cloudlayout生成与编辑)
- [第 26 页：SDF Profile 生成与编辑](#第-26-页sdfprofile生成与编辑)
- [第 27 页：SDF Profile 生成与编辑](#第-27-页sdfprofile生成与编辑)
- [第 28 页：融合两种模型](#第-28-页融合两种模型)
- [第 29 页：复合云海](#第-29-页复合云海)
- [第 30 页：风暴云模拟](#第-30-页风暴云模拟)
- [第 31 页：第 31 页](#第-31-页第31页)
- [第 32 页：内置渲染器原理与限制](#第-32-页内置渲染器原理与限制)
- [第 33 页：内置体积渲染器限制](#第-33-页内置体积渲染器限制)
- [第 34 页：内置体积渲染器原理](#第-34-页内置体积渲染器原理)
- [第 35 页：内置体积渲染器原理](#第-35-页内置体积渲染器原理)
- [第 36 页：内置体积渲染器原理](#第-36-页内置体积渲染器原理)
- [第 37 页：非均质材料渲染的](#第-37-页非均质材料渲染的)
- [第 38 页：Cumulus](#第-38-页cumulus)
- [第 39 页：一些值得思考的问题](#第-39-页一些值得思考的问题)
- [第 40 页：我们希望能够做到的](#第-40-页我们希望能够做到的)
- [第 41 页：如何自由摆放，旋转，缩放云对象](#第-41-页如何自由摆放旋转缩放云对象)
- [第 42 页：像影视流程一样自由](#第-42-页像影视流程一样自由)
- [第 43 页：Open VDB & CPU Analogy](#第-43-页openvdbcpuanalogy)
- [第 44 页：圆的窄带水平集示例 & Open VDB 的 GPU 线性快照](#第-44-页圆的窄带水平集示例openvdb的gpu线性快照)
- [第 45 页：VDB 数据的导入，识别，管理](#第-45-页vdb数据的导入识别管理)
- [第 46 页：一种保留数据稀疏性的可能](#第-46-页一种保留数据稀疏性的可能)
- [第 47 页：如何高效步进](#第-47-页如何高效步进)
- [第 48 页：如何控制射线行进](#第-48-页如何控制射线行进)
- [第 49 页：如何控制射线行进](#第-49-页如何控制射线行进)
- [第 50 页：如何控制射线行进](#第-50-页如何控制射线行进)
- [第 51 页：如何控制射线行进](#第-51-页如何控制射线行进)
- [第 52 页：如何控制射线行进](#第-52-页如何控制射线行进)
- [第 53 页：如何控制射线行进](#第-53-页如何控制射线行进)
- [第 54 页：支持各种形状的云](#第-54-页支持各种形状的云)
- [第 55 页：Shape Profile](#第-55-页shapeprofile)
- [第 56 页：Shape Profile Type](#第-56-页shapeprofiletype)
- [第 57 页：Erosion Profile & Modulate Final Density](#第-57-页erosionprofilemodulatefinaldensity)
- [第 58 页：Single Volume Renderer](#第-58-页singlevolumerenderer)
- [第 59 页：Single Volume Renderer](#第-59-页singlevolumerenderer)
- [第 60 页：如何渲染单体积模型](#第-60-页如何渲染单体积模型)
- [第 61 页：如何渲染单体积模型](#第-61-页如何渲染单体积模型)
- [第 62 页：如何渲染单体积模型](#第-62-页如何渲染单体积模型)
- [第 63 页：如何渲染单体积模型](#第-63-页如何渲染单体积模型)
- [第 64 页：如何渲染单体积模型](#第-64-页如何渲染单体积模型)
- [第 65 页：如何渲染单体积模型](#第-65-页如何渲染单体积模型)
- [第 66 页：如何渲染单体积模型 Ambient Light](#第-66-页如何渲染单体积模型ambientlight)
- [第 67 页：Unreal Engine](#第-67-页unrealengine)
- [第 68 页：如何渲染单体积模型](#第-68-页如何渲染单体积模型)
- [第 69 页：如何渲染单体积模型](#第-69-页如何渲染单体积模型)
- [第 70 页：Single Volume Renderer](#第-70-页singlevolumerenderer)
- [第 71 页：基于 Single Volume 的体积云渲染](#第-71-页基于singlevolume的体积云渲染)
- [第 72 页：基于 Single Volume 的体积云渲染](#第-72-页基于singlevolume的体积云渲染)
- [第 73 页：基于 Single Volume 的体积云渲染](#第-73-页基于singlevolume的体积云渲染)
- [第 74 页：基于 Single Volume 的体积云渲染](#第-74-页基于singlevolume的体积云渲染)
- [第 75 页：mesh volume](#第-75-页meshvolume)
- [第 76 页：基于 Single Volume](#第-76-页基于singlevolume)
- [第 77 页：天空音序器 编辑工具组](#第-77-页天空音序器编辑工具组)
- [第 78 页： 研发路线：以PC平台为基础，分策略将技术下放至移动平台，实现体积方案的多端覆盖](#第-78-页研发路线以pc平台为基础分策略将技术下放至移动平台实现体积方案的多端覆盖)
- [第 79 页：体积云方案与体积渲染方案参考](#第-79-页体积云方案与体积渲染方案参考)
- [第 80 页：THANKS!](#第-80-页thanks)

---

## 第 1 页：在UE5中打造多层

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 1 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
在UE5中打造多层
Unreal Fest 2023




体积天空
Angelou.lv
腾讯生态发展部-技术美术工程师
```

## 第 2 页：腾讯生态发展部

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 2 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
腾讯生态发展部
关注合作伙伴核心诉求，聚合内外部资源，提供产品的全方位解决方案



                                           技术
 与国内外知名图形引擎厂商，硬件研发团队，平台与内容创作公司紧密交流

 为合作项目提供全方位，立体化的游戏产品开发支持
                                      美术        IP
 包括产品标签挖掘，技术研发，美术设计，世界观构思，IP塑造与运营策略

 截至到目前，我们已经与十余家游戏厂商建立起了深度合作

 也向行业贡献了诸多的技术方案与研发经验                  发行        合作


                                           经营
```

## 第 3 页：今年的分享议题

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 3 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
今年的分享议题
1) 历史方案回顾与几个有趣的开发方向讨论
2) 多层体积云实现与风暴模拟
3) 遇到的技术瓶颈与内置渲染器原理分析
4) 单体积渲染器的设计思路
```

## 第 4 页：历史方案回顾与

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 4 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
历史方案回顾与
业内经典云建模与渲染思路总结




热点开发方向讨论
```

## 第 5 页：云建模的经典历史方案回顾

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 5 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
云建模的经典历史方案回顾

  SIGGRPAH 15,16 [1][2]

   ●   Weather Map + Height Gradient Interpolation
       ●   Perlin-Worley Noise + FBM, Remap Erosion, Powder Effect, Cone Sample
       ●   Unified Scattering Equation, Analyze Integration, Two Lobes HG Phase



  SIGGRPAH 17,19 [3][4]

   ●   Cloud Map + Vertical Profile
       ●   Vertical Profile, Absorption/Out-Scatter, In-Scatter Probability
       ●   Checkerboard framed Raymarching, Unified Volume Renderer



  SIGGRPAH 22,23 [5][6]

   ●   Envelope Model, Dimensional Profile Model
       ●   Sub-Layer, Cone Step Mapping, Envelope Model, Supercells Modeling
       ●   VDB Pipeline, Scene Voxelization, Global DF Raymarching, Uprezzed Erosion
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
  SIGGRPAH 15,16 [1][2]
  SIGGRPAH 17,19 [3][4]
  SIGGRPAH 22,23 [5][6]
```

## 第 6 页：幻灯片中可能用到的排版

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 6 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
幻灯片中可能用到的排版

一些热点开发方向讨论




          多层云            风暴系统                真实渲染
          多种云属     积雨云：塔式主体，巨大砧顶，附属云；        低成本多级散射评估
       分层分布，跨层分布    Supercells：台风，雷暴，组合系统   更小偏差的非均质模拟
```

## 第 7 页：多层体积云

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 7 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
多层体积云
基于内置渲染器所做的方案扩展
```

## 第 8 页：多层云海的实现

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 8 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
多层云海的实现
Schneider 22 的 Sub-Layer




                                       Cirrus Sub-Layer




                   Stratus Sub-Layer
```

## 第 9 页：云海的分层分布

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 9 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
云海的分层分布



虚拟层映射
```

## 第 10 页：多层云海的实现

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 10 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
多层云海的实现
UE 云层确定策略

// 球壳相交判断，先进行上球面相交测试，再进行下球面相交测试
float2 tTop2 = 0.0f;
if (RayIntersectSphereSolution(RayWorldOriginKm, Raydir, float4(RenderVolumetricCloudParameters.CloudLayerCenterKm,
RenderVolumetricCloudParameters.TopRadiusKm), tTop2))
{
   float2 tBottom2 = 0.0f;
   if (RayIntersectSphereSolution(RayWorldOriginKm, Raydir, float4(RenderVolumetricCloudParameters.CloudLayerCenterKm,
RenderVolumetricCloudParameters.BottomRadiusKm), tBottom2))
   {
      // 如果上球面与下球面均有2个交叉点，则保留最近的那个；如果上球面与下球面只有一个交叉点，分别保留最远的那个
      float TempTop = all(tTop2 > 0.0f) ? min(tTop2.x, tTop2.y) : max(tTop2.x, tTop2.y);
      float TempBottom = all(tBottom2 > 0.0f) ? min(tBottom2.x, tBottom2.y) : max(tBottom2.x, tBottom2.y);

    if (all(tBottom2 > 0.0f))
    {
        // 特别的，如果与下球面有两个交点，那么上球面不管有一个交点还是两个交点都应该选择最近的那个
        TempTop = max(0.0f, min(tTop2.x, tTop2.y));
    }

    // 最终Trace的Start Point和End Point
    TMin = min(TempBottom, TempTop);
    TMax = max(TempBottom, TempTop);
  }
  else
  {
    // 如果与上球面相交且与下球面不相交，则将第一次交点设定为TMin，第二个交点设定为TMax
    TMin = tTop2.x;
    TMax = tTop2.y;
  }
}
else
{
  // 如果没有与上球面相交，这种情况是最简单的，说明摄像机在太空且并没有任何射线穿过云层
  //OutColor0 = float4(1.0f, 0.0f, 0.0f, 1.0f);
  return;
}

TMin = max(0.0f, TMin) * KILOMETER_TO_CENTIMETER;
TMax = max(0.0f, TMax) * KILOMETER_TO_CENTIMETER;


                                                                                                                         1, 确定 CloudLayerCenterKm 0
                                                                                                                         2, 确定 BottomRadiusKm 3 = 1+2
                                                                                                                         3, 确定 TopRadiusKm 5 = 3 + 4
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
// 球壳相交判断，先进行上球面相交测试，再进行下球面相交测试
float2 tTop2 = 0.0f;
if (RayIntersectSphereSolution(RayWorldOriginKm, Raydir, float4(RenderVolumetricCloudParameters.CloudLayerCenterKm,
   float2 tBottom2 = 0.0f;
   if (RayIntersectSphereSolution(RayWorldOriginKm, Raydir, float4(RenderVolumetricCloudParameters.CloudLayerCenterKm,
      // 如果上球面与下球面均有2个交叉点，则保留最近的那个；如果上球面与下球面只有一个交叉点，分别保留最远的那个
      float TempTop = all(tTop2 > 0.0f) ? min(tTop2.x, tTop2.y) : max(tTop2.x, tTop2.y);
      float TempBottom = all(tBottom2 > 0.0f) ? min(tBottom2.x, tBottom2.y) : max(tBottom2.x, tBottom2.y);
    if (all(tBottom2 > 0.0f))
        // 特别的，如果与下球面有两个交点，那么上球面不管有一个交点还是两个交点都应该选择最近的那个
        TempTop = max(0.0f, min(tTop2.x, tTop2.y));
    // 最终Trace的Start Point和End Point
    TMin = min(TempBottom, TempTop);
    TMax = max(TempBottom, TempTop);
  else
    // 如果与上球面相交且与下球面不相交，则将第一次交点设定为TMin，第二个交点设定为TMax
    TMin = tTop2.x;
    TMax = tTop2.y;
else
  // 如果没有与上球面相交，这种情况是最简单的，说明摄像机在太空且并没有任何射线穿过云层
  //OutColor0 = float4(1.0f, 0.0f, 0.0f, 1.0f);
  return;
TMin = max(0.0f, TMin) * KILOMETER_TO_CENTIMETER;
TMax = max(0.0f, TMax) * KILOMETER_TO_CENTIMETER;
                                                                                                                         2, 确定 BottomRadiusKm 3 = 1+2
                                                                                                                         3, 确定 TopRadiusKm 5 = 3 + 4
```

## 第 11 页：多层云海的实现

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 11 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
多层云海的实现
UE 如何确定云的位置

struct FCloudLayerParameters
{
   float3 CloudLayerCenter;
   float PlanetRadius;
   float BottomRadius;
   float TopRadius;                                                                   CloudSampleAltitude = length(AbsoluteWorldPosition - CloudLayerCenter) –
   float ToNormAltitude;                                                              PlanetRadius
};
                                                                                      CloudSampleAltitudeInLayer = CloudSampleAltitude - BottomRadius;
FCloudLayerParameters GetCloudLayerParams(
  in float3 CloudLayerCenterKm,
  in float PlanetRadiusKm,                                                            CloudSampleNormAltitudeInLayer = saturate(CloudSampleAltitudeInLayer * ToNormAltitude)
  in float BottomRadiusKm,
  in float TopRadiusKm)
{                                                                                     ShadowSampleDistance
  FCloudLayerParameters CloudLayerParams;
  CloudLayerParams.CloudLayerCenter = CloudLayerCenterKm * KILOMETER_TO_CENTIMETER;
  CloudLayerParams.PlanetRadius = PlanetRadiusKm * KILOMETER_TO_CENTIMETER;
  CloudLayerParams.BottomRadius = BottomRadiusKm * KILOMETER_TO_CENTIMETER;
  CloudLayerParams.TopRadius = TopRadiusKm * KILOMETER_TO_CENTIMETER;
  CloudLayerParams.ToNormAltitude = 1.0 / (CloudLayerParams.TopRadius -
CloudLayerParams.BottomRadius);
  return CloudLayerParams;
}


              0，1，3，5是云层核心数据                                                          由云层数据转换单位后确定云位置
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
struct FCloudLayerParameters
   float3 CloudLayerCenter;
   float PlanetRadius;
   float BottomRadius;
   float TopRadius;                                                                   CloudSampleAltitude = length(AbsoluteWorldPosition - CloudLayerCenter) –
   float ToNormAltitude;                                                              PlanetRadius
                                                                                      CloudSampleAltitudeInLayer = CloudSampleAltitude - BottomRadius;
  in float3 CloudLayerCenterKm,
  in float PlanetRadiusKm,                                                            CloudSampleNormAltitudeInLayer = saturate(CloudSampleAltitudeInLayer * ToNormAltitude)
  in float BottomRadiusKm,
  in float TopRadiusKm)
{                                                                                     ShadowSampleDistance
  FCloudLayerParameters CloudLayerParams;
  CloudLayerParams.CloudLayerCenter = CloudLayerCenterKm * KILOMETER_TO_CENTIMETER;
  CloudLayerParams.PlanetRadius = PlanetRadiusKm * KILOMETER_TO_CENTIMETER;
  CloudLayerParams.BottomRadius = BottomRadiusKm * KILOMETER_TO_CENTIMETER;
  CloudLayerParams.TopRadius = TopRadiusKm * KILOMETER_TO_CENTIMETER;
  CloudLayerParams.ToNormAltitude = 1.0 / (CloudLayerParams.TopRadius -
CloudLayerParams.BottomRadius);
  return CloudLayerParams;
```

## 第 12 页：多层云海的实现

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 12 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
多层云海的实现
构建 Virtual Sub-Layer 的思路
                                                                  TopRadiusKm




                                                                                                 LayerHeightKm
                  Virtual Sub-Layer 2

                                        Distance Between Layers



                                              BaseCloudHeight                      AltitudeInLayer

                  Virtual Sub-Layer 1                             BottomRadiusKm



                                                                  PlanetRadiusKm                     LayerBottomAltitudeKm
```

## 第 13 页：Sub-Layer 内的归一化高度：

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 13 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Sub-Layer 内的归一化高度：
                                                            𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴𝐴 − 𝐵𝐵𝐵𝐵 + 𝐺𝐺𝐺𝐺𝐺𝐺 ∗ 𝑁𝑁 − 1
                     𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁 =
                                                                                            𝐵𝐵𝐵𝐵
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                     𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁𝑁 =
```

## 第 14 页：多层云海的实现

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 14 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
多层云海的实现
如何确定 Virtual Sub-Layer 中云的位置                                                                       云的分布控制
                                                                                                               （有限编辑）
                                                                                                   Cloud Pattern
                                                                                                   4 Channel 2D
                                                                                                   512 * 512



                   AltitudeInLayer


                        （0, BH）

                                                UV_1 = Wps * ScaleKm + Offset * Time * MoveSpeed
          (BH, BH + Gap)          Sub-Layer 1
                                                MipLevel_1 = CloudMapMip
                                                                                                      Perlin-Worley Noise               Perlin Noise                  Pattern Noise   Mask

(BH + Gap, 2BH + Gap)    Null

                                                UV_2 = UV1 * (ScaleX, ScaleY)                                           拟合程序化分布                                       提供特定模式          融合分布
    Sub-Layer 2
                                                MipLevel_2 = CloudMapMip + LayerMipOffset


                                                                                                   CloudCoverage = (Coverage - Remap(Coverage, 0.0, 0.2, 0.2, 0.0)) * Scale
                                                                                                   CloudCoverage = 1.0 - Clamp(CloudCoverage, -0.2, 3.0) / 6.5
                                                                                                   CloudCoverage = pow(CloudCoverage, 3.0) – 0.25


                                                                                                   ConservativeDensity = saturate(layout.r * layout.g – CloudCoverage)
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                UV_1 = Wps * ScaleKm + Offset * Time * MoveSpeed
          (BH, BH + Gap)          Sub-Layer 1
                                                MipLevel_1 = CloudMapMip
(BH + Gap, 2BH + Gap)    Null
                                                UV_2 = UV1 * (ScaleX, ScaleY)                                           拟合程序化分布                                       提供特定模式          融合分布
                                                MipLevel_2 = CloudMapMip + LayerMipOffset
                                                                                                   CloudCoverage = (Coverage - Remap(Coverage, 0.0, 0.2, 0.2, 0.0)) * Scale
                                                                                                   CloudCoverage = 1.0 - Clamp(CloudCoverage, -0.2, 3.0) / 6.5
                                                                                                   CloudCoverage = pow(CloudCoverage, 3.0) – 0.25
                                                                                                   ConservativeDensity = saturate(layout.r * layout.g – CloudCoverage)
```

## 第 15 页：Photo

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 15 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                                                                                         Photo

多层云海的实现
云的造型思路




                                    Sub Noise Amount              Taper                                                         Wispy

                    Sub Noise Scale

                                                                                 Top Erosion Amount
                                                                                                                          Billowy

Variation Along Altitude


                                                                                 Bottom Erosion Amount

                                                                                                         Render                         Billowy



HeightVariation = pow(normAlttitudeInVSL, shiftCurve) * (layout.g * 0.5 + 1.0)




                                                                                                                  Wispy
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
HeightVariation = pow(normAlttitudeInVSL, shiftCurve) * (layout.g * 0.5 + 1.0)
```

## 第 16 页：Shift : 0.3 Shift : 1.0

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 16 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Shift : 0.3   Shift : 1.0




Shift : 2.0   Shift : 3.0
```

## 第 17 页：低频大型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 17 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
低频大型
```

## 第 18 页：中频侵蚀

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 18 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
中频侵蚀
```

## 第 19 页：高频细节

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 19 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
高频细节
```

## 第 20 页：第 20 页

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 20 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

_该页未提取到可复制文字，可能主要由图片或图表构成。_

## 第 21 页：程序化造型的优缺点

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 21 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
程序化造型的优缺点
快速完成天空造景

比较好的中远景动态效果，云卷云舒

便于同TOD，天气系统关联，实现数据驱动



高度插值模型能够模拟的云类型有限

美术无法准确控制云出现的位置和状态
```

## 第 22 页：准确控制体积云大型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 22 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
准确控制体积云大型


距离场剖面模型
```

## 第 23 页：准确控制水平分布

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 23 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
准确控制水平分布
水平分布信息索引垂直分布信息

                                                        Cloud Layout（完全手动编辑）
                                                        4 Channel 2D
                                                        512 * 512
                             C




          N
      W       E
          S                                         H


          Coverage Profile       Vertical Profile             Coverage 1       Coverage 2   Coverage 3   Storm Coverage
```

## 第 24 页：准确控制云的垂直分布

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 24 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
准确控制云的垂直分布
垂直分布信息




                       Profile

         Coverage XY




                       NormAltitudeInLayer
```

## 第 25 页：Cloud Layout 生成与编辑

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 25 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Cloud Layout 生成与编辑
扩展体积插件中的云投影笔刷，合成器与绘制器



●   云笔刷对象
    ●   类似画布上画云的笔刷
    ●   每个蓝图实例代表一朵云
    ●   产生二维形状投影



●   云图生成器
    ●   作用类似于“画布”，提供绘制框
    ●   接受来自笔刷对象的投影
    ●   执行 RT Ping-Pong 操作依次绘制并混合笔刷的二
        维投影，进行模糊处理
    ●   将存储有分布信息的RT序列化到磁盘或直接泵送给
        多层云装置，实时反馈




                                        单一云朵的融合
```

## 第 26 页：SDF Profile 生成与编辑

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 26 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
SDF Profile 生成与编辑
距离场垂直剖面生成器算法流程
                                                                                                        原始到结果

                                                                                                                       Previewer
                                                                                Color Curve

Create RT_SeedCurve, RT_CurveLineEquation, RT_JumpFlood2DResult,
RT_AccumulatedDistanceFields;

for(i=0; i++; i<=3)
                                                                                                                预览渲染
{
  // Step1：清空三张工作RT
  do Clear RT_SeedCurve, RT_CurveLineEquation, RT_JumpFlood2DResult;                           曲线重编码
    // Step2: 曲线重编码
    do RT_SeedCurve = CreateSeedCurve(Curve)；                                   Seed Curve                             SDF Profile

    // Step3: 生成跳洪需要的2D种子纹理                                                                    生成种子纹理
    do RT_CurveLineEquation = ComputeSeedCurvePositions(RT_SeedCurve);


                                                                                                                计算有符号距离场
    // Step4: 执行2D跳洪算法，得到距离场信息和Mask
    do RT_JumpFlood2DResult = JumpFlood2D(RT_CurveLineEquation);

    // Step5: 分通道写入剖面
    do RT_AccumulatedDistanceFields = CompositeResults(RT_JumpFlood2DResult);
}                                                                                                        跳洪
                                                                                Seed Texture                           JFA Result
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
RT_AccumulatedDistanceFields;
for(i=0; i++; i<=3)
  // Step1：清空三张工作RT
  do Clear RT_SeedCurve, RT_CurveLineEquation, RT_JumpFlood2DResult;                           曲线重编码
    // Step2: 曲线重编码
    do RT_SeedCurve = CreateSeedCurve(Curve)；                                   Seed Curve                             SDF Profile
    // Step3: 生成跳洪需要的2D种子纹理                                                                    生成种子纹理
    do RT_CurveLineEquation = ComputeSeedCurvePositions(RT_SeedCurve);
    // Step4: 执行2D跳洪算法，得到距离场信息和Mask
    do RT_JumpFlood2DResult = JumpFlood2D(RT_CurveLineEquation);
    // Step5: 分通道写入剖面
    do RT_AccumulatedDistanceFields = CompositeResults(RT_JumpFlood2DResult);
}                                                                                                        跳洪
```

## 第 27 页：SDF Profile 生成与编辑

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 27 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
SDF Profile 生成与编辑
基于曲线控制的垂直分布




 使用曲线编辑器实时更新剖面

 Vertical Profile 控制云垂直造型

 Cloud Layout 驱动水平分布
```

## 第 28 页：融合两种模型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 28 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
融合两种模型
两套高度坐标能够共存

             TopRadiusKm




             BottomRadiusKm



             PlanetRadiusKm
```

## 第 29 页：复合云海

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 29 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
复合云海
```

## 第 30 页：风暴云模拟

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 30 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
风暴云模拟
```

## 第 31 页：第 31 页

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 31 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

_该页未提取到可复制文字，可能主要由图片或图表构成。_

## 第 32 页：内置渲染器原理与限制

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 32 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
内置渲染器原理与限制
我们在开发中遇到的挑战
```

## 第 33 页：内置体积渲染器限制

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 33 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
内置体积渲染器限制

最佳工作状态的空间尺度要求

1) 云的水平覆盖范围为 10km~500km，最佳尺度100~300km数量级
2) 垂直方向上云层厚度不能太小
3) 体积雾的最佳工作尺度是100m以内, 体积雾的最大覆盖范围
与体积云最小工作范围间仍有很大的空间裕量： 10^2




不能或很难贴地，超近距离下的噪点问题

1) 不能直接贴地，会影响渲染质量
通过“高度图过滤”能够做到贴地，但依旧会影响 Raymarching 的工作状态；只能同时运行一个场景代理
2) 超低空时与指数高度雾的融合（降低云的轮廓感和对比度，颜色融合）
3) 地面近距离，飞行视角近距离容易产生不可避免的噪点，需要通过额外手段处理




对物理照明的要求与特殊视角问题

1) 严格遵循物理照明，环境光屏蔽的做法只考虑了地面视角
2) 云层平视视角下天际线会有一条明显的“噪波亮带”
```

## 第 34 页：内置体积渲染器原理

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 34 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
内置体积渲染器原理
属性传递与核心类，方法调用



             UObject


                                                                                                   FRenderResource            FSceneView
        UActorComponent


                                                                                                  FMaterialRenderProxy         FViewInfo
       USceneComponent            CloudVolumeMaterial



   UVolumetricCloudComponent    FVolumetricCloudSceneProxy      FVolumetricCloudRenderSceneInfo         FCloudRenderContext           FSceneRenderer::RenderVolumetricCloud



                           FVolumetricCloudRenderViewMeshProcessor        FSceneRenderer::RenderVolumetricCloudsInteranl




                                              FRenderVolumetricCloudRenderViewParametersPS          FRenderVolumetricCloudRenderViewCS
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                                                                   FRenderResource            FSceneView
   UVolumetricCloudComponent    FVolumetricCloudSceneProxy      FVolumetricCloudRenderSceneInfo         FCloudRenderContext           FSceneRenderer::RenderVolumetricCloud
                           FVolumetricCloudRenderViewMeshProcessor        FSceneRenderer::RenderVolumetricCloudsInteranl
```

## 第 35 页：内置体积渲染器原理

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 35 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
内置体积渲染器原理
SIGGRAPH 2015 Frostbite volumetric rendering: single scattering




                                                                        𝒙𝒙
                                                                                                                                       𝒙𝒙𝒙𝒙




                                                                              𝒔𝒔
                                                                                                                                                           𝒔𝒔
                        𝑳𝑳𝑳𝑳 𝒙𝒙, 𝝎𝝎𝝎𝝎 = 𝑻𝑻𝑻𝑻 𝒙𝒙, 𝒙𝒙𝒙𝒙 𝑳𝑳𝑳𝑳 𝒙𝒙𝒙𝒙, 𝝎𝝎𝝎𝝎 + � 𝑻𝑻𝑻𝑻 𝒙𝒙, 𝒙𝒙𝒙𝒙 𝝈𝝈𝒕𝒕 𝒙𝒙 𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳 𝒙𝒙𝒙𝒙, 𝝎𝝎𝝎𝝎 𝒅𝒅𝒅𝒅   𝑻𝑻𝑻𝑻 𝒙𝒙, 𝒙𝒙𝒙𝒙 = 𝒆𝒆𝒆𝒆𝒆𝒆(− ∫𝟎𝟎 𝝈𝝈𝒕𝒕 𝒙𝒙 𝒅𝒅𝒅𝒅)
                                                                             𝟎𝟎

                                                   𝒍𝒍𝒍𝒍𝒍𝒍𝒍𝒍𝒍𝒍𝒍𝒍

                        𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳 𝒙𝒙𝒙𝒙, 𝝎𝝎𝒊𝒊 = 𝛒𝛒 � 𝒇𝒇 𝒗𝒗, 𝒍𝒍 𝑽𝑽𝑽𝑽𝑽𝑽 𝒙𝒙, 𝒍𝒍 𝑳𝑳𝑳𝑳(𝒙𝒙, 𝒍𝒍)                                    𝑽𝑽𝑽𝑽𝑽𝑽 𝒙𝒙, 𝒍𝒍 = 𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔 𝒙𝒙, 𝒍𝒍
                                                     𝒍𝒍=𝟎𝟎                                                                                 ∗ 𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗 𝒙𝒙, 𝒍𝒍
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                        𝑳𝑳𝑳𝑳 𝒙𝒙, 𝝎𝝎𝝎𝝎 = 𝑻𝑻𝑻𝑻 𝒙𝒙, 𝒙𝒙𝒙𝒙 𝑳𝑳𝑳𝑳 𝒙𝒙𝒙𝒙, 𝝎𝝎𝝎𝝎 + � 𝑻𝑻𝑻𝑻 𝒙𝒙, 𝒙𝒙𝒙𝒙 𝝈𝝈𝒕𝒕 𝒙𝒙 𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳 𝒙𝒙𝒙𝒙, 𝝎𝝎𝝎𝝎 𝒅𝒅𝒅𝒅   𝑻𝑻𝑻𝑻 𝒙𝒙, 𝒙𝒙𝒙𝒙 = 𝒆𝒆𝒆𝒆𝒆𝒆(− ∫𝟎𝟎 𝝈𝝈𝒕𝒕 𝒙𝒙 𝒅𝒅𝒅𝒅)
                        𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳𝑳 𝒙𝒙𝒙𝒙, 𝝎𝝎𝒊𝒊 = 𝛒𝛒 � 𝒇𝒇 𝒗𝒗, 𝒍𝒍 𝑽𝑽𝑽𝑽𝑽𝑽 𝒙𝒙, 𝒍𝒍 𝑳𝑳𝑳𝑳(𝒙𝒙, 𝒍𝒍)                                    𝑽𝑽𝑽𝑽𝑽𝑽 𝒙𝒙, 𝒍𝒍 = 𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔𝒔 𝒙𝒙, 𝒍𝒍
                                                     𝒍𝒍=𝟎𝟎                                                                                 ∗ 𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗𝒗 𝒙𝒙, 𝒍𝒍
```

## 第 36 页：内置体积渲染器原理

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 36 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
内置体积渲染器原理
Raymarching执行流程: VolumetricCloud.usf                    逐采样更新云层参数


                                                        逐采样更新云层PBS



                                                        逐采样评估PMPC
                                            获取大气透射光

                                                        逐采样评估PMC
                                            计算步进次数与步幅                      评估预曝光
                   CS                                    为天光评估AO
                           Ray-Sphere求交判断
                                            获取距离场天光                     评估Aerial Perspective

                            采样深度判断遮挡                     评估Ls_ground
                   PS
                                             初始化PMPC                        融合雾效

                                                        评估接收的Shadow
                           确定摄像机位置
                                            获取云层高度数据                        功能补充
                                                          评估Ls_dir1,2

                                            评估地表环境光散射
                                                          评估Ls_local


                                              准备数据
                                                          执行方程积分



                                                            主循环
```

## 第 37 页：非均质材料渲染的

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 37 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
非均质材料渲染的
独立的Single Volume Renderer




单体积法
```

## 第 38 页：Cumulus

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 38 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
     Cumulus
render with Single Volume
   318 x 324 x 480 (m)
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
   318 x 324 x 480 (m)
```

## 第 39 页：一些值得思考的问题

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 39 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
一些值得思考的问题

●   如何尽可能减少无效采样点，高效步进？

●   如何描述云对象的形状轮廓？

●   自然且灵活的场景编辑模式？
```

## 第 40 页：我们希望能够做到的

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 40 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
我们希望能够做到的

●   场景设计师可以自由摆放，选转，缩放云对象

●   渲染器能够知道云的摆放位置，仅在接触云后才开始步进

●   云的大型控制包含全部3D维度，支持各种形状的云属

●   能够兼容体积雾，体积烟以及自发辐射介质
```

## 第 41 页：如何自由摆放，旋转，缩放云对象

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 41 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何自由摆放，旋转，缩放云对象



解决第一个问题
```

## 第 42 页：像影视流程一样自由

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 42 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
像影视流程一样自由

设计师可以自由摆放，旋转云对象，具有很高的缩放
自 由度

可以在代理场景，渲染场景之间切换，新的天空造景
工作流


将场景中任意物件转换为体积的
```

## 第 43 页：Open VDB & CPU Analogy

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 43 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Open VDB & CPU Analogy


Root node (Sparse, unbounded)   VDB Hierarchy                                                                      Memory Hierarchy

                                                Tile values with                                        Capacity

                                                active/inactive states                                                  Memory


                                                                         Active Mask
                                                                                                                        L3 Cache
Internal Node 1                                                          Child Mask
                                                                         Tile values / Child pointers

                                                                                                                        L2 Cache
                                                                         Active Mask
                                                                         Child Mask
Internal Node 2
                                                                         Tile values / Child pointers                   L1 Cache


                                                                               Active Mask
                                                                                                                          Core
Leaf Node                                                                      Voxels Value
                                                                                                                                      Latency
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
Root node (Sparse, unbounded)   VDB Hierarchy                                                                      Memory Hierarchy
```

## 第 44 页：圆的窄带水平集示例 & Open VDB 的 GPU 线性快照

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 44 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
圆的窄带水平集示例 & Open VDB 的 GPU 线性快照


                                                       Outside
                                                       Φ(x, y)>0
                       Φ(x, y) =x^2 + y^2 – r^2




                                           inside
                                           Φ(x, y)<0




Dense representation
Root node
Internal Node 1                                                    00 00                 Active bit-mask
                                                                   10 11                 Child bit-mask
Internal Node 2                                                    00 00 00
                                                                   11 01 10
Leaf nodes                                                         0001 1000 0001 1000

                                                                                                           Nano VDB
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                       Φ(x, y)>0
                       Φ(x, y) =x^2 + y^2 – r^2
                                           Φ(x, y)<0
```

## 第 45 页：VDB 数据的导入，识别，管理

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 45 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
VDB 数据的导入，识别，管理
如何在引擎中使用稀疏体素数据

●   数据导入
    ●   转换 open vdb 至 nano vdb
    ●   直接载入 nano vdb



●   数据识别
    ●   定义 UObject, UFactory UActorFactory
    ●   VDB Container



●   数据管理
    ●   “窄带水平集” 或 “距离场”写入体积纹理
        （稠密转换）
    ●   体积流送与稀疏存储 (SVT?)
    ●   VDB 自身的数据管理机制



                                             Thibault Lambert’s “Sparse Volumetric” : https://github.com/thilamb/unreal-vdb/tree/5.2
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
    ●   体积流送与稀疏存储 (SVT?)
                                             Thibault Lambert’s “Sparse Volumetric” : https://github.com/thilamb/unreal-vdb/tree/5.2
```

## 第 46 页：一种保留数据稀疏性的可能

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 46 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
一种保留数据稀疏性的可能
《Ray Tracing Gems II》


●   Atlas Texture
    ●   存储块数据



●   Indirection Texture
    ●   存储数据块的链接索引，起到类似页表的作用



●   Range Texture
    ●   存储 [Minorant，Majorant]，即范围
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
    ●   存储 [Minorant，Majorant]，即范围
```

## 第 47 页：如何高效步进

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 47 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何高效步进



解决第二个问题
```

## 第 48 页：如何控制射线行进

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 48 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何控制射线行进
Legacy ray-march


                   Top Level




                                                         Final Density




                                  Conservative Density


                   Bottom Level
```

## 第 49 页：如何控制射线行进

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 49 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何控制射线行进
Legacy ray-march



                                                                      Sphere-Interaction test                     “零密度点”能够提前预知从而省略吗？
                                                                      For each step along view ray

                                                                          Sample Conservative Density
                                                                          Sample Detail Density & Modulate Density
                   Cloud Map + Vertical Profile + FBM Noise Erosion
                                                                          Sample Light Energy
                                       Cloud Ray-March Procedure
                                                                              For each step along light
                                                                                                                     尽可能减少子循环运算
                                                                                  Sample Conservative Density
                                                                                  Sample Detail Density & Modulate Density

                                                                          Integration
                                                                          Determine step size and take the step
```

## 第 50 页：如何控制射线行进

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 50 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何控制射线行进
Ray-march Box



                distance in ray box    distance to ray box




                           entry pos
```

## 第 51 页：如何控制射线行进

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 51 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何控制射线行进
Ray-march Box intersection test




       float3 BoundMin = LocalToWorld(LocalObjectBoundsMax);
       float3 BoundMax = LocalToWorld(LocalObjectBoundsMin);

       float3 RayDir = normalize(u_WorldPosition - u_CameraPosition);
       float3 InvRayDir = 1.0 / RayDir;
       float3 RayOrigin = u_CameraPosition;

       float3 t0 = (BoundMin - RayOrigin) * InvRayDir;
       float3 t1 = (BoundMax - RayOrigin) * InvRayDir;

       float3 tMin = min(t0, t1);
       float3 tMax = max(t0, t1);

       float tN = max(tMin.x, max(tMin.y, tMin.z));
       float tF = min(tMax.x, min(tMax.y, tMax.z));
       tF = min(tF, SceneDepth);
       tN = max(tN, 0.0);

       float dstInSideBox = max(0, tF - tN);
       float3 entryPos = RayOrigin + tN * RayDir;

       return float4(entryPos, dstInSideBox);
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
       float3 BoundMin = LocalToWorld(LocalObjectBoundsMax);
       float3 BoundMax = LocalToWorld(LocalObjectBoundsMin);
       float3 RayDir = normalize(u_WorldPosition - u_CameraPosition);
       float3 InvRayDir = 1.0 / RayDir;
       float3 RayOrigin = u_CameraPosition;
       float3 t0 = (BoundMin - RayOrigin) * InvRayDir;
       float3 t1 = (BoundMax - RayOrigin) * InvRayDir;
       float3 tMin = min(t0, t1);
       float3 tMax = max(t0, t1);
       float tN = max(tMin.x, max(tMin.y, tMin.z));
       float tF = min(tMax.x, min(tMax.y, tMax.z));
       tF = min(tF, SceneDepth);
       tN = max(tN, 0.0);
       float dstInSideBox = max(0, tF - tN);
       float3 entryPos = RayOrigin + tN * RayDir;
       return float4(entryPos, dstInSideBox);
```

## 第 52 页：如何控制射线行进

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 52 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何控制射线行进
Shadow sample distribution




                             Linear shadow distribution   Nonlinear shadow distribution   Cone shadow distribution
```

## 第 53 页：如何控制射线行进

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 53 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何控制射线行进
Raymarching acceleration & break methods




                                                    步进优化策略




         保守密度估计                            提前中断机制            非线性阴影样本估计   随距离增大 miplevel
```

## 第 54 页：支持各种形状的云

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 54 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
支持各种形状的云



解决第三个问题
```

## 第 55 页：Shape Profile

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 55 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Shape Profile
3-Dimensional Profile




                                   VDB                                      Mesh                                     Mesh




    Level Sets                                 Mesh SDF                                Mesh Slice




                               Volume Render                           Volume Render                               Volume Render




                 VDB Toolkit                          Mesh To SDF Toolkit                           Mesh To VT Toolkit
```

## 第 56 页：Shape Profile Type

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 56 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Shape Profile Type




         Density Level Sets    Mesh SDF   Mesh Slice




不同的形状剖面表征方法需要不同的 Density Tracker !
```

## 第 57 页：Erosion Profile & Modulate Final Density

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 57 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Erosion Profile & Modulate Final Density

     Noise Mod 0 : wispy & billowy


                                                                                                    Curly Alligator
                                                                                                                        float wispy_noise = lerp(DN.r, DN.g, ShapeProfile);
                                                                                                    4 Channel           float billowy_type_gradient = pow(ShapeProfile, 0.25);
                                                                                                                        float billowy_noise = lerp(DN.b * 0.3, DN.a * 0.3, billowy_type_gradient);
                                                                                                    128 x 128 x 128     float noise_composite = lerp(wispy_noise, billowy_noise, CloudType);
                                                                                                                        float uprezzed_density = ValueErosion(ShapeProfile, noise_composite);
                                                                                                                        return uprezzed_density;



Low Freq “Curly-Alligator” High Freq “Curly-Alligator”   Low Freq Alligator   High Freq Alligator




     Noise Mod 1 : balanced fbm

                                                                                                    Balanced
                                                                                                                      float DNfbm = remap(DN.r, -(1- (DN.g * 0.625 + DN.b * 0.25 + DN.a * 0.125)), 1.0, 0.0, 1.0);
                                                                                                    4 Channel         float DNmod = DNfbm;
                                                                                                    128 x 128 x 128   return saturate(DNmod - (1.0 - ShapeProfile));




Low Freq “Perlin-Worley”       Low Freq Worley           Med Freq Worley      High Freq Worley

                                                                                                                      return saturate(ShapeProfile);

     Noise Mod 2 : level sets
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                                                                                        float wispy_noise = lerp(DN.r, DN.g, ShapeProfile);
                                                                                                    4 Channel           float billowy_type_gradient = pow(ShapeProfile, 0.25);
                                                                                                                        float billowy_noise = lerp(DN.b * 0.3, DN.a * 0.3, billowy_type_gradient);
                                                                                                    128 x 128 x 128     float noise_composite = lerp(wispy_noise, billowy_noise, CloudType);
                                                                                                                        float uprezzed_density = ValueErosion(ShapeProfile, noise_composite);
                                                                                                                        return uprezzed_density;
                                                                                                                      float DNfbm = remap(DN.r, -(1- (DN.g * 0.625 + DN.b * 0.25 + DN.a * 0.125)), 1.0, 0.0, 1.0);
                                                                                                    4 Channel         float DNmod = DNfbm;
                                                                                                    128 x 128 x 128   return saturate(DNmod - (1.0 - ShapeProfile));
                                                                                                                      return saturate(ShapeProfile);
```

## 第 58 页：Single Volume Renderer

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 58 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Single Volume Renderer


Wdas Cloud




         498 x 338 x 613 (m)
Cloud Volume ：Triple order Scattering
            Noise Mod:2
        Density Multiple : 110
     Shadow Multiple : 0.498352
             4k, 120fps
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
         498 x 338 x 613 (m)
```

## 第 59 页：Single Volume Renderer

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 59 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Single Volume Renderer



如何渲染单体积模型
```

## 第 60 页：如何渲染单体积模型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 60 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何渲染单体积模型
Light transmission in Participating Media



              外散射                           吸收                                    自发光                内散射




                                   削弱辐射                                                       增强辐射
                                            属性        描述       单位

                                             𝜎𝜎𝑎𝑎    吸收系数      𝑚𝑚−1

                                             𝜎𝜎𝑠𝑠    散射系数      m−1       𝜎𝜎𝑡𝑡 = 𝜎𝜎𝑠𝑠 + 𝜎𝜎𝑎𝑎
                                             𝜎𝜎𝑡𝑡    消光系数      m−1       𝜎𝜎𝑠𝑠 = 𝜌𝜌 ∗ 𝜎𝜎𝑡𝑡
                                             𝜎𝜎𝑒𝑒   自 发辐射系数    m−1

                                             𝜌𝜌      反照率      unitless

                                              P      相位函数      𝑠𝑠𝑠𝑠 −1
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                             𝜎𝜎𝑠𝑠    散射系数      m−1       𝜎𝜎𝑡𝑡 = 𝜎𝜎𝑠𝑠 + 𝜎𝜎𝑎𝑎
                                             𝜎𝜎𝑡𝑡    消光系数      m−1       𝜎𝜎𝑠𝑠 = 𝜌𝜌 ∗ 𝜎𝜎𝑡𝑡
```

## 第 61 页：如何渲染单体积模型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 61 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何渲染单体积模型
Out-Scatter & Attenuation
                                                                                                            The Beer-Lambert Law 1



        Standard Beer Law                                              Transmittance ( Tr )

        float GetAttenuation(float
        OpticalDepth)
        {
           return exp(-OpticalDepth);
        }




        Schneider 17

        float GetAttenuation(float OpticalDepth, float CosTheta)
        {
           // 融合两个Beer定律来弥补由于散射模型评估非物理导致的
        颜色过于暗
           float prim = exp(-OpticalDepth);                                                                        Optical Depth ( τ )
           float scnd = exp(-OpticalDepth * 0.25) * 0.7; // 这里的0.5
        是个衰减参数，可以开给用户
           return max(remap(CosTheta, 0.7, 1.0, scnd, scnd * 0.25) ,                          𝜎𝜎𝑡𝑡 = 𝜎𝜎𝑠𝑠 + 𝜎𝜎𝑎𝑎
        prim);
        }                                                                                     𝑇𝑇𝑟𝑟 𝑥𝑥 → 𝑥𝑥 ′ = 𝑒𝑒 −𝜎𝜎𝑡𝑡 𝑑𝑑 = 𝑒𝑒 𝜏𝜏
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
        Standard Beer Law                                              Transmittance ( Tr )
        float GetAttenuation(float
           return exp(-OpticalDepth);
        float GetAttenuation(float OpticalDepth, float CosTheta)
           // 融合两个Beer定律来弥补由于散射模型评估非物理导致的
           float prim = exp(-OpticalDepth);                                                                        Optical Depth ( τ )
           float scnd = exp(-OpticalDepth * 0.25) * 0.7; // 这里的0.5
           return max(remap(CosTheta, 0.7, 1.0, scnd, scnd * 0.25) ,                          𝜎𝜎𝑡𝑡 = 𝜎𝜎𝑠𝑠 + 𝜎𝜎𝑎𝑎
        prim);
        }                                                                                     𝑇𝑇𝑟𝑟 𝑥𝑥 → 𝑥𝑥 ′ = 𝑒𝑒 −𝜎𝜎𝑡𝑡 𝑑𝑑 = 𝑒𝑒 𝜏𝜏
```

## 第 62 页：如何渲染单体积模型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 62 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何渲染单体积模型
In-Scatter & Emissive

   Schneider 15

   float GetPowderEffect(float Density, float CosTheta)
   {
      float powd = 1.0 - exp(-Density * 2.0); // 这里的常数2.0可以根据需要暴露出去
      return lerp(1.0, powd, saturate((-CosTheta * 0.5) + 0.5)); // [-1,1]->[0,1]
   }



   Schneider 17

   float GetInScatterProbability(float3 p, float ds_loded)
   {
      // 计算深度密度函数
      float depth_probability = 0.05 + pow(ds_loded, remap(height_fraction, 0.3, 0.85, 0.5, 2.0));

       // 计算垂向分布
       float vertical_probability = pow(remap(height_fraction, 0.07, 0.14, 0.1, 1.0 ), 0.8 );

       // 最终的内散射计算
       float in_scatter_probability = depth_probability * vertical_probability;

       return in_scatter_probability;
   }


   Standard
   float3 GetScatterCoefficient(float3 Albedo, float3 SigmaT)
   {
      return Albedo * SigmaT;
   }
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
   float GetPowderEffect(float Density, float CosTheta)
      float powd = 1.0 - exp(-Density * 2.0); // 这里的常数2.0可以根据需要暴露出去
      return lerp(1.0, powd, saturate((-CosTheta * 0.5) + 0.5)); // [-1,1]->[0,1]
   float GetInScatterProbability(float3 p, float ds_loded)
      // 计算深度密度函数
      float depth_probability = 0.05 + pow(ds_loded, remap(height_fraction, 0.3, 0.85, 0.5, 2.0));
       // 计算垂向分布
       float vertical_probability = pow(remap(height_fraction, 0.07, 0.14, 0.1, 1.0 ), 0.8 );
       // 最终的内散射计算
       float in_scatter_probability = depth_probability * vertical_probability;
       return in_scatter_probability;
   float3 GetScatterCoefficient(float3 Albedo, float3 SigmaT)
      return Albedo * SigmaT;
```

## 第 63 页：如何渲染单体积模型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 63 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何渲染单体积模型
Phase function based directional scattering


                           Rayleigh scattering                                                 1 − 𝑔𝑔2
                𝑥𝑥 ≤ 1                                                 𝑃𝑃ℎ𝑔𝑔 𝜃𝜃,𝑔𝑔 =
       2𝜋𝜋𝑟𝑟                                                                         4𝜋𝜋 1 + 𝑔𝑔2 − 2𝑔𝑔 cos 𝜃𝜃 1.5
𝑥𝑥 =            𝑥𝑥 ≈ 1     Mie scattering
        𝜆𝜆                                                                               1
                𝑥𝑥 ≥ 1     Geometric scattering                            𝑃𝑃𝑖𝑖𝑖𝑖𝑖𝑖 =
                                                                                        4𝜋𝜋




 Hillaire 16
 float SamplePhase(PhaseBlend)
 {
    float forward_hg_scatter = HenyeyGreenstein(_PhaseG, _CosTheta);
    float back_hg_scatter = HenyeyGreenstein(_PhaseG2, _CosTheta);
    return lerp(forward_hg_scatter, back_hg_scatter, PhaseBlend)
 }




 Schneider 17
 float SamplePhase(float silver_intensity, float silver_spread)
 {
    return max(HenyeyGreenstein(_PhaseG, _CosTheta), silver_intensity *
           HenyeyGreenstein(0.99 - silver_spread, _CosTheta))
 }
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                𝑥𝑥 ≤ 1                                                 𝑃𝑃ℎ𝑔𝑔 𝜃𝜃,𝑔𝑔 =
𝑥𝑥 =            𝑥𝑥 ≈ 1     Mie scattering
                𝑥𝑥 ≥ 1     Geometric scattering                            𝑃𝑃𝑖𝑖𝑖𝑖𝑖𝑖 =
 float SamplePhase(PhaseBlend)
    float forward_hg_scatter = HenyeyGreenstein(_PhaseG, _CosTheta);
    float back_hg_scatter = HenyeyGreenstein(_PhaseG2, _CosTheta);
    return lerp(forward_hg_scatter, back_hg_scatter, PhaseBlend)
 float SamplePhase(float silver_intensity, float silver_spread)
    return max(HenyeyGreenstein(_PhaseG, _CosTheta), silver_intensity *
           HenyeyGreenstein(0.99 - silver_spread, _CosTheta))
```

## 第 64 页：如何渲染单体积模型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 64 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何渲染单体积模型
Final VRE : Frostbite 15 (Unreal Engine) Expand




                                                遮挡物反射光                        当前采样点透光率* 散射光 散射系数                                  自发光*                    黑体辐射*


                                                                                   𝑆𝑆
                                 𝐿𝐿 𝑥𝑥, 𝜔𝜔   = 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑠𝑠 𝐿𝐿 𝑥𝑥𝑠𝑠 , 𝜔𝜔   + � 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑡𝑡 𝐿𝐿𝑠𝑠 𝑥𝑥𝑡𝑡 , 𝜔𝜔 𝜎𝜎𝑠𝑠 𝑥𝑥𝑡𝑡 + 𝐿𝐿𝑒𝑒 𝑥𝑥𝑡𝑡 , 𝜔𝜔 ∗ 𝜎𝜎𝑎𝑎 𝑥𝑥𝑡𝑡 + 𝐿𝐿𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵 𝑑𝑑𝑡𝑡
                                                                                 0




                                                𝐿𝐿𝐿𝐿𝐿𝐿𝐿𝐿𝐿𝐿𝐿                                                                                        𝑥𝑥𝑡𝑡
                                𝐿𝐿𝑠𝑠 𝑥𝑥𝑡𝑡 , 𝜔𝜔 = � 𝑃𝑃 𝜔𝜔, 𝐿𝐿 𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 𝐿𝐿𝑖𝑖 𝑥𝑥, 𝐿𝐿                               𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑡𝑡 = 𝑒𝑒𝑒𝑒𝑒𝑒 − � 𝜎𝜎𝑡𝑡 𝑥𝑥 𝑑𝑑𝑡𝑡
                                                                                                                                                  𝑥𝑥
                                                  𝐿𝐿=0



                                𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 = 𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆 𝑥𝑥, 𝐿𝐿 ∗ 𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿            𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 = 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝐿𝐿
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
Final VRE : Frostbite 15 (Unreal Engine) Expand
                                 𝐿𝐿 𝑥𝑥, 𝜔𝜔   = 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑠𝑠 𝐿𝐿 𝑥𝑥𝑠𝑠 , 𝜔𝜔   + � 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑡𝑡 𝐿𝐿𝑠𝑠 𝑥𝑥𝑡𝑡 , 𝜔𝜔 𝜎𝜎𝑠𝑠 𝑥𝑥𝑡𝑡 + 𝐿𝐿𝑒𝑒 𝑥𝑥𝑡𝑡 , 𝜔𝜔 ∗ 𝜎𝜎𝑎𝑎 𝑥𝑥𝑡𝑡 + 𝐿𝐿𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵𝐵 𝑑𝑑𝑡𝑡
                                𝐿𝐿𝑠𝑠 𝑥𝑥𝑡𝑡 , 𝜔𝜔 = � 𝑃𝑃 𝜔𝜔, 𝐿𝐿 𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 𝐿𝐿𝑖𝑖 𝑥𝑥, 𝐿𝐿                               𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝑡𝑡 = 𝑒𝑒𝑒𝑒𝑒𝑒 − � 𝜎𝜎𝑡𝑡 𝑥𝑥 𝑑𝑑𝑡𝑡
                                                  𝐿𝐿=0
                                𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 = 𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆𝑆 𝑥𝑥, 𝐿𝐿 ∗ 𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿            𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉𝑉 𝑥𝑥, 𝐿𝐿 = 𝑇𝑇𝑟𝑟 𝑥𝑥, 𝑥𝑥𝐿𝐿
```

## 第 65 页：如何渲染单体积模型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 65 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何渲染单体积模型
Analyze Integration: Hillaire 16




 // 使用Sebastien Hillaire的分析积分进行数值过程
 #if ANALYTIC_INTEGRATION
   float SafeExtinctionCoefficients = max(0.000001f, ExtinctionCoefficients);
   float SafePathSegmentTransmittance = exp(-SafeExtinctionCoefficients * PathSegmentCloudDensity);
                                                                                                                                         𝐷𝐷                                     −𝜎𝜎𝑡𝑡 𝐷𝐷
   float3 LuminanceIntegral = (ScatteredLuminance - ScatteredLuminance * SafePathSegmentTransmittance) / SafeExtinctionCoefficients;                            𝑆𝑆 − 𝑆𝑆 ×    𝑒𝑒
   Radiance += Transmittance * LuminanceIntegral;                                                                                      � 𝑒𝑒 −𝜎𝜎𝑡𝑡𝑥𝑥 × 𝑆𝑆 𝑑𝑑𝑥𝑥 =
 #else
                                                                                                                                        0                               𝜎𝜎𝑡𝑡
   float SafePathSegmentTransmittance = exp(-ExtinctionCoefficients * PathSegmentCloudDensity);
   Radiance += Transmittance * ScatteredLuminance * PathSegmentCloudDensity;
 #endif

 // 更新透射率
 Transmittance *= SafePathSegmentTransmittance;
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
 // 使用Sebastien Hillaire的分析积分进行数值过程
 #if ANALYTIC_INTEGRATION
   float SafeExtinctionCoefficients = max(0.000001f, ExtinctionCoefficients);
   float SafePathSegmentTransmittance = exp(-SafeExtinctionCoefficients * PathSegmentCloudDensity);
   float3 LuminanceIntegral = (ScatteredLuminance - ScatteredLuminance * SafePathSegmentTransmittance) / SafeExtinctionCoefficients;                            𝑆𝑆 − 𝑆𝑆 ×    𝑒𝑒
   Radiance += Transmittance * LuminanceIntegral;                                                                                      � 𝑒𝑒 −𝜎𝜎𝑡𝑡𝑥𝑥 × 𝑆𝑆 𝑑𝑑𝑥𝑥 =
 #else
   float SafePathSegmentTransmittance = exp(-ExtinctionCoefficients * PathSegmentCloudDensity);
   Radiance += Transmittance * ScatteredLuminance * PathSegmentCloudDensity;
 #endif
 // 更新透射率
 Transmittance *= SafePathSegmentTransmittance;
```

## 第 66 页：如何渲染单体积模型 Ambient Light

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 66 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何渲染单体积模型                                             Ambient Light
                                                      float3 GetDistantSkyLightLuminance(bool SkyAtmospherePresentInScene, float3 Normal)
                                                      {
                                                         float3 DistantSkyLightLuminance = 0.0;
Ambient light contribution                                if (SkyAtmospherePresentInScene)
                                                          {
                                                             DistantSkyLightLuminance = Texture2DSampleLevel(View.DistantSkyLightLutTexture, View.DistantSkyLightLutTextureSampler, float2(0.5f, 0.5f), 0.0f).rgb;
                                                          }
                                                          else
                                                          {
                                                             DistantSkyLightLuminance = GetSkySHDiffuse(Normal) * View.SkyLightColor.rgb;
                                                          }

                                                          return DistantSkyLightLuminance;
                                                      }




                                                      Transmittance along sky
                                                      float GetTransmittanceTowardSky(float3 LocalRayPos, float3 Offset, int Mip, float AmbientDensity, SamplerState
                                                      ShapeTexSampler)
                                                      {
                                                         float DensityToSky = 0;
                                                         float cursample = 0;

                                                          float3 light_pos = LocalRayPos + Offset;
                                                          cursample = GetShapeDensity(saturate(light_pos), Mip, ShapeTexSampler).r;
                                                          DensityToSky += cursample;

                                                          light_pos += Offset ;
                                                          cursample = GetShapeDensity(saturate(light_pos), Mip, ShapeTexSampler).r;
                                                          DensityToSky += cursample;

                                                          light_pos += Offset;
                                                          cursample = GetShapeDensity(saturate(light_pos), Mip, ShapeTexSampler).r;
                                                          DensityToSky += cursample;

                                                          float energy = exp(-DensityToSky * AmbientDensity);


                             环境光保持与UE一致
                                                          return energy;
                                                      }


                             AO重新拟合
                             只对一阶散射评估环境光
                             基于 Ryan Brucks 的思路进行3次
                             偏移采样累积环境光衰减
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                      float3 GetDistantSkyLightLuminance(bool SkyAtmospherePresentInScene, float3 Normal)
                                                         float3 DistantSkyLightLuminance = 0.0;
Ambient light contribution                                if (SkyAtmospherePresentInScene)
                                                             DistantSkyLightLuminance = Texture2DSampleLevel(View.DistantSkyLightLutTexture, View.DistantSkyLightLutTextureSampler, float2(0.5f, 0.5f), 0.0f).rgb;
                                                          else
                                                             DistantSkyLightLuminance = GetSkySHDiffuse(Normal) * View.SkyLightColor.rgb;
                                                          return DistantSkyLightLuminance;
                                                      float GetTransmittanceTowardSky(float3 LocalRayPos, float3 Offset, int Mip, float AmbientDensity, SamplerState
                                                         float DensityToSky = 0;
                                                         float cursample = 0;
                                                          float3 light_pos = LocalRayPos + Offset;
                                                          cursample = GetShapeDensity(saturate(light_pos), Mip, ShapeTexSampler).r;
                                                          DensityToSky += cursample;
                                                          light_pos += Offset ;
                                                          cursample = GetShapeDensity(saturate(light_pos), Mip, ShapeTexSampler).r;
                                                          DensityToSky += cursample;
                                                          light_pos += Offset;
                                                          cursample = GetShapeDensity(saturate(light_pos), Mip, ShapeTexSampler).r;
                                                          DensityToSky += cursample;
                                                          float energy = exp(-DensityToSky * AmbientDensity);
                                                          return energy;
```

## 第 67 页：Unreal Engine

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 67 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
                                                                                          Unreal Engine

                                                                                          struct ParticipatingMediaPhaseContext
                                                                                          {
                                                                                             float Phase[MSCOUNT];
                                                                                          };


如何渲染单体积模型
                                                                                          struct ParticipatingMediaContext
                                                                                          {
                                                                                             float3 ScatteringCoefficients[MSCOUNT];
                                                                                             float ExtinctionCoefficients[MSCOUNT];
                                                                                             float TransmittanceToLight[MSCOUNT];
Integrated multi-scattering                                                               };

                                                                                          ParticipatingMediaContext SetupParticipatingMediaContext(float3 BaseAlbedo, float BaseExtinctionCoefficients, float
Schneider 22                                                                              MsSFactor, float MsEFactor, float InitialTransmittanceToLight)
                                                                                          {
Direct Scattering = (Transmittance * Primary Scattering Phase) + (Multiple Scattering *
                                                                                            float3 ScatteringCoefficients = BaseAlbedo * BaseExtinctionCoefficients;
Secondary Scattering Phase)
                                                                                              ParticipatingMediaContext PMC;
float ms_volume = Remap(dimensional_profile* step_size, 0.1, 1.0, 0.0, 1.0) *
pow(cloud_coverage * cloud_type, 0.25);
                                                                                              PMC.ScatteringCoefficients[0] = ScatteringCoefficients;
ms_volume *= pow(attenuated_light, cMultipleScatteringDepthPower);
                                                                                              PMC.ExtinctionCoefficients[0] = BaseExtinctionCoefficients;
ms_volume *= pow(height_fraction, cMultipleScatteringHeightPower);
                                                                                              PMC.TransmittanceToLight[0] = InitialTransmittanceToLight;

                                                                                              for(int ms = 1; ms < MSCOUNT; ++ms)
                                                                                              {
                                                                                                PMC.ScatteringCoefficients[ms] = PMC.ScatteringCoefficients[ms - 1] * MsSFactor;
                                                                                                PMC.ExtinctionCoefficients[ms] = PMC.ExtinctionCoefficients[ms - 1] * MsEFactor;
                                                                                                MsSFactor *= MsSFactor;
                                                                                                MsEFactor *= MsEFactor;

                                                                                                  PMC.TransmittanceToLight[ms] = InitialTransmittanceToLight;
                                                                                              }

                                                                                              return PMC;
                                                                                          }

                                                                                          ParticipatingMediaPhaseContext SetupParticipatingMediaPhaseContext(float BasePhase, float MsPhaseFactor)
                                                                                          {
                                                                                            ParticipatingMediaPhaseContext PMPC;
                                                                                            PMPC.Phase[0] = BasePhase;

                                                                                              for(int ms = 1; ms < MSCOUNT; ++ms)
                                                                                              {
                                                                                                PMPC.Phase[ms] = lerp(1.0f / (4.0f * PI), PMPC.Phase[0], MsPhaseFactor);

                                                                                                  MsPhaseFactor *= MsPhaseFactor;
                                                                                              }

                                                                                              return PMPC;
                                                                                          }
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
                                                                                          struct ParticipatingMediaPhaseContext
                                                                                             float Phase[MSCOUNT];
                                                                                          struct ParticipatingMediaContext
                                                                                             float3 ScatteringCoefficients[MSCOUNT];
                                                                                             float ExtinctionCoefficients[MSCOUNT];
                                                                                             float TransmittanceToLight[MSCOUNT];
Integrated multi-scattering                                                               };
                                                                                          ParticipatingMediaContext SetupParticipatingMediaContext(float3 BaseAlbedo, float BaseExtinctionCoefficients, float
Schneider 22                                                                              MsSFactor, float MsEFactor, float InitialTransmittanceToLight)
Direct Scattering = (Transmittance * Primary Scattering Phase) + (Multiple Scattering *
                                                                                            float3 ScatteringCoefficients = BaseAlbedo * BaseExtinctionCoefficients;
                                                                                              ParticipatingMediaContext PMC;
float ms_volume = Remap(dimensional_profile* step_size, 0.1, 1.0, 0.0, 1.0) *
pow(cloud_coverage * cloud_type, 0.25);
                                                                                              PMC.ScatteringCoefficients[0] = ScatteringCoefficients;
ms_volume *= pow(attenuated_light, cMultipleScatteringDepthPower);
                                                                                              PMC.ExtinctionCoefficients[0] = BaseExtinctionCoefficients;
ms_volume *= pow(height_fraction, cMultipleScatteringHeightPower);
                                                                                              PMC.TransmittanceToLight[0] = InitialTransmittanceToLight;
                                                                                              for(int ms = 1; ms < MSCOUNT; ++ms)
                                                                                                PMC.ScatteringCoefficients[ms] = PMC.ScatteringCoefficients[ms - 1] * MsSFactor;
                                                                                                PMC.ExtinctionCoefficients[ms] = PMC.ExtinctionCoefficients[ms - 1] * MsEFactor;
                                                                                                MsSFactor *= MsSFactor;
                                                                                                MsEFactor *= MsEFactor;
                                                                                                  PMC.TransmittanceToLight[ms] = InitialTransmittanceToLight;
                                                                                              return PMC;
                                                                                          ParticipatingMediaPhaseContext SetupParticipatingMediaPhaseContext(float BasePhase, float MsPhaseFactor)
                                                                                            ParticipatingMediaPhaseContext PMPC;
                                                                                            PMPC.Phase[0] = BasePhase;
                                                                                              for(int ms = 1; ms < MSCOUNT; ++ms)
                                                                                                PMPC.Phase[ms] = lerp(1.0f / (4.0f * PI), PMPC.Phase[0], MsPhaseFactor);
                                                                                                  MsPhaseFactor *= MsPhaseFactor;
                                                                                              return PMPC;
```

## 第 68 页：如何渲染单体积模型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 68 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何渲染单体积模型
Single scattering & multi-scattering




                                  单散射   多散射
```

## 第 69 页：如何渲染单体积模型

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 69 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
如何渲染单体积模型
Two render processes ?
                                                                                Global Shader
                                                                                Mesh Material Shader
                         Mesh SDF           Global SDF



           Mesh, VDB                                     Single Volume Render



                         Mesh SDF / Level sets
```

## 第 70 页：Single Volume Renderer

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 70 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
Single Volume Renderer



单体积渲染器用例介绍
```

## 第 71 页：基于 Single Volume 的体积云渲染

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 71 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
基于 Single Volume 的体积云渲染
                          ShapeProfile: 单体轮廓剖面     Detail Noise Scale: 侵蚀噪声缩放
                          View Steps: 主步进次数        Wind & Scroll: 控制风场
User Attributes
                          Shadow Steps: 阴影步进次数     Phase : 控制光散射
                          Density: 密度增益值（不是密度！）    Color : 介质反照率
                          Shadow Multiple: 光吸收倍乘   Emissive : 自发光模块
                          Shadow Threshold: 中断阈值   Ambient : 环境光贡献倍率
```

## 第 72 页：基于 Single Volume 的体积云渲染

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 72 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
基于 Single Volume 的体积云渲染
7个核心数据封装为 Preset




 Noise Mod: 最终密度场的评估模式
 Shape Profile: 形状剖面类型
 View Steps: RayBox内的步进次数，影响单次步进距离
 Shadow Steps: RayBox内的阴影采样数，影响阴影表现
 Density: 密度增益值
 Shadow Multiplier: 缩放光吸收
 Scale: SingleVolume的缩放




                                      可以像操作 VDB 云那样在场景内
                                      自由摆放，旋转，缩放
```

## 第 73 页：基于 Single Volume 的体积云渲染

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 73 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
基于 Single Volume 的体积云渲染
还原国际云图

                                                   Cirrocumulus                    Cirrostratus      Cirrus
 Cirrostratus: 卷层云
 Cirrocumulus: 卷积云        High Level
 Cirrus: 卷云
 Altostratus: 高层云
 Altocumulus: 高积云
 Stratocumulus: 层积云                         Altocumulus
 Stratus: 层云              Middle Level                               Altostratus
 Cumulonimbus: 积雨云                                                                            Cumulonimbus
 Cumulus: 积云

                                                          Stratocumulus
                          Low Level
                                         Stratus
                                                                                          Cumulus
```

## 第 74 页：基于 Single Volume 的体积云渲染

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 74 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
基于 Single Volume 的体积云渲染
各式各样的云
```

## 第 75 页：mesh volume

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 75 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
mesh   volume
```

## 第 76 页：基于 Single Volume

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 76 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
基于 Single Volume
的体积云渲染
Shape Profile Viewer Toolkit




  用于可视化形状剖面
  一些独立的核心着色器代码
  只包含基础步进功能
  最简化的控制参数
```

## 第 77 页：天空音序器 编辑工具组

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 77 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
天空音序器                                      编辑工具组

体积天空造景工具                                   Cloud Compositing

                                                                    内置渲染器
                                           Cloud Projection Brush

                                                                    Multiple Cloud Layers
                                           Cloud Painter


                                           Profile Baker                                     云动画编辑器

                         通用算法组件                                                              Sky Sequence

                                           VDB Toolkit
                      JFA 2D Component                              插件式集成

                      JFA 3D Component     MeshToSDF                Single Volume Renderer

           Fluid Simulation 2D Component
                                           Shape Profile Viewer
           Fluid Simulation 3D Component



                                           Noise Generator


                                           Flowmap Baker               Sky Capture


                                           资产生成
```

## 第 78 页： 研发路线：以PC平台为基础，分策略将技术下放至移动平台，实现体积方案的多端覆盖

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 78 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
   研发路线：以PC平台为基础，分策略将技术下放至移动平台，实现体积方案的多端覆盖

   截至目前，体积流体工具包已应用于多款项目

   我们仍在积极探索下一代实时体积渲染技术（直接应用VDB）的诸多可能，并已经有了一些成果

       通用体积渲染模型

       直接使用VDB进行实时体渲染

       稀疏体素数据的神经压缩过程

       散射模型与高阶散射拟合

   这些成果将直接用于解决项目实际问题，提升技术竞争力与产品上限

   体积效果是游戏引擎内提升视觉表现力的关键因素

   欢迎大家与我们建立联系
```

## 第 79 页：体积云方案与体积渲染方案参考

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 79 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
体积云方案与体积渲染方案参考
[1] [SIGGRAPH 2015],Andrew Schneider,《The Real-time Volumetric Cloudscapes of Horizon: Zero Dawn》

[2] [SIGGRAPH 2016], Sebastien Hillaire,《Physically Based Sky, Atmosphere and Cloud Rendering in Frostbite》

[3] [SIGGRAPH 2017], Andrew Schneider,《Nubis: Authoring Real-Time Volumetric Cloudscapes with the Decima Engine》

[4] [SIGGRAPH 2019], RAGE,《Creating the Atmospheric World of Red Dead Redemption 2: A Complete and Integrated Solution》

[5] [SIGGRAPH 2022], Andrew Schneider,《Nubis, Evolved: Real-Time Volumetric Clouds for Skies, Environments, and VFX》

[6] [SIGGRAPH 2023], Andrew Schneider,《Nubis3: Methods (and madness) to model and render immersive real-time voxel-based clouds》


[7] [Ray Tracing Gems II], Nikolai Hofmann, Alex Evans,《Efficient Unbiased Volume Path Tracing on the GPU》

[8] [SIGGRAPH 2012], B. Miller, K. Museth, D. Penney and N. Bin Zafar《Cloud Modeling and Rendering for “Puss In Boots”》

[9] [SIGGRAPH 2017], Peter Kutz, Ralf Habel, Yining Kar Li and Jan Novak 《Spectral and decomposition tracking for rendering heterogeneous volumes》




开源库参考
[1] Thibault Lambert’s “Sparse Volumetric” : https://github.com/thilamb/unreal-vdb/tree/5.2

[2] Disney Data Sets – “Wdas Cloud” : https://www.disneyanimation.com/data-sets/?




相关网站
[1] Open VDB: https://www.openvdb.org/

[2] 国际气象组织， 云图集：https://cloudatlas.wmo.int/en/home.html
```

### 代码／接口转录

> 代码来自 PDF 页面中的可见文本或人工重建。若原幻灯片为多栏代码，行序可能需要结合原页复核。

```hlsl
[1] [SIGGRAPH 2015],Andrew Schneider,《The Real-time Volumetric Cloudscapes of Horizon: Zero Dawn》
[2] [SIGGRAPH 2016], Sebastien Hillaire,《Physically Based Sky, Atmosphere and Cloud Rendering in Frostbite》
[3] [SIGGRAPH 2017], Andrew Schneider,《Nubis: Authoring Real-Time Volumetric Cloudscapes with the Decima Engine》
[4] [SIGGRAPH 2019], RAGE,《Creating the Atmospheric World of Red Dead Redemption 2: A Complete and Integrated Solution》
[5] [SIGGRAPH 2022], Andrew Schneider,《Nubis, Evolved: Real-Time Volumetric Clouds for Skies, Environments, and VFX》
[6] [SIGGRAPH 2023], Andrew Schneider,《Nubis3: Methods (and madness) to model and render immersive real-time voxel-based clouds》
[7] [Ray Tracing Gems II], Nikolai Hofmann, Alex Evans,《Efficient Unbiased Volume Path Tracing on the GPU》
[8] [SIGGRAPH 2012], B. Miller, K. Museth, D. Penney and N. Bin Zafar《Cloud Modeling and Rendering for “Puss In Boots”》
[9] [SIGGRAPH 2017], Peter Kutz, Ralf Habel, Yining Kar Li and Jan Novak 《Spectral and decomposition tracking for rendering heterogeneous volumes》
[1] Thibault Lambert’s “Sparse Volumetric” : https://github.com/thilamb/unreal-vdb/tree/5.2
[2] Disney Data Sets – “Wdas Cloud” : https://www.disneyanimation.com/data-sets/?
[1] Open VDB: https://www.openvdb.org/
[2] 国际气象组织， 云图集：https://cloudatlas.wmo.int/en/home.html
```

## 第 80 页：THANKS!

> 来源：`在UE5中打造多层体积天空-终稿【最终版】.pdf` 第 80 页。以下为自动 Markdown 转译；多栏排版可能发生行序交错。

### 页面文字

```text
THANKS!
Unreal Fest 2023
```
