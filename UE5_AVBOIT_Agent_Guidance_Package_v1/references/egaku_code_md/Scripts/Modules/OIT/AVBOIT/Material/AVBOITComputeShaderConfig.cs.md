# Egaku source: `Scripts/Modules/OIT/AVBOIT/Material/AVBOITComputeShaderConfig.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using Egaku.Rendering.Pipeline.Common.Shader;
using UnityEngine;

namespace Modules.OIT.AVBOIT.Material {

    public class AVBOITComputeShaderConfig {
        

        public class VirtualBlockBasedShaderConfig {

            public static string FileRootURL = "OIT/AVBOIT/VirtualBlockBased";
            public static string FileRootDirectory = "Shaders/OIT/AVBOIT/VirtualBlockBased";
            
            
            public class VolumeTransmittanceIntegrationCSConfig : ComputeShaderConfig {
                public override string FileURL =>
                    $"{FileRootURL}/AVBOIT.VolumeTransmittanceIntegrationPass";

                public override string ComputeShaderName => "AVBOIT.VolumeTransmittanceIntegrationPass";
                public override string FileDirectory => $"{FileRootDirectory}";

                public class VolumeTransmittanceIntegrationPassKernel : ComputeShaderKernelConfig {
                    public override string KernelSymbolName => "Kernel_RenderTransmittanceLut";

                    public override string[] IncludedFiles =>
                        new[] { "Passes/VolumeTransmittanceIntegrationPass.hlsl" };
                }


                public override ComputeShaderKernelConfig[] KernelConfigs => new ComputeShaderKernelConfig[] {
                    new VolumeTransmittanceIntegrationPassKernel()
                };

                public string GetVolumeTransmittanceIntegrationPassKernelName() {
                    return KernelConfigs[0].KernelSymbolName;
                }

                public string GetComputeShaderAssetURL() {
                    return FileURL;
                }
            }

            public class VolumeBlockLutBuildingCSConfig : ComputeShaderConfig {
                public override string FileURL => $"{FileRootURL}/AVBOIT.VolumeBlockLutBuildingPass";
                public override string ComputeShaderName => "AVBOIT.VolumeBlockLutBuildingPass";
                public override string FileDirectory =>  $"{FileRootDirectory}";

                public class LutBuildingPassKernel : ComputeShaderKernelConfig {
                    public override string KernelSymbolName => "Kernel_BuildVolumeBlockLut";
                    public override string[] IncludedFiles => new[] { "Passes/VolumeBlockLutBuildingPass.hlsl" };
                }
                
                public class ProtectionPlaneBuildingPassKernel : ComputeShaderKernelConfig {
                    public override string KernelSymbolName => "Kernel_BuildProtectionPlane";
                    public override string[] IncludedFiles => new[] { "Passes/VolumeBlockLutBuildingPass.hlsl" };
                }
                
                public class LutParallelBuilding_ScanPassKernel : ComputeShaderKernelConfig {
                    public override string KernelSymbolName => "Kernel_BuildVolumeBlockLutParallel_Scan";
                    public override string[] IncludedFiles => new[] { "Passes/VolumeBlockLutBuildingPass.hlsl" };
                }

                public class LutParallelBuilding_AuxiliaryArrayInitPassKernel : ComputeShaderKernelConfig {
                    public override string KernelSymbolName => "Kernel_BuildVolumeBlockLutParallel_InitAuxiliaryArray";
                    public override string[] IncludedFiles => new[] { "Passes/VolumeBlockLutBuildingPass.hlsl" };
                }
                
                public class LutParallelBuilding_FinalSumPassKernel : ComputeShaderKernelConfig {
                    public override string KernelSymbolName => "Kernel_BuildVolumeBlockLutParallel_FinalSum";
                    public override string[] IncludedFiles => new[] { "Passes/VolumeBlockLutBuildingPass.hlsl" };
                }
                
                


                public override ComputeShaderKernelConfig[] KernelConfigs => new ComputeShaderKernelConfig[] {
                    new LutBuildingPassKernel(),
                    new ProtectionPlaneBuildingPassKernel(),
                    new LutParallelBuilding_ScanPassKernel(),
                    new LutParallelBuilding_AuxiliaryArrayInitPassKernel(),
                    new LutParallelBuilding_FinalSumPassKernel()
                };

                public string GetLutBuildingPassKernelName() {
                    return KernelConfigs[0].KernelSymbolName;
                }
                
                public string GetProtectionPlaneBuildingKernelName() {
                    return KernelConfigs[1].KernelSymbolName;
                }
                
                public string GetLutParallelBuilding_ScanPassKernelName() {
                    return KernelConfigs[2].KernelSymbolName;
                }
                
                public string GetLutParallelBuilding_AuxiliaryArrayInitPassKernelName() {
                    return KernelConfigs[3].KernelSymbolName;
                }
                
                public string GetLutParallelBuilding_FinalSumPassKernelName() {
                    return KernelConfigs[4].KernelSymbolName;
                }
                
                public string GetComputeShaderAssetURL() {
                    return FileURL;
                }
            }

            private static ComputeShaderConfig[] _ComputeShaderConfigs = {
                ScriptableObject.CreateInstance<VolumeTransmittanceIntegrationCSConfig>(),
                ScriptableObject.CreateInstance<VolumeBlockLutBuildingCSConfig>(),
            };
            

            public VolumeTransmittanceIntegrationCSConfig GetVolumeTransmittanceIntegrationCSConfig() {
                return (VolumeTransmittanceIntegrationCSConfig)_ComputeShaderConfigs[0];
            }

            public VolumeBlockLutBuildingCSConfig GetVolumeBlockLutBuildingCSConfig() {
                return (VolumeBlockLutBuildingCSConfig)_ComputeShaderConfigs[1];
            }
        }
        
        
    }
        
}
```
