# Egaku source: `Shaders/Sky/Skybox/Skybox.ParamStruct.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// This file was generated from Modules.Sky.Skybox.Material.SkyboxShaderParamTable by Egaku.Rendering.Pipeline.Common.ShaderCodeGenerator.
// Please do not modify it manually.

#ifndef Skybox_ParamStruct_hlsl
#define Skybox_ParamStruct_hlsl
#include "Skybox.ParamTable.hlsl"


#define FetchParam(structName) Get##structName()
#define DeclareParamFetch(structName, var) structName var = FetchParam(structName)


// Declare material parameter struct SkyboxDesc
struct SkyboxDesc
{
	float sunLightIntensity;
};


// Getter for struct SkyboxDesc
SkyboxDesc GetSkyboxDesc()
{
	SkyboxDesc result;
	result.sunLightIntensity = _Skybox_sunLightIntensity;
	return result;
}




#endif // Skybox_ParamStruct_hlsl
```
