# Egaku source: `Scripts/Common/Rendering/CustomRendererManager.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using System;
using System.Collections.Generic;


namespace Common.Rendering {
   
    
    
    public class CustomRendererManager {
        private static readonly Dictionary<Type, CustomRenderer> _customRenderers = new();

        
        public static T GetOrCreateCustomRenderer<T>() where T : CustomRenderer,new(){
            //Assert.IsTrue(LangReflectionUtils.IsTypeDerivedFrom<CustomRenderer>(typeof(T)));
            CustomRenderer renderer;
            if (_customRenderers.TryGetValue(typeof(T),out renderer) == false) {
                TryAddCustomRenderer<T>();
                renderer = _customRenderers[typeof(T)];
            }

            return (T)renderer;
        }
        
        public static T TryGetCustomRenderer<T>() where T : CustomRenderer,new(){
            if (_customRenderers.TryGetValue(typeof(T),out var renderer) == false) {
                return null;
            }
            return (T)renderer;
        }

        public static bool TryAddCustomRenderer<T>() where T : CustomRenderer,new() {
            return _customRenderers.TryAdd(typeof(T), new T());
        }
        
        public static bool TryAddCustomRenderer<T>(T renderer) where T : CustomRenderer,new() {
            return _customRenderers.TryAdd(typeof(T), renderer);
        }

        public static void InitializeCustomRenderers() {
            foreach (var (type,renderer) in _customRenderers) {
                if (renderer.Enabled && renderer.Valid) {
                    renderer.Initialize();
                }
            }
        }
        
       

    }
}
```
