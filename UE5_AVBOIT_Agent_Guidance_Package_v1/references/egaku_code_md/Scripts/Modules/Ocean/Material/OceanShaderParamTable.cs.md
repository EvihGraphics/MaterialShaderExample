# Egaku source: `Scripts/Modules/Ocean/Material/OceanShaderParamTable.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using Egaku.Rendering.Pipeline.Common;
using Egaku.Rendering.Pipeline.Common.Shader;
using UnityEngine;


namespace Modules.Ocean.MaterialParamTable {
    

  
    
    [ShaderParam.ConstantBuffer(name = "OceanMaterial")]
    public class OceanShaderParamTable : ShaderParamTable{
        
        [ShaderParam.Instanced]
        [ShaderParam.ConstantBuffer(name = "PerInstance")]
        [ShaderParam.ParamProperty(exposed = false)]
        public struct MeshDesc {
            public Vector4 pageStitchingMask;
        }
        
        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        [ShaderParam.Header(title = "Ocean Terrain")]
        public struct OceanTerrainDesc {
            [ShaderParam.ParamProperty(label = "Terrain Map")]
            public Texture2D terrainMap;
            [ShaderParam.ParamProperty(label = "Height Map")]
            public Texture2D heightMap;
            [ShaderParam.ParamProperty(label = "Terrain Rect Shape")]
            public Vector2 rectShapeWS;
            [ShaderParam.ParamProperty(label = "Terrain Rect Origin")]
            public Vector2 rectPosOriginWS;
            [ShaderParam.ParamProperty(label = "Coastline Max Distance")]
            public float toCoastlineMaxDist;
            [ShaderParam.ParamProperty(label = "Slope Prediction Length")]
            public float slopePredictionLength;
            
            [ShaderParam.Range(min = 0,max = 100)]
            [ShaderParam.ParamProperty(label = "Coastline Wave Range Width")]
            public float coastlineWaveRangeWidth;
        }

        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        [ShaderParam.Header(title = "Smooth Noise Map")]
        public struct SmoothNoiseMapDesc {
            [ShaderParam.ParamProperty(label = "Smooth Noise Map")]
            public Texture2D tex;
        }

        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        [ShaderParam.Header(title = "Coastline Wave Profile")]
        public struct CoastlineWaveProfileDesc {
            [ShaderParam.ParamProperty(label = "Wave Profile Map")]
            public Texture2D profileMap;
            [ShaderParam.ParamProperty(label = "Wave Length")]
            public float waveLength;
            [ShaderParam.ParamProperty(label = "Wave Count")]
            public float waveCount;
            [ShaderParam.ParamProperty(label = "Wave Displacement Scale")]
            public Vector2 waveDisplaceScale;
            [ShaderParam.ParamProperty(label = "Wave Displacement Offset")]
            public Vector2 waveDisplaceOffset;
        }

        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        [ShaderParam.Header(title = "Coastline Wave Profile Noise")]
        public struct WaveProfileNoiseDesc {
            [ShaderParam.ParamProperty(label = "Wave Noise Time")]
            public float noiseTime;
            [ShaderParam.ParamProperty(label = "Wave Noise Scale")]
            public float noiseScale;
            [ShaderParam.ParamProperty(label = "With Details")]
            public bool withDetails;
        }

        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        [ShaderParam.Header(title = "Coastline Surface")]
        public struct CoastlineSurfaceDesc {
            [ShaderParam.ParamProperty(label = "Neighbor Vtx Mock Length")]
            public float neighborVtxMockLength;
            [ShaderParam.ParamProperty(label = "Velocity Scale")]
            public float velocityScale;
            [ShaderParam.ParamProperty(label = "Foam Scale")]
            public float foamScale;
            [ShaderParam.ParamProperty(label = "Foam Offset")]
            public float foamDistance; // distance that having foam on the water surface.
        }
        
        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        [ShaderParam.Header(title = "Distant Wave")]
        public struct DistantWaveDesc {
            // rg: normal derivative (r,g)=(n.x/n.z,n.y/n.z)
            // b: wave height
            // a: foam mask
            [ShaderParam.ParamProperty(label = "Wave Sequence Frame Map")]
            public Texture2DArray waveSeqFrameMap;
            [ShaderParam.ParamProperty(label = "Wave Sequence Frame FPS and Slice Count")]
            public Vector2 waveSeqFPSSliceCount;
            [ShaderParam.ParamProperty(label = "Wave Height Scale")]
            public float waveHeightScale;
            [ShaderParam.ParamProperty(label = "Wave Height Offset")]
            public float waveHeightOffset;
            [ShaderParam.ParamProperty(label = "Foam Scale")]
            public float foamScale;
            [ShaderParam.ParamProperty(label = "Foam Offset")]
            public float foamOffset;
            [ShaderParam.ParamProperty(label = "Wave Length Scale")]
            public Vector2 waveLengthScale; // this functions as UV scalar and affect the normal.
            [ShaderParam.ParamProperty(label = "Wind Velocity")]
            public Vector2 windVelocity;
            [ShaderParam.ParamProperty(label = "Wave Velocity Bottom")]
            public Vector2 waveVelocityBottom;  // wave velocity when wave height is at the lowest.
            [ShaderParam.ParamProperty(label = "Wave Velocity Top")]
            public Vector2 waveVelocityTop; // wave velocity when wave height is at the highest.
            [ShaderParam.ParamProperty(label = "Map Normal Decode Const")]
            public float mapNormalDecodeConst; // a constant value used to decode normal map
        }

        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        [ShaderParam.Header(title = "Water Volume Shading")]
        public struct WaterVolumetricShadingDesc {
    
            [ShaderParam.ParamProperty(label = "Clarity Far")]
            public float clarityFar;
            
            [ShaderParam.ParamProperty(label = "Clarity Near")]
            public float clarityNear;   
            
            
          //  [ShaderParam.HDR]
            [ShaderParam.ParamProperty(label = "Absorption Far")]
            public Color absorptionFar;
            
            [ShaderParam.ParamProperty(label = "Absorption Far Dist")]
            public float absorptionFarDist;
            
         //   [ShaderParam.HDR]
            [ShaderParam.ParamProperty(label = "Absorption Near")]
            public Color absorptionNear;
            
            [ShaderParam.ParamProperty(label = "Absorption Near Dist")]
            public float absorptionNearDist;
            
          //  [ShaderParam.HDR]
            [ShaderParam.ParamProperty(label = "Scattering Far")]
            public Color scatteringFar;
            
            [ShaderParam.ParamProperty(label = "Scattering Far Dist")]
            public float scatteringFarDist;
         //   [ShaderParam.HDR]
            [ShaderParam.ParamProperty(label = "Scattering Near")]
            public Color scatteringNear;
            
            [ShaderParam.ParamProperty(label = "Scattering Near Dist")]
            public float scatteringNearDist;

            [ShaderParam.ParamProperty(label = "Foam Scattering Scale")]
            public float scatteringFoamScale;

            [ShaderParam.ParamProperty(label = "SSS detail")]
            public float subscatteringDetail;

            [ShaderParam.ParamProperty(label = "SSS power")]
            public float subscatteringPower;
            
            [ShaderParam.ParamProperty(label = "SSS scale")]
            public float subscatteringScale;
            
            
            
            [ShaderParam.ParamProperty(label = "Behind Water Color Far")]
            public Color behindColorFar; 
            
            [ShaderParam.ParamProperty(label = "Behind Water Color Near")]
            public Color behindColorNear; 
 
        }

         [ShaderParam.ConstantBuffer(name = "PerMaterial")]
         [ShaderParam.Header(title = "Water Surface Shading")]
        public struct WaterSurfaceShadingDesc {
            [ShaderParam.ParamProperty(label = "Overlay Color Far")]
            public Color overlayColorFar; 
            
            [ShaderParam.ParamProperty(label = "Overlay Color Near")]
            public Color overlayColorNear;
            
            [ShaderParam.ParamProperty(label = "Direct Light Ks")]
            public float directLightSpecular;

            [ShaderParam.ParamProperty(label = "Reflection Ks")]
            public float reflectionSpecular;

            [ShaderParam.ParamProperty(label = "Foam Base Color")]
            public Color foamBaseColor;
            
            [ShaderParam.ParamProperty(label = "Foam Detail Color")]
            public Color foamDetailColor;
            
            [ShaderParam.ParamProperty(label = "Soft Foam Scale")]
            public float softFoamScale;
            
        }
        
        [ShaderParam.ConstantBuffer(name = "PerMaterial")]
        [ShaderParam.Header(title = "Water Foam Map")]
        public struct WaterFoamMapDesc {
            [ShaderParam.ParamProperty(label = "Foam Map")]
            public Texture2D foamMap;
            
            [ShaderParam.ParamProperty(label = "Foam Map UV Scale")]
            public Vector2 uvScale;

            [ShaderParam.ParamProperty(label = "Foam Map Advection Speed Scale")]
            public float advectionSpeedScale;
            
        }
    }
}
```
