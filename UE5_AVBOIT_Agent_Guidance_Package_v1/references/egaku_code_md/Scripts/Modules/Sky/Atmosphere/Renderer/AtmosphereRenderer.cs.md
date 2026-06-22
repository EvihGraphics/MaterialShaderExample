# Egaku source: `Scripts/Modules/Sky/Atmosphere/Renderer/AtmosphereRenderer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Modules.Sky.Atmosphere.Material;
using Modules.Sky.Atmosphere.Renderer.RenderTargets;
using RenderFeatures.Atmosphere;
using UnityEngine;

using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

namespace Modules.Sky.Atmosphere.Renderer {
    
    [ExecuteAlways]
    public class AtmosphereRenderer : MonoBehaviour {
        [Serializable]
        struct AtmospherePrtConfig {
            public int width;
            public int height;
        }
        
        [Serializable]
        struct AtmosphereTransmittanceLutConfig {
            public int width;
            public int height;
            public int samplePerDirection;
        }

        
        [SerializeField]
        private UnityEngine.Material _material;
        
        [SerializeField]
        private AtmospherePrtConfig _atmospherePrtConfig;
        
        [SerializeField]
        private AtmosphereTransmittanceLutConfig _atmosphereTransmittanceLutConfig;

        [SerializeField]
        private Texture2D _atmosphereTransmittanceLut;
        

        private AtmospherePrtRenderTarget _atmospherePrtRT;
        
        private AtmosphereTransmittanceLutRenderTarget _atmosphereTransmittanceLutRT;
        
        private int _transmittanceLutPassID;
        private int _atmospherePrtPassID;

        private bool _enabled = false;
        public bool Enabled => _enabled;
        
        private bool _firstFrame = true;

        public void SetMaterial(UnityEngine.Material material) {
            _material = material;
        }

        public void SetRenderTargetResolution(int width, int height) {
            _atmospherePrtConfig.width = width;
            _atmospherePrtConfig.height = height;
        }


        public void SetupShaderParameters() {
            _material.SetInteger("_TransmittanceLut_SamplePerDirection",_atmosphereTransmittanceLutConfig.samplePerDirection);
            _material.SetTexture("_AtmosphereTransmittanceLut", _atmosphereTransmittanceLutRT.GetRenderTarget());
        }
        
        public void SetupRenderTargets() {
            _atmospherePrtRT.Setup(_atmospherePrtConfig.width, _atmospherePrtConfig.height);
            _atmosphereTransmittanceLutRT.Setup(_atmosphereTransmittanceLutConfig.width, _atmosphereTransmittanceLutConfig.height);
        }
        
        public void Init(bool withRenderFeature) {
            _enabled = true;
            _firstFrame = true;
            AtmosphereShaderConfig shaderConfig = new();
            _atmospherePrtPassID = _material.FindPass(shaderConfig.SubShaderConfigs[0].PassConfigs[0].PassName);
            _transmittanceLutPassID = _material.FindPass(  shaderConfig.SubShaderConfigs[0].PassConfigs[1].PassName);
            if (withRenderFeature) {
                AtmosphereRenderFeature.SetRenderer(this);
            }
        }

        public void OnEnable() {
            Init(true);
            _enabled = true;
        }
        
        public void OnDisable() {
            _enabled = false;
        }
        
        private void RecordRenderCommandsOctahedronMarchingPass_ComputeImpl(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("Atmosphere Prt Octahedron Marching CS");
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        private void RecordRenderCommandsOctahedronMarchingPass_RasterImpl(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("Atmosphere Prt Octahedron Marching");
            cmd.Blit(null, _atmospherePrtRT.GetRenderTarget(),_material, _atmospherePrtPassID);
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        
        private void RecordRenderCommandsTransmittanceLutPass_RasterImpl(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("Atmosphere Transmittance Lut");
            cmd.Blit(null, _atmosphereTransmittanceLutRT.GetRenderTarget(),_material, _transmittanceLutPassID);
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        public void RecordRenderCommandsTransmittanceLutPass(CommandBuffer cmd) {
            cmd.Blit(null, _atmospherePrtRT.GetRenderTarget(),_material, _transmittanceLutPassID);
        }
        
        public void RecordRenderCommandsTransmittanceLutPass(ref ScriptableRenderContext context, ref RenderingData data) {
            RecordRenderCommandsTransmittanceLutPass_RasterImpl(ref context,ref data);
        }
        
        public void RecordRenderCommandsAtmospherePrtPass(ref ScriptableRenderContext context, ref RenderingData data) {
            RecordRenderCommandsOctahedronMarchingPass_RasterImpl(ref context,ref data);
        }
        
        public RTHandle GetAtmospherePrtRenderTarget() {
            return _atmospherePrtRT.GetRenderTarget();
        }

        public RTHandle GetAtmosphereTransmittanceLutRenderTarget() {
            return _atmosphereTransmittanceLutRT.GetRenderTarget();
        }
    }
}
```
