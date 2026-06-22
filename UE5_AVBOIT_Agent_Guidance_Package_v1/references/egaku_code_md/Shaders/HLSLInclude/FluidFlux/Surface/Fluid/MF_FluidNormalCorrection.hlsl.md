# Egaku source: `Shaders/HLSLInclude/FluidFlux/Surface/Fluid/MF_FluidNormalCorrection.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// MF_FluidNormalCorrection

void MF_FluidNormalCorrection (inout MaterialAttributes InputLayer, in FSurfacePositionData PosData)
{
    InputLayer.Normal = MF_ImposibleNormalFix(InputLayer.Normal, PosData.CameraVectorWS);
}
```
