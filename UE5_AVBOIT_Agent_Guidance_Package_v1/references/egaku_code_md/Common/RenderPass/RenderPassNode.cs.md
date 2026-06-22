# Egaku source: `Common/RenderPass/RenderPassNode.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿
using UnityEngine;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {
    public abstract class RenderPassNode : ScriptableObject {
        public enum Role {
            Producer,
            Consumer
        }
        
        private string _name;
        
        private string _scopeTag;

        private bool _active;
  
        // This must be a reference to current renderer's manager. Don't new one by yourself. 
        private RenderResourceManager _resourceManager;
        protected RenderResourceManager ResourceManager => _resourceManager;
        
        protected string Name { get => _name; }
        protected string ScopeTag { get => _scopeTag; }
        
        public bool Active { get => _active; set => _active = value; }
        
        // consider using attributes to do field injection?
        public void _InternalCreate(RenderResourceManager renderResourceManager){
            _resourceManager = renderResourceManager;
        }

        protected RenderPassNode(string name) {
            _name = name;
            _scopeTag = name;
        }

        protected RenderPassNode(string name, string scopeTag) {
            _name = name;
            _scopeTag = scopeTag;
        }
        
        protected RenderPassNode() : this("Unnamed Render Pass Node") {
            
        }
        
        
        
        public virtual void Setup() {
            
        }

        public abstract void Execute();

        // interface for cleaning resources. (call Release() or Dispose() on resources if necessary in current pass node)
        public virtual void Clean() {
            
        }
        
        // interface for bypassing current pass node
        public virtual void Bypass() {
            
        }
        
    }
    
    public abstract class RenderPassNode<TRenderPass> : RenderPassNode where TRenderPass : RenderPass, new() { 
        private TRenderPass _thisPass = new();
                
        private ProfilingSampler _thisProfSampler;
        
        private CommandBuffer _thisCmdBuffer; 
        protected TRenderPass ThisPass => _thisPass;
        
        protected ProfilingSampler ThisProfSampler => _thisProfSampler;

        protected CommandBuffer ThisCmdBuffer => _thisCmdBuffer;

        private void _InternalCreate() {
            _thisProfSampler = new ProfilingSampler($"Prof_{ScopeTag}");
            _thisCmdBuffer = new CommandBuffer {name = ScopeTag};
        }
        
        protected RenderPassNode() : base() {
            _InternalCreate();
        }
        protected RenderPassNode(string name) : base(name) {
            _InternalCreate();
        }
   
        protected RenderPassNode(string name,string scopeTag) : base(name,scopeTag) {
            _InternalCreate();
        }
    }
}
```
