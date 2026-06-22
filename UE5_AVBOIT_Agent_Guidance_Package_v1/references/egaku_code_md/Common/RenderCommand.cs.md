# Egaku source: `Common/RenderCommand.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Runtime.CompilerServices;
using Unity.Collections;
using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {
   
    // this class should contain buffered command recording procedures only.
    public static class RenderCommand {
        
  
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)] 
        public static void SetupCurrentCameraProperties(RenderContext context) {
            context.InternalRenderContext.SetupCameraProperties(context.Camera.InternalCamera);
        }

        
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)] 
        public static CullingResults Cull(RenderContext context,ref ScriptableCullingParameters p) {
            return context.InternalRenderContext.Cull(ref p);
        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void DrawRenderers(RenderContext context,in CullingResults cullingResults,ref DrawingSettings drawingSettings,ref FilteringSettings filteringSettings) {
            context.InternalRenderContext.DrawRenderers(cullingResults, ref drawingSettings, ref filteringSettings);
        }
        
  

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void DrawSkybox(RenderContext context) {
            context.InternalRenderContext.DrawSkybox(context.Camera.InternalCamera);
        }

    
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void InjectCommands(RenderContext context,CommandBuffer cmdBuf) {
            context.InternalRenderContext.ExecuteCommandBuffer(cmdBuf);
        }

                
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void InjectCommandsAndClear(RenderContext context,CommandBuffer cmdBuf) {
            InjectCommands(context, cmdBuf);
            cmdBuf.Clear();
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void BeginCommandScope(RenderContext context,CommandBuffer cmdBuf,ProfilingSampler profSampler = null) {
            profSampler?.Begin(cmdBuf);
            InjectCommandsAndClear(context,cmdBuf);
        }
        
        
        // This method will automatically inject the command buffer and clear it!
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void EndCommandScope(RenderContext context,CommandBuffer cmdBuf,ProfilingSampler profSampler = null) {
            profSampler?.End(cmdBuf);
            InjectCommandsAndClear(context,cmdBuf);
        }

        // Create an IDisposable struct to make use of 'using' semantics to automatically end a command scope.
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static CommandScope CreateCommandScope(RenderContext context, CommandBuffer cmdBuf,
            ProfilingSampler profSampler = null) {
            return new CommandScope(context, cmdBuf, profSampler);
        }
        
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static ScopedRenderPass CreateRenderPassScope(RenderContext context,int width,
            int height,
            int samples,
            in NativeArray<AttachmentDescriptor> attachments,
            int depthAttachmentIndex = -1) {
            return context.InternalRenderContext.BeginScopedRenderPass(width, height, samples, attachments, depthAttachmentIndex);
        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static ScopedRenderPass CreateRenderPassScope(RenderContext context,int samples,
            in NativeArray<AttachmentDescriptor> attachments,
            int depthAttachmentIndex = -1) {
            return CreateRenderPassScope(context,context.Camera.PixelWidth, context.Camera.PixelHeight, samples,attachments,
                depthAttachmentIndex);
        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static ScopedSubPass CreateSubPassScope(RenderContext context,in NativeArray<int> colors, bool isDepthStencilReadOnly = false)
        {
            return context.InternalRenderContext.BeginScopedSubPass(colors, isDepthStencilReadOnly);
        }
    }
}
```
