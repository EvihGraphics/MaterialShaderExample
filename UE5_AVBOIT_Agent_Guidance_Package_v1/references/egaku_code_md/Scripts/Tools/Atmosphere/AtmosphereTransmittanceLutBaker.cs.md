# Egaku source: `Scripts/Tools/Atmosphere/AtmosphereTransmittanceLutBaker.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.IO;
using Modules.Sky.Atmosphere.Material;
using UnityEngine;
using Modules.Sky.Atmosphere.Renderer;
using UnityEngine.Rendering;


namespace Tools.Atmosphere {
    
    
    [ExecuteAlways]
    public class AtmosphereTransmittanceLutBaker : MonoBehaviour{
        [Serializable]
        public class TransmittanceLutConfig {
            public int textureWidth;
            public int textureHeight;
            public int samplePerDirection;
            public string outputFileDirectory;
            public string outputFileName;
        }
        
        [SerializeField]
        private TransmittanceLutConfig _config;
        
        private AtmosphereRenderer _renderer;
        private AtmosphereShaderConfig _shaderConfig;
        private Material _material;
        private CommandBuffer _cmd;
        public void OnEnable() {
            _renderer = new();
            _shaderConfig = ScriptableObject.CreateInstance<AtmosphereShaderConfig>();
            _material = new Material(Shader.Find(_shaderConfig.ShaderURL));
            _renderer.SetMaterial(_material);
            _renderer.Init(false);
            _cmd = CommandBufferPool.Get();
        }

        byte[] GetTextureBytes(Texture2D texture, string ext) {
            byte[] bytes = null;
            if (ext == ".png") {
                bytes = texture.EncodeToPNG();
            } else if(ext == ".tga"){
                bytes = texture.EncodeToTGA();
            }else if(ext == ".jpg"){
                bytes = texture.EncodeToJPG();
            }else if (ext == ".exr") {
                bytes = texture.EncodeToEXR();
            }
            return bytes;
        }

        void SaveRenderTarget(RTHandle rtHandle) {
            if (String.IsNullOrEmpty(_config.outputFileName)) {
                return;
            }

            Vector2Int vp = rtHandle.referenceSize;
            Texture2D tex = new Texture2D(vp.x, vp.y, TextureFormat.RGBAFloat, false);
            RenderTexture.active = rtHandle.rt;
            tex.ReadPixels(new Rect(0, 0, vp.x,vp.y), 0, 0);
            tex.Apply();
            RenderTexture.active = null;
            string ext = Path.GetExtension(_config.outputFileName);
         
            Directory.CreateDirectory(_config.outputFileDirectory);
            File.WriteAllBytes(Path.Combine(_config.outputFileDirectory, _config.outputFileName), GetTextureBytes(tex, ext));
            DestroyImmediate(tex);
        }
        
        [ContextMenu("Generate Atmosphere Transmittance Lut")]
        void BakeTransmittanceLut() {
            int width = _config.textureWidth;
            int height = _config.textureHeight; 
            
            _material.SetInteger("_TransmittanceLut_SamplePerDirection",_config.samplePerDirection);
            _renderer.SetRenderTargetResolution(width, height);
            _renderer.SetupRenderTargets();
           
          
            _renderer.RecordRenderCommandsTransmittanceLutPass(_cmd);
            Graphics.ExecuteCommandBuffer(_cmd);
 
            SaveRenderTarget(_renderer.GetAtmosphereTransmittanceLutRenderTarget());
        }

        void OnDisable() {
            CommandBufferPool.Release(_cmd);
            _renderer.OnDisable();
        }
    }
}
```
