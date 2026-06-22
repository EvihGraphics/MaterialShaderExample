# Egaku source: `Shaders/Test/TestShader.compute`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef CBT_GROUP_SIZE
#define CBT_GROUP_SIZE 32
#endif
struct CBTBuffer
{
   float data;
};

RWStructuredBuffer<CBTBuffer> cbtBuffer;

#pragma kernel CSMain_CBT
[numthreads(CBT_GROUP_SIZE,1,1)]
void CSMain_CBT(uint threadID : SV_DispatchThreadID)
{
   float old;
   //InterlockedAdd(cbtBuffer[0].data,1,old);
   cbtBuffer[1].data = 10.0F;
}
```
