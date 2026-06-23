# UE4 AVBOIT Fragment Injection Contract

To execute and validate the AVBOIT Plugin-Owned Vertical Slice without integrating with the engine's Rasterization pipeline (which risks linking bugs), we define a Synthetic Deterministic Fragment Injection protocol.

## C++ Fragment Structure
The C++ test harness (`AVBOITRGBTestAutomation`) will submit a list of fragments.
```cpp
struct FAVBOITInjectedFragment
{
    FVector4f LinearColorAndAlpha; // R, G, B, A (Linear space, 0-1)
    float LinearDepth;             // Raw Linear Depth in Unreal Units (cm)
    uint32 SubmissionIndex;        // Tracks CPU submission order
    FIntPoint RectMin;             // 2D ROI Min (e.g. 224, 224)
    FIntPoint RectMax;             // 2D ROI Max (e.g. 288, 288)
};
```
* **Byte Alignment**: 32 bytes total. (16 bytes Color + 4 bytes Depth + 4 bytes Index + 8 bytes Rect = 32 bytes). Perfect multiple of 16 for `StructuredBuffer`.

## GPU Shader Equivalent
```hlsl
struct FInjectedFragment
{
    float4 LinearColorAndAlpha;
    float LinearDepth;
    uint SubmissionIndex;
    int2 RectMin;
    int2 RectMax;
};
```

## Injection Rules
* **SingleWhite**: 1 fragment submitted.
* **DoubleRedBlue**: 2 fragments submitted. Two passes are run: front->back and back->front.
* **TripleRGB**: 3 fragments submitted. Six passes are run across all order permutations.
* The CPU will *only* provide raw depth and color. The GPU must independently compute Extinction, Slice Index, Transmittance, and Final Shaded Color. CPU mathematically calculated `ExpectedColor` is explicitly FORBIDDEN from being uploaded to the GPU result texture.
