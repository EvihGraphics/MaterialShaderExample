# Egaku source: `Shaders/Sky/Cloud/Volumetric/Utils/TilePixelSelector.hlsl`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```hlsl
﻿#pragma once
#ifndef TILE_PIXEL_SELECTOR_HLSL
#define TILE_PIXEL_SELECTOR_HLSL

float _TilePixelSelector_FPS;
uint _TilePixelSelector_SelectedIndex;
float4 _TilePixelSelector_TileSize;

uint GetPixelIndexInTile(float2 pixelPosition, uint2 tileSize)
{
    uint2 pixelPos = uint2(pixelPosition);
    pixelPos = pixelPos % tileSize ;
    int index = pixelPos.x +  pixelPos.y * tileSize.x; 
    return index;
}

uint GetPixelIndexInTile(float2 pixelPosition)
{
    return GetPixelIndexInTile(pixelPosition, _TilePixelSelector_TileSize.xy);
}

uint2 GetTilePositionOfPixel(float2 pixelPosition, uint2 tileSize)
{
    uint2 tilePos  = uint2(pixelPosition) / tileSize ;
    return tilePos;
}

uint2 GetTilePositionOfPixel(float2 pixelPosition)
{
    return GetTilePositionOfPixel(pixelPosition, _TilePixelSelector_TileSize.xy);
}




uint2 GetSelectedPixelPositionOffset()
{
    uint2 tileSize =  _TilePixelSelector_TileSize.xy;
    uint selectedIndex = _TilePixelSelector_SelectedIndex;
    return uint2(selectedIndex%tileSize.x, selectedIndex/tileSize.x);
}

uint2 GetSelectedPixelPositionOfTile(uint2 tilePos)
{
    
    return tilePos* _TilePixelSelector_TileSize.xy + GetSelectedPixelPositionOffset();
}

bool IsPixelSelectedInTile(float2 pixelPosition,float timeInSec)
{
    uint2 tileSize=  _TilePixelSelector_TileSize.xy;
    uint index = GetPixelIndexInTile(pixelPosition, tileSize);
    int frame = timeInSec *  _TilePixelSelector_FPS;
    return index == frame % (tileSize.x*tileSize.y);
}



bool IsPixelSelectedInTile(float2 pixelPosition, uint2 tileSize)
{
    uint index = GetPixelIndexInTile(pixelPosition, tileSize);
    return index == (_TilePixelSelector_SelectedIndex%(tileSize.x*tileSize.y));
}

bool IsPixelSelectedInTile(float2 pixelPosition)
{
    return IsPixelSelectedInTile(pixelPosition, _TilePixelSelector_TileSize.xy);
}


#endif
```
