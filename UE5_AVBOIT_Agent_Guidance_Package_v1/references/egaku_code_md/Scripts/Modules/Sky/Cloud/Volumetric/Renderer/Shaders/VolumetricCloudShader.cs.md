# Egaku source: `Scripts/Modules/Sky/Cloud/Volumetric/Renderer/Shaders/VolumetricCloudShader.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using ShaderResource;
using UnityEngine;
using UnityEngine.Rendering;

namespace Modules.Sky.Cloud.Volumetric.Renderer.Shaders {
    struct VolumetricCloudShader {
        private ComputeShader _rayMarchingCS;
    
        struct KernelId {
            public int voxelMetaLutBuilding;
        }
            
        KernelId _kernelId;
        private int _threadGroupsDispatchX;
        private int _threadGroupsDispatchY;
        private int _threadGroupsDispatchZ;
        private RTHandle _rayMarchingRT;
        private RTHandle _voxelMetaLut;
        private UnityEngine.Material _material;
            
        public void Build(UnityEngine.Material material) {
            _rayMarchingCS = ComputeShaderResource.GetComputeShader("Sky/Cloud/Volumetric/VolumetricCloud");
            _kernelId.voxelMetaLutBuilding = _rayMarchingCS.FindKernel("Kernel_VoxelMetaLutBuilding");
            _material = material;
        }

        public void SetDispatchGroupSize(int groupSizeX, int groupSizeY) {
          
            _rayMarchingCS.GetKernelThreadGroupSizes(_kernelId.voxelMetaLutBuilding,
                out uint threadGroupSizeX, out uint threadGroupSizeY, out uint threadGroupSizeZ);
            if (threadGroupSizeZ != 1) {
                throw new System.Exception("VolumetricCloudRenderer: Thread group size Z must be 1");
            }

            _threadGroupsDispatchX = groupSizeX;
            _threadGroupsDispatchY = groupSizeY;
            _threadGroupsDispatchZ = 1;
        }

        public void SetVoxelMetaLut(RTHandle rt) {
           _voxelMetaLut = rt;
        }
        
        public void CmdDispatch(CommandBuffer cmd){
            cmd.SetComputeParamsFromMaterial(_rayMarchingCS,_kernelId.voxelMetaLutBuilding,_material);
            cmd.SetComputeTextureParam(_rayMarchingCS,_kernelId.voxelMetaLutBuilding,"_VolumetricCloud_VoxelMetaLut",_voxelMetaLut);
            cmd.DispatchCompute(_rayMarchingCS, _kernelId.voxelMetaLutBuilding,
                _threadGroupsDispatchX, _threadGroupsDispatchY, _threadGroupsDispatchZ);
        }
    }

}
```
