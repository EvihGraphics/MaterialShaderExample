# Egaku source: `ShaderLibrary/Utilities/CBT/CBTTypedef.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef CBT_TYPEDEF_HLSL
#define CBT_TYPEDEF_HLSL

// data structures
struct cbt_Node {
    uint id;    // heapID
    int depth;  // findMSB(heapID) := node depth
};

struct leb_DiamondParent
{
    cbt_Node base, top;
};

#endif
```
