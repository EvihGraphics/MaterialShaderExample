# Egaku source: `ShaderLibrary/Lighting/Volumetric.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#ifndef VOLUMETRIC_HLSL
#define VOLUMETRIC_HLSL

#include "../Typedef.hlsl"

real IsotropicPhase()
{
    return INV_FOUR_PI;
}

real RayleighPhase(real cosTheta)
{
    real k = 3 / (16 * PI);
    return k * (1 + cosTheta * cosTheta);
}

// Henyey-Greenstein approximation
real HGPhase(real g, real cosTheta)
{
    return (1 - g*g) / (FOUR_PI* pow(1.0 + g*g - 2 * g * cosTheta, 1.5));
}

// Schlick approximation
real SchlickPhaseFromK(real k, real cosTheta)
{
    real f = 1 + k*cosTheta;
    return (1 - k*k) / (FOUR_PI * f * f);
}

real SchlickPhase(real g, real cosTheta)
{
    real k = 1.55 * g - 0.55 *g *g *g;
    return SchlickPhaseFromK(k, cosTheta);
}

real ExtinctionCoefficient(real scatterCoeff,real absorptionCoeff)
{
    return scatterCoeff + absorptionCoeff;
}

real3 ExtinctionCoefficient(real3 scatterCoeff,real3 absorptionCoeff)
{
    return scatterCoeff + absorptionCoeff;
}

real VolumeAlbedo(real scatterCoeff,real extinctionCoeff)
{
    return scatterCoeff/extinctionCoeff;
}

real3 VolumeAlbedo(real3 scatterCoeff,real3 extinctionCoeff)
{
    return scatterCoeff/extinctionCoeff;
}

real VolumeOpacity(real transmittance)
{
    return (real)1 - transmittance;
}

real3 VolumeOpacity(real3 transmittance)
{
    return (real3)1 - transmittance;
}


// Beer's law
real BeerTransmittance(real opticalDepth)
{
    return exp(-opticalDepth);
}

real3 BeerTransmittance(real3 opticalDepth)
{
    return exp(-opticalDepth);
}

// Homogeneous medium

real HomogeneousMediumOpticalDepth(real extinctionCoeff,real distance)
{
    return extinctionCoeff * distance;
}

real3 HomogeneousMediumOpticalDepth(real3 extinctionCoeff,real3 distance)
{
    return extinctionCoeff * distance;
}

real HomogeneousMediumTransmittance(real extinctionCoeff, real distance)
{
    return BeerTransmittance(HomogeneousMediumOpticalDepth(extinctionCoeff,distance));
}

real3 HomogeneousMediumTransmittance(real3 extinctionCoeff, real distance)
{
    return BeerTransmittance(HomogeneousMediumOpticalDepth(extinctionCoeff,distance));
}


real HomogeneousMediumTransmittanceIntegral(real extinctionCoeff, real maxTransmittance)
{
    return rcp(extinctionCoeff) -  maxTransmittance * rcp(extinctionCoeff);
}

real3 HomogeneousMediumTransmittanceIntegral(real3 extinctionCoeff, real3 maxTransmittance)
{
    return rcp(extinctionCoeff) -  maxTransmittance * rcp(extinctionCoeff);
}

real3 HomogeneousMediumScatter(real3 scatterIntensity, real3 extinctionCoeff, real3 transmittance)
{
    return scatterIntensity * HomogeneousMediumTransmittanceIntegral(extinctionCoeff,transmittance);
}

real3 HomogeneousMediumScatter(real3 mediumAlbedo, real3 transmittance)
{
    return mediumAlbedo * ((real3)1 - transmittance);
}



#endif
```
