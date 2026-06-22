# Egaku source: `Shaders/Sky/Atmosphere/Atmosphere.ParamTable.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// This file was generated from Modules.Sky.Atmosphere.Material.AtmosphereShaderParamTable by Egaku.Rendering.Pipeline.Common.ShaderCodeGenerator.
// Please do not modify it manually.
// Codes in this file are functioned as material parameter table, which is used to declare some uniform variables and macro constants.

#ifndef Atmosphere_ParamTable_hlsl
#define Atmosphere_ParamTable_hlsl
#include "Packages/io.github.chiikusa.egaku-render-pipeline/ShaderLibrary/Core.hlsl"


CBUFFER_BEGIN(PerMaterial)
	float _AtmosphereVolume_atmosphereDensityScale;
	float _AtmosphereVolume_dustDensityFraction;
	float _AtmosphereVolume_airDensityFraction;
	float _AtmosphereVolume_ozoneDensityFraction;
	float _AtmosphereVolume_mieScatteringAnisotropy;
CBUFFER_END






#endif // Atmosphere_ParamTable_hlsl
```
