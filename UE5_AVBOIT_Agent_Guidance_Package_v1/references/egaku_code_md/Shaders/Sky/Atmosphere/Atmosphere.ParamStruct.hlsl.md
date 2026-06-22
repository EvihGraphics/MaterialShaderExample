# Egaku source: `Shaders/Sky/Atmosphere/Atmosphere.ParamStruct.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// This file was generated from Modules.Sky.Atmosphere.Material.AtmosphereShaderParamTable by Egaku.Rendering.Pipeline.Common.ShaderCodeGenerator.
// Please do not modify it manually.

#ifndef Atmosphere_ParamStruct_hlsl
#define Atmosphere_ParamStruct_hlsl
#include "Atmosphere.ParamTable.hlsl"


#define FetchParam(structName) Get##structName()
#define DeclareParamFetch(structName, var) structName var = FetchParam(structName)


// Declare material parameter struct AtmosphereVolumeDesc
struct AtmosphereVolumeDesc
{
	float atmosphereDensityScale;
	float dustDensityFraction;
	float airDensityFraction;
	float ozoneDensityFraction;
	float mieScatteringAnisotropy;
};


// Getter for struct AtmosphereVolumeDesc
AtmosphereVolumeDesc GetAtmosphereVolumeDesc()
{
	AtmosphereVolumeDesc result;
	result.atmosphereDensityScale = _AtmosphereVolume_atmosphereDensityScale;
	result.dustDensityFraction = _AtmosphereVolume_dustDensityFraction;
	result.airDensityFraction = _AtmosphereVolume_airDensityFraction;
	result.ozoneDensityFraction = _AtmosphereVolume_ozoneDensityFraction;
	result.mieScatteringAnisotropy = _AtmosphereVolume_mieScatteringAnisotropy;
	return result;
}




#endif // Atmosphere_ParamStruct_hlsl
```
