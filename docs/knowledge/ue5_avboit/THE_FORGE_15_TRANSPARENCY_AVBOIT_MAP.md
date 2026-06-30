# The Forge 15_Transparency AVBOIT Map

Generated UTC: 20260630T171030Z

## Reference Role

The Forge 15_Transparency remains the algorithmic reference for AVBOIT resource layout, pass order, and debug expectations. UE-4.2D uses it as a contract reference, not as a direct code import.

## Mapping To UE Plugin

- The Forge clear/init maps to `AVBOIT.Niagara.Clear`.
- The Forge transparent geometry splat maps to `AVBOIT.Niagara.SpriteSplat`.
- The Forge transmittance integration maps to `AVBOIT.Niagara.Integrate`.
- The Forge forward transparent shade maps to `AVBOIT.Niagara.ForwardUnlit`.
- The Forge final resolve maps to `AVBOIT.Niagara.Composite`.

## UE-Specific Constraints

- UE units and camera projection must come from runtime view state.
- UE material and Niagara renderer contracts must be captured without saving Epic assets.
- UE SceneColor composite must be made through RDG render target binding or an equivalent renderer extension hook.
- Plugin-only work may be blocked by private Niagara renderer internals; such blockers must be documented rather than bypassed with default renderer fallback.

## Current UE-4.2D Status

- Pass names and intermediate resource names are present.
- True Niagara sprite material/VF input is not yet present.
- Composite is scratch-only and not final SceneColor.
- The mapping is therefore a foundation map, not passed parity evidence.
