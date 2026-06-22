# Egaku source: `Shaders/Sky/Cloud/Volumetric/RandomNumberUtils.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
#ifndef RANDOM_NUMBER_UTILS_HLSL
#define RANDOM_NUMBER_UTILS_HLSL

static float randomSeed;

void SetRandomSeed(float seed) {
    randomSeed = seed;
}

float GetRandom(){
   // float result = frac(sin(randomSeed))
    float result = sin(randomSeed * 641.5467987313875 + 1.943856175);
    SetRandomSeed(result);
    return result;
}

float GetRandomWithTimeSeed(){
    // float result = frac(sin(randomSeed))
    float result = sin(randomSeed * _Time.y * 641.5467987313875 + 1.943856175);
    SetRandomSeed(result);
    return result;
}

float GetRandomWithTimeSeed(float min,float max){
    // float result = frac(sin(randomSeed))
    return min + (max - min) * 0.5 * (GetRandomWithTimeSeed() + 1);
}

float Get01RandomWithTimeSeed(float a,float b)
{
    return frac(sin(a + _Time.y)*sin(b - _Time.y)*999999);
}

float GetRandom(float min, float max) {
    return min + (max - min) * 0.5 * (GetRandom() + 1);
}

float GetRandom(float range) {
    return range*GetRandom();
}

#endif //RANDOM_NUMBER_UTILS_HLSL
```
