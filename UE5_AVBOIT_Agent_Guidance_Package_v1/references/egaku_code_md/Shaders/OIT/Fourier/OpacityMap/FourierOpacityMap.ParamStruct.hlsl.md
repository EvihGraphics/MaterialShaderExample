# Egaku source: `Shaders/OIT/Fourier/OpacityMap/FourierOpacityMap.ParamStruct.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// This file was generated from Modules.OIT.Fourier.OpacityMap.Material.FOMShaderParamTable by Egaku.Rendering.Pipeline.Common.ShaderCodeGenerator.
// Please do not modify it manually.

#ifndef FourierOpacityMap_ParamStruct_hlsl
#define FourierOpacityMap_ParamStruct_hlsl
#include "FourierOpacityMap.ParamTable.hlsl"


#define FetchParam(structName) Get##structName()
#define DeclareParamFetch(structName, var) structName var = FetchParam(structName)


// Declare material parameter struct TranslucencyDesc
struct TranslucencyDesc
{
	float4 baseColor;
};


// Getter for struct TranslucencyDesc
TranslucencyDesc GetTranslucencyDesc()
{
	TranslucencyDesc result;
	result.baseColor = _Translucency_baseColor;
	return result;
}




#endif // FourierOpacityMap_ParamStruct_hlsl
```
