# 3D printed case

Designed in OpenSCAD with [BOSL2](https://github.com/BelfrySCAD/BOSL2) library, exportable from `top.scad` and `bottom.scad`.

## Design

Object orientation:

- bottom is flat part adjacent to ground
- top is flat, it may feature holes for reset button in future, but it should stay flat
- front is facing towards user and feature OLED display, buttons or light sensor may be added here
- sides are flat and should remain that way
- bottom is for all cables:
  - USB connector directly on MCU
  - 4-pin fan connectors
  - cables for thermometers, currently implemented as simple clamps for insulation (wires connected to screw terminals accessible only from inside)

Designed in two parts: bottom and top, both connected to perfboard via screw posts. Each part contains top/bottom surface and some part of side walls. 

### Bottom

- side walls have full height here (not strict requirement, can be any ratio of walls from top and bottom)
- front has full height and window for display, as well as 4x screw hole; it must be full height to keep display mounted to one part of the case only
  - this part requires support
- back is split to two parts with different height:
  - left is for thermometer and fan connectors:
    - full cutout for thermometer cables - this is baseline of height
    - portion of cutouts for fan connectors
  - right is for USB, cutout for exactly half of final hole

### Top

- no front and side walls
- back wall:
  - similar split as in bottom part of case
  - extra screw hole for stabilizing riser board

## Printability

Initial goal was to avoid any supports (i.e. holes in side walls should be split in middle), but it was problematic with display that has to be screwed in place. This single hole is not structurally important and works well with basic supports.

Printing on largest surface (external surface of top/bottom) on plate.

My print settings:

- Creality Hyper PLA-CF
- 0.16mm layers
- 2x wall loop
- 10% in-fill cross-hatch
- supports - only for display window:
  - tree
  - only on build plate
  - 10 degree threshold
- aligned seam
