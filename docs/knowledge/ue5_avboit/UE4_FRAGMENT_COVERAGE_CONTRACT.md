# UE4 Fragment Coverage Contract

## Coordinate Space
- Standard 2D pixel coordinate space `[0, ViewResolution.x)` and `[0, ViewResolution.y)`.
- Origin `(0,0)` is at the top-left of the viewport.

## Rect Definition
All synthetic fragments are defined with a rectangular bound:
- `RectMin`: Inclusive lower bound.
- `RectMax`: Exclusive upper bound.

Interval: `[RectMin, RectMax)`

## Shader Intersection Rule
A fragment contributes to a pixel `(x, y)` if and only if:
```hlsl
x >= RectMin.x && x < RectMax.x &&
y >= RectMin.y && y < RectMax.y
```

## Resolution & ROI
- Synthetic test resolution: `512x512`.
- Region of Interest (ROI) used for CPU validation: `x ∈ [224, 288)` and `y ∈ [224, 288)`.
- The ROI perfectly matches the fragment `RectMax` bounds without out-of-bounds overflow.
