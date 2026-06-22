# Egaku source: `Scripts/Modules/Sky/Cloud/Volumetric/Renderer/VolumetricCloudRenderer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Security.Cryptography;
using Modules.Sky.Cloud.Volumetric.Renderer.RenderResources;
using Modules.Sky.Cloud.Volumetric.Renderer.Shaders;
using RenderFeatures.Cloud;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;
using Random = System.Random;

namespace Modules.Sky.Cloud.Volumetric.Renderer {
    
    [ExecuteAlways]
    public class VolumetricCloudRenderer : MonoBehaviour {
        
        [SerializeField]
        private UnityEngine.Material _material;
        
        [Serializable]
        struct RenderTargetConfig {
            public int width;
            public int height;
        }
        
        
        [Serializable]
        struct DCTShadowMapConfig {
            public int width;
            public int height;
            public int sliceCount;
        }
        
        

        [Serializable]
        struct FilterConfig {
            [Range(0,1)]
            public float temporalBlendFactor;
            [Range(0,1)]
            public float temporalBlendFactorOutView;
            [Range(0,5)]
            public int bilateralFilterKernelSize;
            [Range(0,15)]
            public float bilateralFilterSpaceSigma;
            [Range(0,3)]
            public float bilateralFilterColorSigmaR;
            [Range(0,3)]
            public float bilateralFilterColorSigmaG;
            [Range(0,3)]
            public float bilateralFilterColorSigmaB;
            [Range(0,1)]
            public float bilateralFilterColorSigmaA;
        }
        
        [SerializeField]
        private FilterConfig _filterConfig;
        

        [SerializeField]
        private DCTShadowMapConfig dctShadowMapConfig;
        
        [SerializeField]
        private RenderTargetConfig _cloudRTConfig;
        
        private DirectionOccupancyLut _directionOccupancyLut;
        private VolumetricCloudRenderTarget _cloudRT;
        private SunTransmittanceLut _sunTransmittanceLut;
        private VoxelMetaLut _voxelMetaLut;
        private TemporalRT _temporalRT;
        private FourierShadowMap _fourierShadowMap;
        private DCTShadowMap _dctShadowMap;
        private VolumetricCloudShader _shader;

        private int _octahedronMarchingPassID;
        private int _temporalFilterPassID;
        private int _fourierShadowMapBuildingPassID;
        private int _DCTShadowMapBuildingPassID;

        private bool _enabled = false;
        public bool Enabled => _enabled;
        
        private bool _firstFrame = true;
        

        private Matrix4x4 _previousViewProjection;
        private Matrix4x4 _currentViewProjection;
        private Texture2D _blueNoise;
        
        private Mesh _quadMesh;

        private int _framePingPong = 0;
        
        
        private Mesh CreateFullscreenQuad()
        {
            Mesh mesh = new Mesh();
            mesh.vertices = new Vector3[]
            {
                new(-1, 1, 0.99f), new( -1, -1, 0.99f),
                new( 1,  -1, 0.99f), new(1,  1, 0.99f)
            };
            mesh.triangles = new [] { 0, 1, 2, 0, 2, 3 };
            mesh.uv = new Vector2[] { new(0,0), new(0,1), new(1,1), new(1,0) };
            mesh.RecalculateBounds();
            return mesh;
        }
        
        public void OnEnable() {
            _quadMesh = CreateFullscreenQuad();
            _enabled = true;
            _firstFrame = true;
            _blueNoise = Resources.Load<Texture2D>("T_Noise");
            _octahedronMarchingPassID = _material.FindPass("OctahedronMarching");
            _temporalFilterPassID = _material.FindPass("TemporalFilter");
            _fourierShadowMapBuildingPassID = _material.FindPass("FourierShadowMapBuilding");
            _DCTShadowMapBuildingPassID = _material.FindPass("DCTShadowMapBuilding");
            _material.SetTexture("_BlueNoise", _blueNoise);
            _material.SetVector("_RTParams", new Vector4(_cloudRTConfig.width, _cloudRTConfig.height, 1.0f / _cloudRTConfig.width, 1.0f / _cloudRTConfig.height));
            BuildSH();
            
            _temporalRT.Setup(_cloudRTConfig.width / 2, _cloudRTConfig.height / 2);
            _fourierShadowMap.Setup(dctShadowMapConfig.width,dctShadowMapConfig.height,dctShadowMapConfig.sliceCount);
            _dctShadowMap.Setup(dctShadowMapConfig.width,dctShadowMapConfig.height,dctShadowMapConfig.sliceCount);
            
            var fsmHandles = _fourierShadowMap.GetSliceHandles();
            for (int i = 0; i < fsmHandles.Length; i++) {
                Shader.SetGlobalTexture($"_VolumetricCloud_FSM_FourierCoeffsLut{i}", fsmHandles[i]);
            }
            
            var dsmHandles = _dctShadowMap.GetSliceHandles();
            for (int i = 0; i < dsmHandles.Length; i++) {
                Shader.SetGlobalTexture($"_VolumetricCloud_DCTShadowMap{i}", dsmHandles[i]);
            }
            
            SetupRenderTargets();
            _shader.Build( _material);
            _shader.SetVoxelMetaLut(_voxelMetaLut.GetHandle());
            Shader.SetGlobalFloat("_HemiOctahedron",1);
            VolumetricCloudRenderFeature.SetRenderer(this);
        }
        
        public void OnDisable() {
            Shader.SetGlobalFloat("_HemiOctahedron",0);
            _sunTransmittanceLut.Release();
            _voxelMetaLut.Release();
            _temporalRT.Release();
            _fourierShadowMap.Release();
            _enabled = false;
        }
        
        private void Update() {
            _material.SetFloat("_TemporalBlendFactor", _filterConfig.temporalBlendFactor);
            _material.SetFloat("_TemporalBlendFactor_OutView", _filterConfig.temporalBlendFactorOutView);
            Shader.SetGlobalFloat("_BilateralFilter_KernelSize", _filterConfig.bilateralFilterKernelSize);
            Shader.SetGlobalVector("_BilateralFilter_ColorSigma", new Vector4(_filterConfig.bilateralFilterColorSigmaR,
                _filterConfig.bilateralFilterColorSigmaG, 
                _filterConfig.bilateralFilterColorSigmaB, 
                _filterConfig.bilateralFilterColorSigmaA));
            Shader.SetGlobalFloat("_BilateralFilter_SpaceSigma", _filterConfig.bilateralFilterSpaceSigma);
        }
        
        void BuildSH() {
            Vector3 pos = transform.position;
            SphericalHarmonicsL2 sh;
            LightProbes.GetInterpolatedProbe(pos, null, out sh);

            _material.SetVector("_EnvLight_SHAr",new Vector4(sh[0,3],sh[0,1],sh[0,2],sh[0,0]));
            _material.SetVector("_EnvLight_SHBr",new Vector4(sh[0,4],sh[0,5],sh[0,6],sh[0,7]));
        
            _material.SetVector("_EnvLight_SHAg",new Vector4(sh[1,3],sh[1,1],sh[1,2],sh[1,0]));
            _material.SetVector("_EnvLight_SHBg",new Vector4(sh[1,4],sh[1,5],sh[1,6],sh[1,7]));
            
            _material.SetVector("_EnvLight_SHAb",new Vector4(sh[2,3],sh[2,1],sh[2,2],sh[2,0]));
            _material.SetVector("_EnvLight_SHBb",new Vector4(sh[2,4],sh[2,5],sh[2,6],sh[2,7]));
            
            _material.SetVector("_EnvLight_SHC",new Vector4(sh[0,8],sh[1,8],sh[2,8],1));
        }

        
        

        public void SetupRenderTargets() {
            _cloudRT.Setup(_cloudRTConfig.width, _cloudRTConfig.height, true);
            _material.SetTexture("_CurrentFrameTex", _cloudRT.GetCloudShadingRT());
        }

        public void SetRenderTargetHandles() {
            _cloudRT.SwapTemporalFilterBuffer();
            if (_firstFrame) {
                _material.SetTexture("_HistoryFrameTex", _cloudRT.GetCloudShadingRT());
            } else {
                _material.SetTexture("_HistoryFrameTex", _cloudRT.GetTemporalFilterHistoryBuffer());
            }
        }
        
    
     
        
        public void RecordRenderCommandsFourierShadowMapBuildingPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("Volumetric Cloud - Fourier Shadow Map Building");

            CoreUtils.SetRenderTarget(cmd, _fourierShadowMap.GetSliceTargets(),_fourierShadowMap.GetSliceHandles()[0].rt.depthBuffer,ClearFlag.Color,Color.clear);
            cmd.DrawMesh(_quadMesh, Matrix4x4.identity,_material,0,_fourierShadowMapBuildingPassID);
      //      context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        public void RecordRenderCommandsDCTShadowMapBuildingPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("Volumetric Cloud - DCT Shadow Map Building");
            _previousViewProjection = _currentViewProjection;
            _currentViewProjection = data.cameraData.camera.projectionMatrix * data.cameraData.camera.worldToCameraMatrix;
            cmd.SetGlobalMatrix("_ViewProjectionMatrix", _currentViewProjection);
            cmd.SetGlobalMatrix("_PreviousViewProjectionMatrix", _previousViewProjection);
            
            CoreUtils.SetRenderTarget(cmd, _dctShadowMap.GetSliceTargets(),_dctShadowMap.GetSliceHandles()[0].rt.depthBuffer,ClearFlag.None);
            cmd.DrawMesh(_quadMesh, Matrix4x4.identity,_material,0,_DCTShadowMapBuildingPassID);
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
       
        public void RecordRenderCommandsOctahedronMarchingPass_ComputeImpl(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("Volumetric Cloud - Octahedron Marching CS");
            
            _previousViewProjection = _currentViewProjection;
            _currentViewProjection = data.cameraData.camera.projectionMatrix * data.cameraData.camera.worldToCameraMatrix;
            Shader.SetGlobalMatrix("_ViewProjectionMatrix", _currentViewProjection);
            Shader.SetGlobalMatrix("_PreviousViewProjectionMatrix", _previousViewProjection);
            _shader.CmdDispatch(cmd);
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
        public void RecordRenderCommandsOctahedronMarchingPass_RasterImpl(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("Volumetric Cloud - Octahedron Marching");
            if (_firstFrame) {
               // cmd.Blit(null, _cloudRT.GetTemporalFilterHistoryBuffer(), _material,_octahedronMarchingPassID);
                _firstFrame = false;
            }
            cmd.Blit(null, _cloudRT.GetCloudShadingRT(), _material,_octahedronMarchingPassID);
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
        
                
        public void RecordRenderCommandsOctahedronMarchingPass(ref ScriptableRenderContext context, ref RenderingData data) {
            RecordRenderCommandsOctahedronMarchingPass_RasterImpl(ref context,ref data);
        }
        
        public void RecordRenderCommandsFilterPass(ref ScriptableRenderContext context, ref RenderingData data) {
            CommandBuffer cmd = CommandBufferPool.Get("Volumetric Cloud - Filter Pass");
           // cmd.SetGlobalTexture("_BilateralFilteredCloud", _cloudRT.GetFinalResultRT());
            cmd.SetGlobalTexture("_Cloud", _cloudRT.GetFinalResultRT());
            cmd.Blit(null, _cloudRT.GetTemporalFilterCurrentRT(), _material,_temporalFilterPassID);
            /*cmd.SetGlobalTexture("_TemporalFilteredCloud", _cloudRT.GetTemporalFilterCurrentRT());*/
            cmd.Blit(_cloudRT.GetTemporalFilterCurrentRT(),_cloudRT.GetFinalResultRT());
            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }

        public RTHandle GetFinalCloudRenderTarget() {
            return _cloudRT.GetFinalResultRT();
        }
    }
}
```
