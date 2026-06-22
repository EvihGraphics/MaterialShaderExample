# Egaku source: `Runtime/Modules/CommonRenderPasses/InitialRenderPass.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System.Diagnostics;
using System.Reflection;
using Egaku.Rendering.Pipeline.Common;
using UnityEngine.Rendering;
using Debug = UnityEngine.Debug;

namespace Egaku.Rendering.Pipeline.Runtime {
    
    public class ContextInitRenderPass : RenderPass {
        
        public class BasicRenderData : RenderResource {

            public RenderTargetHandle renderTarget;
            public int frameCount;
            public override void Create() {
                frameCount = 0;
                renderTarget = null;
                StackTrace stackTrace = new StackTrace();
                StackFrame callerFrame = stackTrace.GetFrame(1);
                MethodBase callerMethod = callerFrame.GetMethod();
                CodeScope.Debug(
                    ()=> {
                        string s = $"{typeof(RenderTargetHandle).Name}.{nameof(Create)} " +
                                   $"is called by {callerMethod.Name}.";
                        Debug.Log(s);
                    });
            }

            public override void AfterConsumption() {
                StackTrace stackTrace = new StackTrace();
                StackFrame callerFrame = stackTrace.GetFrame(1);
                MethodBase callerMethod = callerFrame.GetMethod();
                CodeScope.Debug(
                    ()=> {
                        string s = $"{typeof(RenderTargetHandle).Name}.{nameof(AfterConsumption)} " +
                                   $"is called by {callerMethod.Name}.";
                        Debug.Log(s);
                    });
            }
            
        }

        [SharedRenderResource(role = RenderResource.Role.Producer)]
        public readonly BasicRenderData renderData;


        public class InputData {
            public int param1;
        }

        public class OutputData {
            
        }
        


        
        
    }
    
}
```
