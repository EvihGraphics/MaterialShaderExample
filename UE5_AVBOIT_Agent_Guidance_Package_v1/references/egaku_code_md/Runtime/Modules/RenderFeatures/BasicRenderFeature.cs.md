# Egaku source: `Runtime/Modules/RenderFeatures/BasicRenderFeature.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Collections.Generic;
using System.Resources;
using Egaku.Rendering.Pipeline.Common;
using Unity.Collections;
using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Runtime {
    
    public class BasicRenderFeature : RenderFeature{
        
        
        private class TestPassNode : RenderPassNode {
            
            private Shader _shader = Shader.Find("Egaku Render Pipeline/Blit");
            Material _mat;
            
            public override void Execute() {
                RenderContext context = ResourceManager.GetCurrentRenderContext();
                RenderCommand.SetupCurrentCameraProperties(context);
                context.SubmitCommands();
                RenderTextureDescriptor colorRTDesc = new RenderTextureDescriptor(context.Camera.PixelWidth, context.Camera.PixelHeight);
                colorRTDesc.colorFormat = RenderTextureFormat.ARGB32;
                colorRTDesc.depthBufferBits = 24;
                colorRTDesc.sRGB = (QualitySettings.activeColorSpace == ColorSpace.Linear);
                colorRTDesc.msaaSamples = 1;
                colorRTDesc.enableRandomWrite = false;
                RenderTexture renderTexture = new RenderTexture(colorRTDesc);
                renderTexture.name = "BasicRenderFeature_ColorRenderTexture";
                renderTexture.Create();
                RenderTargetIdentifier colorRT = new RenderTargetIdentifier(renderTexture);
                CommandBuffer cmdBuffer = CommandBufferPool.Get("DrawSkyBoxPassNode");
              //  cmdBuffer.SetRenderTarget(colorRT);
              //  RenderCommand.InjectCommands(context,cmdBuffer);
                
         
                //renderTargetDesc.depthDesc.ConfigureClear(Color.black);
                //renderTargetDesc.depthDesc.loadAction = RenderBufferLoadAction.DontCare;
                RenderTargetDesc renderTargetDesc = new RenderTargetDesc {
                    colorDesc = new AttachmentDescriptor(RenderTextureFormat.ARGB32),
                    depthDesc = new AttachmentDescriptor(RenderTextureFormat.Depth)
                };
                renderTargetDesc.color = renderTexture;
                renderTargetDesc.colorDesc.ConfigureTarget(colorRT,false,false);
                RenderPassDesc renderPassDesc = new RenderPassDesc{
                    width = context.Camera.PixelWidth,
                    height = context.Camera.PixelHeight,
                    msaaSamples = 1
                };
                
                const int depthIndex = 0, albedoIndex = 1;
                var attachments = new NativeArray<AttachmentDescriptor>(2, Allocator.Temp);
                
                RenderCommand.InjectCommandsAndClear(context,cmdBuffer);
                cmdBuffer = CommandBufferPool.Get("ClearAllPassNode");
             
                using (RenderCommand.CreateCommandScope(context,cmdBuffer,new ProfilingSampler("Prof_ClearAllPassNode"))) {
                    renderTargetDesc.colorDesc.ConfigureClear(Color.blue);
                    renderTargetDesc.depthDesc.ConfigureClear(Color.clear);
                    attachments[depthIndex] = renderTargetDesc.depthDesc;
                    attachments[albedoIndex] = renderTargetDesc.colorDesc;
                    using (attachments)
                    using (RenderCommand.CreateRenderPassScope(context, renderPassDesc.width, renderPassDesc.height,
                               renderPassDesc.msaaSamples, attachments, depthIndex)) {
                        var colors = new NativeArray<int>(1, Allocator.Temp);
                        colors[0] = albedoIndex;
                        using (colors)
                        using (RenderCommand.CreateSubPassScope(context, colors)) {
                        }
                    }
                }
                cmdBuffer.Release();
                
                
                cmdBuffer = CommandBufferPool.Get("DrawSkyBoxPassNode");      
          
                using (RenderCommand.CreateCommandScope(context,cmdBuffer,new ProfilingSampler("Prof_DrawSkyBoxPassNode"))) {
                  //  renderTargetDesc.colorDesc.loadAction = RenderBufferLoadAction.Load;
            //        renderTargetDesc.depthDesc.ConfigureClear(Color.clear);
                    attachments = new NativeArray<AttachmentDescriptor>(2, Allocator.Temp);
                    attachments[depthIndex] = renderTargetDesc.depthDesc;
                    attachments[albedoIndex] = renderTargetDesc.colorDesc;
                    using (attachments) 
                    using (RenderCommand.CreateRenderPassScope(context, renderPassDesc.width, renderPassDesc.height,
                               renderPassDesc.msaaSamples, attachments, depthIndex)) {
                        var colors = new NativeArray<int>(1, Allocator.Temp);
                        colors[0] = albedoIndex;
                        using (colors)
                        using (RenderCommand.CreateSubPassScope(context, colors)) {
                            //     context.Camera.InternalCamera.targetTexture = renderTargetDesc.color;
                            RendererList list =
                                context.InternalRenderContext.CreateSkyboxRendererList(context.Camera.InternalCamera);
                            cmdBuffer.DrawRendererList(list); 
                            RenderCommand.InjectCommandsAndClear(context,cmdBuffer);
                            //   RenderCommand.DrawSkybox(context);
                        }
                    }
                }
                
                //   cmdBuffer.Clear();
                cmdBuffer.Release();
                
                /*using (RenderCommand.CreateCommandScope(context, cmdBuffer,new ProfilingSampler("Prof_DrawSkyBoxPassNode"))) {
                    RendererList list =
                        context.InternalRenderContext.CreateSkyboxRendererList(context.Camera.InternalCamera);
                    cmdBuffer.DrawRendererList(list); 
                }
                context.SubmitCommands();
                cmdBuffer.Release();*/
                
                cmdBuffer = CommandBufferPool.Get("SolidColorPostProcessNode");
  
                if (_mat is null) {
                    _mat = new Material(_shader);
                    _mat.SetColor("_Color",new Color(0.2f,0.05f,0.2f,1.0f));
                }
                
                using (RenderCommand.CreateCommandScope(context, cmdBuffer,new ProfilingSampler("SolidColorPostProcessNode"))) {
             //     cmdBuffer.Blit(context.Camera.BackBuffer,renderTexture);
                    cmdBuffer.Blit(renderTargetDesc.color,context.Camera.BackBuffer, _mat);
                    RenderCommand.InjectCommandsAndClear(context,cmdBuffer);
                }
                context.SubmitCommands();
                cmdBuffer.Release();
                
            }
        }
        
        
        public class RenderContextInitPassNode : RenderPassNode {
            
            [RenderPassOutput]
            public ResourcePipe<RenderContext>.SendPort renderContext;

            //[RenderPassOutput] 
           // public ResourcePipe<RenderTargetHandle>.SendPort backBuffer;
            
            [RenderPassOutput]
            public ResourcePipe<Color>.SendPort solidColor;
            
            [RenderPassOutput]
            public ResourcePipe<RenderTargetDesc>.SendPort renderTargetDesc;
            
            
            [RenderPassOutput]
            public ResourcePipe<RenderPassDesc>.SendPort renderPassDesc;
            
            

            public Color[] colors = new[] { Color.blue, Color.red, Color.green };

            private int _frameCount = 0;
            private RenderTexture _renderTexture;
            private RenderTexture _depthTexture;
            private RenderContext _context;
            

            public override void Setup() {
                _context = ResourceManager.GetCurrentRenderContext();
  
                RenderTextureDescriptor colorRTDesc = new RenderTextureDescriptor(_context.Camera.PixelWidth, _context.Camera.PixelHeight);
                colorRTDesc.colorFormat = RenderTextureFormat.ARGB32;
                colorRTDesc.depthBufferBits = 24;
                colorRTDesc.sRGB = (QualitySettings.activeColorSpace == ColorSpace.Linear);
                colorRTDesc.msaaSamples = 1;
                colorRTDesc.enableRandomWrite = false;
                
                RenderTextureDescriptor depthRTDesc = new RenderTextureDescriptor(_context.Camera.PixelWidth, _context.Camera.PixelHeight);
                depthRTDesc.colorFormat = RenderTextureFormat.Depth;
                depthRTDesc.depthBufferBits = 24;
                depthRTDesc.msaaSamples = 1;
                depthRTDesc.enableRandomWrite = false;
                
                if (_renderTexture is null) {
                    _renderTexture = new RenderTexture(colorRTDesc);
                    _renderTexture.name = "BasicRenderFeature_ColorRenderTexture";
                    _renderTexture.Create();
                }

                if (_depthTexture is null) {
                    _depthTexture = new RenderTexture(depthRTDesc);
                    _depthTexture.name = "BasicRenderFeature_DepthRenderTexture";
                    _depthTexture.Create();
                }


            //    CommandBuffer cmdBuffer = CommandBufferPool.Get("DrawSkyBoxPassNode");
                
                /*RenderContext _context = ResourceManager.CurrentRenderContext;*/
                Camera camera = _context.Camera.InternalCamera;
                int width = camera.scaledPixelWidth;
                int height = camera.scaledPixelHeight;
                int aa = 1;
                //RenderCommand.SetupCurrentCameraProperties(_context);
               // _context.SubmitCommands();
              //  RenderTargetIdentifier target = BuiltinRenderTextureType.CameraTarget;
                
                /*// test switch for condition that rendering unity scene view
                if (camera.targetTexture is not null) {
                    width = camera.targetTexture.width;
                    height = camera.targetTexture.height;
                    target = camera.targetTexture;
                    aa = camera.targetTexture.antiAliasing;
                }*/
                
             //   RTHandle sdl = RTHandles.Alloc(colorRT);
               
                /*
                var colorDesc = new AttachmentDescriptor(RenderTextureFormat.ARGB32);
                var depthDesc = new AttachmentDescriptor(RenderTextureFormat.Depth);*/
             //   CommandBuffer cmdBuffer = CommandBufferPool.Get();
           //     cmdBuffer.SetRenderTarget(colorRT);
            //    RenderCommand.InjectCommands(_context,cmdBuffer);
            //    cmdBuffer.Release();
            
            //    _context.Camera.InternalCamera.targetTexture = _renderTexture;
             //   depthDesc.ConfigureTarget(BuiltinRenderTextureType.Depth, false, false);
                /*var targetDesc = new RenderTargetDesc {
                    color = _renderTexture,
                    colorDesc = colorDesc,
                    depthDesc = depthDesc
                };
                var passDesc = new RenderPassDesc {
                    width = width,
                    height = height,
                    msaaSamples = aa
                };*/

            }
            
            
            
            public override void Execute() {
                RenderPassDesc renderPassDsc = new RenderPassDesc{
                    width = _context.Camera.PixelWidth,
                    height = _context.Camera.PixelHeight,
                    msaaSamples = 1
                };
                RenderTargetDesc renderTargetDsc = new RenderTargetDesc {
                    colorDesc = new AttachmentDescriptor(RenderTextureFormat.ARGB32),
                    depthDesc = new AttachmentDescriptor(RenderTextureFormat.Depth)
                };
                RenderTargetIdentifier colorRT = new RenderTargetIdentifier(_renderTexture);
                RenderTargetIdentifier depthRT = new RenderTargetIdentifier(_depthTexture);
                renderTargetDsc.color = _renderTexture;
                renderTargetDsc.colorDesc.ConfigureTarget(colorRT,false,false);
                renderTargetDsc.depth = _depthTexture;
                renderTargetDsc.depthDesc.ConfigureTarget(depthRT,false,false);

                RenderCommand.SetupCurrentCameraProperties(_context);
                _context.SubmitCommands();
                _frameCount++;
                
                renderContext.Send(_context);
                renderPassDesc.Send(renderPassDsc);
                renderTargetDesc.Send(renderTargetDsc);
                solidColor.Send(colors[(_frameCount)%3]);
            }
            
            
        }

        public class ClearDepthColorPassNode : RenderPassNode<ClearDepthColorPass> {
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderContext>.RecvPort renderContext;
            
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderTargetDesc>.RecvPort renderTargetDesc;
            
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderPassDesc>.RecvPort renderPassDesc;

            [RenderPassOutput]
            public ResourcePipe<RenderTargetDesc>.SendPort renderTargetDesc2;
            
            public ClearDepthColorPassNode() 
                : base(nameof(ClearDepthColorPassNode),$"{nameof(ClearDepthColorPassNode)}@{nameof(BasicRenderFeature)}") {
            }

            RenderContext _context;
            
            public override void Setup() {
                _context = renderContext.Receive();
                ThisPass.renderTargetDesc = renderTargetDesc.Receive();
                ThisPass.renderPassDesc = renderPassDesc.Receive();
            }

            public override void Execute() {
                using (RenderCommand.CreateCommandScope(_context,ThisCmdBuffer, ThisProfSampler)) { 
                    ThisPass.Execute(_context);
                }
                _context.SubmitCommands();
                renderTargetDesc2.Send(ThisPass.renderTargetDesc);
            }
            
        }
        
        public class DrawSkyBoxPassNode : RenderPassNode<DrawSkyBoxPass> {
            
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderContext>.RecvPort renderContext;
                        
           // [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderTargetDesc>.RecvPort renderTargetDesc;
            
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderPassDesc>.RecvPort renderPassDesc;
            
            [RenderPassInput(typeof(ClearDepthColorPassNode))]
            public ResourcePipe<RenderTargetDesc>.RecvPort renderTargetDesc2;

            public DrawSkyBoxPassNode() 
                : base(name:nameof(DrawSkyBoxPassNode),scopeTag:$"{nameof(DrawSkyBoxPassNode)}@{nameof(BasicRenderFeature)}") {
            }
            
            private RenderContext _context;
            
            public override void Setup() {
                _context = renderContext.Receive();
                ThisPass.renderTargetDesc = renderTargetDesc.Receive();
                ThisPass.renderPassDesc = renderPassDesc.Receive();
            }
            
            public override void Execute() {
             
                //  Debug.Log($"Context received by {nameof(DrawSkyBoxPassNode)}.");
              //  var rtDesc = renderTargetDesc.Receive();
              
              //  rtDesc.depthDesc.loadAction = RenderBufferLoadAction.DontCare;
           //     rtDesc.depthDesc.ConfigureTarget(BuiltinRenderTextureType.CameraTarget, false, false);
     
                ThisPass.cmdBuffer = ThisCmdBuffer;
                using (RenderCommand.CreateCommandScope(_context, ThisCmdBuffer, ThisProfSampler)) {
                    ThisPass.Execute(_context);
                }
                _context.SubmitCommands();
            }
        
        }
        
        public class DrawOpaquePassNode : RenderPassNode<DrawOpaquePass> {
            
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderContext>.RecvPort renderContext;

            [RenderPassOutput]
            public ResourcePipe<RenderTexture>.SendPort renderTarget;
            
            private RenderContext _context;

            public override void Setup() {
                _context = renderContext.Receive();
            }

            public override void Execute() {
                ThisPass.Execute(_context);
                _context.SubmitCommands();
                renderTarget.Send(_context.Camera.InternalCamera.activeTexture);
            }
        }
        
        public class SolidColorPostProcessPassNode : RenderPassNode<SolidColorPostProcessPass> {
            
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderContext>.RecvPort renderContext;

            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<Color>.RecvPort solidColor;
            
                                   
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderTargetDesc>.RecvPort renderTargetDesc;
            
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderPassDesc>.RecvPort renderPassDesc;

            public SolidColorPostProcessPassNode () 
                : base(name:nameof(SolidColorPostProcessPassNode),scopeTag:$"{nameof(SolidColorPostProcessPassNode)}@{nameof(BasicRenderFeature)}") {
            }
            
            private RenderContext _context;
            
            public override void Setup() {
                _context = renderContext.Receive();
                ThisPass.SolidColor = solidColor.Receive();
                ThisPass.SolidColor = new Color(0.1f,0.1f,0.0f,1.0f);
                ThisPass.renderTargetDesc = renderTargetDesc.Receive();
                ThisPass.renderPassDesc = renderPassDesc.Receive();
                ThisPass.cmdBuffer = ThisCmdBuffer;
            }

            public override void Execute() {
                using (RenderCommand.CreateCommandScope(_context, ThisCmdBuffer, ThisProfSampler)) {
                    ThisPass.Execute(_context);
                }
                _context.SubmitCommands();
            }
        }

        public class FinalBlitPassNode : RenderPassNode<BlitRenderPass> {
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderContext>.RecvPort renderContext;
            
            [RenderPassInput(typeof(RenderContextInitPassNode))]
            public ResourcePipe<RenderTargetDesc>.RecvPort renderTargetDesc;
            
            public FinalBlitPassNode () 
                : base(name:nameof(FinalBlitPassNode),scopeTag:$"{nameof(FinalBlitPassNode )}@{nameof(BasicRenderFeature)}") {
            }
            
            private RenderContext _context;
            
            public override void Setup() {
                _context = renderContext.Receive();
                RenderTargetDesc desc = renderTargetDesc.Receive();
                var context = renderContext.Receive();
                //      Debug.Log($"Context received by {nameof(FinalBlitPassNode)}.");
                ThisPass.Source = desc.color;
                ThisPass.Destination = context.Camera.BackBuffer;
            }

            public override void Execute() {
                
                using (RenderCommand.CreateCommandScope(_context, ThisCmdBuffer, ThisProfSampler)) {
                    ThisPass.Execute(_context);
                }
                _context.SubmitCommands();
           
            }
        }
        

        private RenderContextInitPassNode _renderContextInitPassNode = new();
        private ClearDepthColorPassNode _clearDepthColorPassNode = new();
        private DrawSkyBoxPassNode _drawSkyBoxPassNode = new();
        private DrawOpaquePassNode _drawOpaquePassNode = new();
        private SolidColorPostProcessPassNode _solidColorPostProcessPassNode = new();
        private FinalBlitPassNode _finalBlitPassNode = new();
        private TestPassNode _testPassNode = new();
        public override IEnumerable<RenderPassNode> GetRenderPassNodes() {
         //   return new RenderPassNode[] { _testPassNode };
            return new RenderPassNode[]{_renderContextInitPassNode,_clearDepthColorPassNode,_drawSkyBoxPassNode,_solidColorPostProcessPassNode,_finalBlitPassNode};
        }

        
    }
}
```
