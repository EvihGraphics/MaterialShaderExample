# The Forge 15_Transparency Scene Contract

## General Coordinate System Conversion
The Forge uses a Y-Up coordinate system. UE uses a Z-Up coordinate system in centimeters.
The standard mapping used for this contract is:
- UE.X = Forge.Z * 100.0
- UE.Y = Forge.X * 100.0
- UE.Z = Forge.Y * 100.0

This preserves handedness via a cyclic axis permutation.

## Objects
The scene includes the following objects mapping from `CreateScene()` in `15_Transparency.cpp`.

1. **Ground Plane (Cube)**
   - Forge: `pos(0,0,0), scale(100, 0.5, 100)`
   - UE: `pos(0,0,0), scale(100, 100, 0.5)`
2. **Transparent Cube Matrix (5x5)**
   - Forge: from X=-6..6, Z=-6..6, Y=2.3. Scale 1.0. Varying alpha.
3. **Thick RGB Walls (Right)**
   - Forge: `pos(15, 4, 5/0/-5), scale(4, 4, 0.1), alpha 0.9`
4. **Thick RGB Walls (Left)**
   - Forge: `pos(-15, 4, 5/0/-5), scale(4, 4, 0.1), alpha 0.5`
5. **High Color Thin Slices**
   - Forge: 25 cubes at `pos(-25..23, 4, 25), scale(0.1, 4, 4), color(1, 1, 10, 0.1)`
6. **Distant Red/Green Planes**
   - Forge: `pos(1, 5, -22), pos(-1, 5, -35)`
7. **Blue Refractive Sphere**
   - Forge: `pos(0, 5, -25), scale(4), eta 1.5`
8. **Lion Mesh**
   - Forge: `pos(10, 0, -25), scale(0.25)`
9. **Collimation Walls**
   - Forge: `pos(7/10/13, 5, -22), collimation 0/0.5/0.9`
10. **Refractive Spheres (3)**
    - Forge: `pos(-22.5/-20/-17.5, 5, -20), eta 1.001/1.3/1.5, collimation 1.0`
11. **Measuring Grid Plane**
    - Forge: `pos(-20, 10, -25), scale(10, 1, 10), rot(-90, 180, 0)`
12. **AVBOIT RGB Ground Truth Planes**
    - Forge: `pos(-10, 15, 10/5/0), scale(2, 2, 2), rot(-90, -45, 0)`

## Differences and Missing
- Particles are not currently modeled in this UE basic verification unless Niagara is integrated.
- Lion mesh will be replaced by a basic sphere or cube in our parity test if the mesh is not imported.
