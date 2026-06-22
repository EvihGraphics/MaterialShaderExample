# Egaku source: `Scripts/Modules/CBT/Renderer/CBTMeshRenderer.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using Common.Rendering;
using Modules.CBT.Renderer.RenderResources;
using Modules.CBT.Renderer.Shaders;
using UnityEngine;
using UnityEngine.Rendering;


namespace Modules.CBT.Renderer {
    
    [ExecuteAlways]
    public class CBTMeshRenderer : CustomRenderer {
        private CBTMeshRenderInstance _renderInstance;
        private Mesh _meshlet;

        // shaders
        private CBTMeshRenderSettings _renderSettings;
        private CBTSumReductionShader _sumReductionShader;
        private CBTUpdateShader _updateShader;
        private CBTIndirectShader _indirectShader;
        private CBTEvalShader _evalShader;
        
        // resources
        private CBTBuffer _cbtBuffer;
        private CBTEvalResultBuffer _cbtEvalResultBuffer;
        private CBTEvalResultMetaBuffer _cbtEvalResultMetaBuffer;
        private CBTUpdateContextCBuffer _cbtUpdateContextCBuffer;
        private CBTEvalContextCBuffer _cbtEvalContextCBuffer;
        private CBTUpdateIndirectBuffer _cbtUpdateIndirectBuffer;
        private CBTEvalIndirectBuffer _cbtEvalIndirectBuffer;
        private CBTMeshDrawIndirectBuffer _cbtMeshDrawIndirectBuffer;
        private MaterialPropertyBlock[] _materialPropertyBlocks;
        
        private int _cbtMaxNodeCount;
        private int _pingPong = 0;
        private int _drawCallCount;

        private readonly CommandBuffer[] _cbtUpdateCommandBuffer = new CommandBuffer[2];
        private readonly CommandBuffer[] _cbtEvalCommandBuffer = new CommandBuffer[2];
        private CommandBuffer _cbtRenderMeshCommandBuffer;
        
        // indirect draw parameters
        const uint MaxInstancePerDraw = 32768;
        
    
        private bool _enabled = false;
        private bool _loaded = false;
        
        public override bool Enabled => _enabled;
        public override bool Valid => _loaded;

        public void SetRenderSettings(in CBTMeshRenderSettings renderSettings) {
            _renderSettings = renderSettings;
        }

        public void SetRenderInstance(CBTMeshRenderInstance renderInstance) {
            _renderInstance = renderInstance;
            if (_renderInstance.Enabled && _renderInstance.Valid && Valid) {
                BuildCBTRenderMeshCommandBuffer();
            }
        }

        private void LoadResource() {
            try {
                BuildMeshlet();
                SetupResources();
                InitShaders();
                SetShaderVariables();
                BuildMaterialPropertyBlocks();
                BuildCBTEvalCommandBuffer();
                BuildCBTUpdateCommandBuffer();
                BuildCBTUpdateCommandBuffer();
                _loaded = true;
            }catch (Exception e) {
                Debug.LogError(e);
                _loaded = false;
            }
        }

        public override bool Initialize() {
            LoadResource();
            return _loaded;
        }
        
        private void SetupResources() {
            _cbtBuffer.Setup(_renderSettings.cbtMaxDepth);
            int cbtMaxNodeCount = CBTUtils.GetCBTLevelNodeCount(_renderSettings.cbtMaxDepth);
            _cbtEvalResultBuffer.Setup(cbtMaxNodeCount);
            _cbtEvalResultMetaBuffer.Setup();
            _cbtUpdateContextCBuffer.Setup();
            _cbtEvalContextCBuffer.Setup();
            _cbtUpdateIndirectBuffer.Setup();
            _cbtEvalIndirectBuffer.Setup();
            _cbtMeshDrawIndirectBuffer.Setup(2,_meshlet.GetIndexCount(0));
            _cbtUpdateIndirectBuffer.Setup();
        }
        
        

        public void SetEnabled() {
            _enabled = true;
        }

        private void OnEnable() {
         //   Initialize();
          //  _cbtMaxNodeCount = CBTUtils.GetCBTLevelNodeCount(_renderSettings.cbtMaxDepth);
         //   InitShaders();
        //    BuildMeshlet();
         //   BuildCBTBuffer();
            //BuildCBTUpdateContextCBuffer();
          //  BuildCBTUpdateIndirectBuffer();
         //   BuildCBTEvalResultBuffer();
         //   BuildCBTEvalResultMetaBuffer();
        //    BuildCBTEvalContextCBuffer();
       //     BuildCBTEvalIndirectBuffer();
          //  BuildDrawIndirectBuffer(2, _meshlet.GetIndexCount(0));
       //     SetShaderVariables();
        //    BuildMaterialPropertyBlocks();
         //   BuildCBTEvalCommandBuffer();
         //   BuildCBTRenderMeshCommandBuffer();
        //    BuildCBTUpdateCommandBuffer();
         //   BuildSH();
         //   _enabled = true;

        //    OceanRenderFeature.SetCBTMeshRenderer(this);
        }
        
        
        
        void InitShaders() {
            _sumReductionShader.Init();
            _updateShader.Init();
            _indirectShader.Init();
            _evalShader.Init();
        }


        void SetShaderVariables() {
            _sumReductionShader.SetCBTBuffer(_cbtBuffer.GetBuffer());
           // _updateShader.SetContextConstants(_cbtUpdateContextCBuffer);
            _updateShader.SetCBTBuffer(_cbtBuffer.GetBuffer());
            _updateShader.SetCBTEvalResultBuffer(_cbtEvalResultBuffer.GetBuffer());
            
            Shader.SetGlobalBuffer("_CBTBuffer",_cbtBuffer.GetBuffer());
            Shader.SetGlobalBuffer("_CBTEvalResultBuffer",_cbtEvalResultBuffer.GetBuffer());
            _indirectShader.SetCBTBuffer(_cbtBuffer.GetBuffer());
            _indirectShader.SetCBTEvalResultBuffer(_cbtEvalResultBuffer.GetBuffer());
            _indirectShader.SetCBTUpdateDispatchIndirectBuffer(_cbtUpdateIndirectBuffer.GetBuffer());
            _indirectShader.SetCBTEvalDispatchIndirectBuffer(_cbtEvalIndirectBuffer.GetBuffer());
            _indirectShader.SetDrawIndirectBuffer(_cbtMeshDrawIndirectBuffer.GetBuffer());
            _indirectShader.SetCBTEvalResultMetaBuffer(_cbtEvalResultMetaBuffer.GetBuffer());
            
            _evalShader.SetCBTEvalResultBuffer(_cbtEvalResultBuffer.GetBuffer());
            _evalShader.SetCBTEvalResultMetaBuffer(_cbtEvalResultMetaBuffer.GetBuffer());
            _evalShader.SetCBTEvalContextCBuffer(_cbtEvalContextCBuffer.GetBuffer());
            _evalShader.SetCBTBuffer(_cbtBuffer.GetBuffer());
        }

        void BuildCBTEvalCommandBuffer() {
            _cbtEvalCommandBuffer[0] = CommandBufferPool.Get("CBT Eval Pass");
            _cbtEvalCommandBuffer[1] = CommandBufferPool.Get("CBT Eval Pass");
            _evalShader.CmdDispatchEvalSplitPassIndirect(_cbtEvalCommandBuffer[0],_cbtEvalIndirectBuffer.GetBuffer());
            _evalShader.CmdDispatchEvalMergePassIndirect(_cbtEvalCommandBuffer[1],_cbtEvalIndirectBuffer.GetBuffer());
            _indirectShader.CmdDispatch(_cbtEvalCommandBuffer[0]);
            _indirectShader.CmdDispatch(_cbtEvalCommandBuffer[1]);
        }
        
        void BuildCBTUpdateCommandBuffer() {
            _cbtUpdateCommandBuffer[0] = CommandBufferPool.Get("CBT Update Pass");
            _cbtUpdateCommandBuffer[1] = CommandBufferPool.Get("CBT Update Pass");
            
            _updateShader.CmdDispatchSplitPassIndirect(_cbtUpdateCommandBuffer[0],_cbtUpdateIndirectBuffer.GetBuffer());
            _updateShader.CmdDispatchMergePassIndirect(_cbtUpdateCommandBuffer[1],_cbtUpdateIndirectBuffer.GetBuffer());

            int depth = _renderSettings.cbtMaxDepth;
            for (int i = depth - 1; i >= 0;i--) {
                int nodeCount = CBTUtils.GetCBTLevelNodeCount(i);
                _sumReductionShader.CmdSetCBTNodeDepth(_cbtUpdateCommandBuffer[0],i);
                _sumReductionShader.CmdDispatch(_cbtUpdateCommandBuffer[0],nodeCount);
                _sumReductionShader.CmdSetCBTNodeDepth(_cbtUpdateCommandBuffer[1],i);
                _sumReductionShader.CmdDispatch(_cbtUpdateCommandBuffer[1],nodeCount);
            }
        }
        
        void  BuildCBTRenderMeshCommandBuffer(){
            _cbtRenderMeshCommandBuffer = CommandBufferPool.Get("CBT Procedural Mesh Render");
            _cbtRenderMeshCommandBuffer.DrawMeshInstancedIndirect(_meshlet,0,_renderInstance.GetMaterial(),0,_cbtMeshDrawIndirectBuffer.GetBuffer());
        }
        

        void BuildMaterialPropertyBlocks() {
            _materialPropertyBlocks = new MaterialPropertyBlock[_drawCallCount];
            for (int i = 0; i < _drawCallCount; i++) {
                _materialPropertyBlocks[i] = new MaterialPropertyBlock();
            }
            for (int i = 0; i < _drawCallCount; i++) {
                _materialPropertyBlocks[i].SetInteger("_InstanceIDOffset",(int)(i * MaxInstancePerDraw));
            }
        }
        

        void BuildMeshlet() {
            // build meshlet for mesh instancing.
            if (_renderSettings.meshletSubdivisionLevel < 8) {
                _meshlet = CBTUtils.BuildMeshletUInt16(_renderSettings.meshletSubdivisionLevel);
            } else {
                _meshlet = CBTUtils.BuildMeshletUInt32(_renderSettings.meshletSubdivisionLevel);
            }
        }

        
        
        public void RecordRenderMeshCommands(ref ScriptableRenderContext context, Camera cam) {
            if (_cbtRenderMeshCommandBuffer!=null) {
                context.ExecuteCommandBuffer(_cbtRenderMeshCommandBuffer);
            }
        }

        Vector4 GetPlaneVector(in Plane plane) {
            return new(plane.normal.x, plane.normal.y, plane.normal.z, plane.distance);
        }
        
        Vector4[] GetViewSpaceFrustumPlanes(Camera cam) {
            Matrix4x4 projectionMatrix = cam.projectionMatrix;
            Vector4[] planes = new Vector4[6];
            planes[0] = projectionMatrix.GetRow(3) - projectionMatrix.GetRow(0);
            planes[1] = projectionMatrix.GetRow(3) + projectionMatrix.GetRow(0);
            planes[2] = projectionMatrix.GetRow(3) - projectionMatrix.GetRow(1);
            planes[3] = projectionMatrix.GetRow(3) + projectionMatrix.GetRow(1);
            planes[4] = projectionMatrix.GetRow(3) - projectionMatrix.GetRow(2);
            planes[5] = projectionMatrix.GetRow(2);
            return planes;
        }
        
        private void UpdateCBTEvalContextCBuffer(Camera cam,CBTMeshRenderInstance renderInstance) {
            float fovy = cam.fieldOfView;
            Vector4[] planes = GetViewSpaceFrustumPlanes(cam);
            var evalContext = _cbtEvalContextCBuffer.GetBuffer().LockBufferForWrite<CBTEvalContextConstants>(0, 1);
            evalContext[0] = new() {
                model = renderInstance.GetTransform().localToWorldMatrix,
                modelView = cam.worldToCameraMatrix *  renderInstance.GetTransform().localToWorldMatrix,
                frustumPlanes0 = planes[0],
                frustumPlanes1 = planes[1],
                frustumPlanes2 = planes[2],
                frustumPlanes3 = planes[3],
                frustumPlanes4 = planes[4],
                frustumPlanes5 = planes[5],
                targetLod = new Vector4(CBTUtils.GetLodBaseFactor(_renderSettings.targetPrimitiveViewportPixelCount, _renderSettings.meshletSubdivisionLevel, 
                    fovy, cam.pixelHeight),0,0,0)
            };
            _cbtEvalContextCBuffer.GetBuffer().UnlockBufferAfterWrite<CBTEvalContextConstants>(1);
        }
        
        public void RecordCBTEvalComputeCommands(ref ScriptableRenderContext context, Camera cam) {
            if (_renderInstance!=null && _renderInstance.Enabled && _renderInstance.Valid) {
                UpdateCBTEvalContextCBuffer(cam,_renderInstance);
            }
            context.ExecuteCommandBuffer(_cbtEvalCommandBuffer[_pingPong]);
        }

        
        public void RecordCBTUpdateComputeCommands(ref ScriptableRenderContext context, Camera cam) {
            context.ExecuteCommandBuffer(_cbtUpdateCommandBuffer[_pingPong]);
        }

        public void UpdatePingPong() {
            _pingPong = 1 - _pingPong;
        }
        

        public void SetDisabled() {
            _enabled = false;
        }

        public override bool Destroy() {
            ReleaseCommandBuffers();
            DestroyBuffers();
            return true;
        }


        private void ReleaseCommandBuffers(){
            if (_cbtRenderMeshCommandBuffer != null) {
                CommandBufferPool.Release(_cbtRenderMeshCommandBuffer);
            }
            if (_cbtUpdateCommandBuffer[0] != null) {
                CommandBufferPool.Release(_cbtUpdateCommandBuffer[0]);
            }
            if (_cbtUpdateCommandBuffer[1] != null) {
                CommandBufferPool.Release(_cbtUpdateCommandBuffer[1]);
            }
            if (_cbtEvalCommandBuffer[0] != null) {
                CommandBufferPool.Release(_cbtEvalCommandBuffer[0]);
            }
            if (_cbtEvalCommandBuffer[1] != null) {
                CommandBufferPool.Release(_cbtEvalCommandBuffer[1]);
            }
        }
        
        void DestroyBuffers() {
            _cbtBuffer.Release();
            _cbtUpdateContextCBuffer.Release();
            _cbtEvalContextCBuffer.Release();
            _cbtEvalResultBuffer.Release();
            _cbtEvalResultMetaBuffer.Release();
            _cbtUpdateIndirectBuffer.Release();
            _cbtEvalIndirectBuffer.Release();
            _cbtMeshDrawIndirectBuffer.Release();
        }
    }
}
```
