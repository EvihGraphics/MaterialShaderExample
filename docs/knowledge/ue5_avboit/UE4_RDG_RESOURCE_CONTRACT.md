# UE4 AVBOIT RDG Resource Contract

## 1. Resolution & Sizing
* **Width**: 512
* **Height**: 512
* **Slice Count**: 64

## 2. Resource Layout & Memory footprint
### 2.1 Extinction Volume
* **Format**: `PF_R32_UINT`
* **Resource Type**: `Texture2DArray` (Since Unreal RDG natively supports `FRDGTextureDesc::Create2DArray`)
* **Size Config**: 512 x 512 x 64
* **Memory**: `512 * 512 * 64 * 4 bytes` = `67,108,864` bytes (64 MiB)
* **Access**: UAV for atomic adds in the Splat/Build pass, SRV for integration pass.

### 2.2 Transmittance Volume
* **Format**: `PF_R32_FLOAT`
* **Resource Type**: `Texture2DArray`
* **Size Config**: 512 x 512 x 64
* **Memory**: `512 * 512 * 64 * 4 bytes` = `67,108,864` bytes (64 MiB)
* **Access**: UAV for the Integration pass, SRV for the Shading and Composite passes.

### 2.3 AVBOIT Result Texture
* **Format**: `PF_FloatRGBA` (16-bit Float per channel, standard HDR)
* **Resource Type**: `Texture2D`
* **Size Config**: 512 x 512
* **Memory**: `512 * 512 * 8 bytes` = `2,097,152` bytes (2 MiB)
* **Access**: UAV for Shading pass (accumulating standard LinearColor output), Readback compatibility.

### 2.4 Total VRAM Footprint
* Extinction: 64 MiB
* Transmittance: 64 MiB
* Result: 2 MiB
* **Total**: 130 MiB

This comfortably fits within limits and provides sufficient exact mathematical precision for the Unit tests.
