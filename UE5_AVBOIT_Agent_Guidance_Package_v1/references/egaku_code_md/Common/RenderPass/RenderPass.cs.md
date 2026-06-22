# Egaku source: `Common/RenderPass/RenderPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿

using System;
using UnityEngine.Rendering;

namespace Egaku.Rendering.Pipeline.Common {
    
    

    public abstract class RenderPass : IComparable<RenderPass>{
        private int _order;

        private string _name;


        private ProfilingSampler _profSampler;
        
        private CommandBuffer _cmdBuf;
        
        [PipelineRenderContext]
        private RenderContext _renderContext;
        
        public RenderContext PipelineRenderContext {
            get => _renderContext;
            set => _renderContext = value;
        } 
        
        public CommandBuffer CmdBuffer {
            get => _cmdBuf;
        }
        
        public int Order {
            get => _order;
            set => value = _order;
        }

        public string Name {
            get => _name;
            set => value = _name;
        }
        
        public ProfilingSampler ProfilingSampler {
            get => _profSampler;
        }

        private void _InnerCreate(int order, string name) {
            _order = order;
            _name = name;
            _profSampler = new ProfilingSampler(name);
            _cmdBuf = CommandBufferPool.Get();
            _cmdBuf.name = Name;
        }
  
        
        protected RenderPass() : this(-1, "Unnamed Render Pass") {
        }
        
        protected RenderPass(int order):this(order, "Unnamed Render Pass") {
        }
        
        protected RenderPass(string name) : this(0,name){
        }
        
        protected RenderPass(int order,string name) {
            _InnerCreate(order,name);
        }


        public void ExecuteWithProfiling(RenderContext context) {
     
            Execute(context);
        }
        
        public virtual void Execute(RenderContext context) {
            
        }

        public virtual void Setup() {
            
        }
        

        public int CompareTo(RenderPass other) {
            if ( Order < other.Order) {
                return -1;
            }
            if (Order > other.Order) {
                return 1;
            }
            return 0;
        }
    }
    
    public class RenderPass<TRenderData> : RenderPass{
        public virtual void Execute(RenderContext context,TRenderData renderData) {
            base.Execute(context);
        }
    }
}
```
