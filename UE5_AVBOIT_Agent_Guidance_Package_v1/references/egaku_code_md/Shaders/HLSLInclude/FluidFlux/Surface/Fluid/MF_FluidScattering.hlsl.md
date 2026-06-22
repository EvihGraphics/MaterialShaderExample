# Egaku source: `Shaders/HLSLInclude/FluidFlux/Surface/Fluid/MF_FluidScattering.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
// MF_FluidScattering

// Just use "Cheap Scattering" is enough.
float MF_FluidScattering(float3 CameraDir, float3 VertexNormal, float3 PixelNormal, float3 PosWS)
{
    float FluidScattering;
    float FluidAdvancedScattering;
    FluidAdvancedScattering = MF_FluidAdvancedScattering(CameraDir, PixelNormal, PosWS);

    float Vertex_NoV = dot(VertexNormal, CameraDir);
    float Pixel_NoV = dot(PixelNormal, CameraDir);

    FluidScattering = lerp(Vertex_NoV, abs(Pixel_NoV), _CheapScatteringDetails);
    FluidScattering -= CameraDir.g;
    FluidScattering = saturate(FluidScattering);
    FluidScattering = pow(FluidScattering, _CheapScatteringPower);
    FluidScattering = saturate(FluidScattering * _CheapScatteringScale);
    FluidScattering = _UseCheapScattering > 0 ? FluidScattering : FluidAdvancedScattering;

    return FluidScattering;
}
```
