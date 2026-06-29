# ExtendedGraphicsProgramming

This plugin is sourced from: https://github.com/heyx3/ExtendedGraphicsProgramming

It is a dependency of `UnrealExtendedGraphicsProgrammingDemo` (originally a nested git submodule).

## Setup

Clone this directory manually:
```
git clone --depth 1 https://github.com/heyx3/ExtendedGraphicsProgramming .
```

## Applied Patches

The following modifications are required for UE 5.7.4 compatibility:

1. **EGP_CustomRenderPasses.h**: Add `#include <memory>` to resolve `std::unique_ptr` compilation error
2. **ExtendedGraphicsProgramming.Build.cs**: Add `Renderer/Internal` to `PrivateIncludePaths`
