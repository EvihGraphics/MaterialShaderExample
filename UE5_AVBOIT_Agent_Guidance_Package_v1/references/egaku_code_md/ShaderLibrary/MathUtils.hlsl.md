# Egaku source: `ShaderLibrary/MathUtils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#pragma once
#ifndef MATH_UTILS_HLSL
#define MATH_UTILS_HLSL
#include "Macros.hlsl"

#define POW_CLAMP 0.000001f

TEMPLATE_1_REAL(Sq, x, {return x*x;})

TEMPLATE_1_REAL(Square, x, {return Sq(x);})

TEMPLATE_1_REAL(Pow2, x, {return Sq(x);})

TEMPLATE_1_REAL(Pow3, x, {return x*x*x;})

TEMPLATE_1_REAL(Pow4, x, {return Sq(Sq(x));})

TEMPLATE_1_REAL(Pow5, x, {return Pow4(x)*x;})

TEMPLATE_1_REAL(Pow6, x, {return Pow3(Sq(x));})

TEMPLATE_2_REAL(SafePow, x,y, {return pow(max(abs(x),POW_CLAMP),y);})

#endif
```
