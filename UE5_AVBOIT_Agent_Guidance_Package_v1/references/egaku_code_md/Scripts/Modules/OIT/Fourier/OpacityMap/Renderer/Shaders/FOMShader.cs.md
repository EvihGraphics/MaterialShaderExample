# Egaku source: `Scripts/Modules/OIT/Fourier/OpacityMap/Renderer/Shaders/FOMShader.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿


using Modules.OIT.Fourier.OpacityMap.Material;
using UnityEngine;
using UnityEngine.Rendering;

namespace Modules.OIT.Fourier.OpacityMap.Renderer.Shaders {
    
    public struct FOMShader {
        struct GraphicsPassId {
            public int fourierCoeffsBuildingPassId;
            public int translucencyForwardPassId;
            public int backgroundResolutionPassId;
        }

        private static readonly FOMShaderConfig _graphicsShaderConfig = new();
        private static GraphicsPassId _graphicsPassId;
        private static RTHandle _fourierCoeffsLut;
        private static UnityEngine.Material _defaultMaterial;

        private UnityEngine.Material _material;
        
        public static void GlobalInit() {
            UnityEngine.Material material = new UnityEngine.Material(Shader.Find(_graphicsShaderConfig.ShaderURL));
            _graphicsPassId.fourierCoeffsBuildingPassId = material.FindPass(
                _graphicsShaderConfig
                    .GetOpacityMapShaderConfig()
                    .GetFourierCoeffsBuildingPassName());
            _graphicsPassId.translucencyForwardPassId = material.FindPass(
                _graphicsShaderConfig
                    .GetOpacityMapShaderConfig()
                    .GetTranslucencyForwardPrePassName());
            _graphicsPassId.backgroundResolutionPassId = material.FindPass(
                _graphicsShaderConfig
                    .GetOpacityMapShaderConfig()
                    .GetBackgroundResolutionPassName());
            _defaultMaterial = material;
        }


        public static void SetFourierCoeffsLut(RTHandle rt) {
            Shader.SetGlobalTexture("_FOM_FourierCoeffsLut", rt);
        }
        
        public static void SetFourierCoeffsLut(RTHandle[] rts) {
            for (int i = 0; i < rts.Length; i++) {
                Shader.SetGlobalTexture($"_FOM_FourierCoeffsLut{i}", rts[i]);
            }
        }
        
        public void InitInnerMaterial() {
            _material = new UnityEngine.Material(Shader.Find(_graphicsShaderConfig.ShaderURL));
        }
        
        public void SetBaseColor(in Color color) {
            _material.SetColor("_Translucency_baseColor",color);
        }
        
        public void CmdDrawMeshFourierCoefficientsBuildingPass(CommandBuffer cmd, Mesh mesh,in Matrix4x4 modelMatrix) {
            cmd.DrawMesh(mesh,modelMatrix, _material, 0,_graphicsPassId.fourierCoeffsBuildingPassId);
        }
        
        public void CmdDrawMeshTranslucencyForwardPass(CommandBuffer cmd, Mesh mesh,in Matrix4x4 modelMatrix) {
            cmd.DrawMesh(mesh,modelMatrix, _material, 0,_graphicsPassId.translucencyForwardPassId);
        }

        public static void CmdResolveBackground(CommandBuffer cmd, RTHandle dest) {
            cmd.Blit(null,dest,_defaultMaterial,_graphicsPassId.backgroundResolutionPassId);
        }
    }
}
```
