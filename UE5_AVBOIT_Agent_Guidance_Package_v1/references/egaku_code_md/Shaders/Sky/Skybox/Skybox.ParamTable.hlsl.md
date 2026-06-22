# Egaku source: `Shaders/Sky/Skybox/Skybox.ParamTable.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// This file was generated from Modules.Sky.Skybox.Material.SkyboxShaderParamTable by Egaku.Rendering.Pipeline.Common.ShaderCodeGenerator.
// Please do not modify it manually.
// Codes in this file are functioned as material parameter table, which is used to declare some uniform variables and macro constants.

#ifndef Skybox_ParamTable_hlsl
#define Skybox_ParamTable_hlsl
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Core.hlsl"


CBUFFER_BEGIN(PerMaterial)
	float _Skybox_sunLightIntensity;
CBUFFER_END






#endif // Skybox_ParamTable_hlsl
```
