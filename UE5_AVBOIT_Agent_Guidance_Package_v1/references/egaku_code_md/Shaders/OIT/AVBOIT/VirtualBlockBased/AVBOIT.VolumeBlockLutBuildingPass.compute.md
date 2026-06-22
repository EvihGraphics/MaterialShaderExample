# Egaku source: `Shaders/OIT/AVBOIT/VirtualBlockBased/AVBOIT.VolumeBlockLutBuildingPass.compute`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// This file was generated from OIT.AVBOIT.Material.AVBOITComputeShaderConfig+VirtualBlockBasedShaderConfig+VolumeBlockLutBuildingCSConfig by Egaku.Rendering.Pipeline.Common.ShaderCodeGenerator.
// Please do not modify it manually.

#include "Passes/VolumeBlockLutBuildingPass.hlsl"


#pragma kernel Kernel_BuildVolumeBlockLut
#pragma kernel Kernel_BuildProtectionPlane
#pragma kernel Kernel_BuildVolumeBlockLutParallel_Scan
#pragma kernel Kernel_BuildVolumeBlockLutParallel_InitAuxiliaryArray
#pragma kernel Kernel_BuildVolumeBlockLutParallel_FinalSum
```
