# What is it?
This is fork of [Apolyton CtP2 Source Code project](https://apolyton.net/forum/other-games/call-to-power-2/ctp2-source-code-project).
Original code was imported from  https://ctp2.darkdust.net/anonsvn/ repository.
# How to build

 1. Open ctp2_code/ctp/civctp.sln with MS Visual Studio 2017 Community Edition (VC++ and Platform SDK for Windows 10 should be installed).
 3. Build "ctp2" project.
# Goals
The main goal of this project is to completely replace DirectDraw based graphics engine with Direct3D 11 based 2d engine.
## Concept
All rendering in original CtP2 based on a set of primitive drawing functions (like `primitives_DrawText()`, `primitives_BevelLeftPiece16()` etc.) and various XxxBltYyy functions(like `ColorBlt()`, `StretchBlt()` etc.), so this is what should be replaced.
The new graphics engine provides similar functions but instead of immediate blitting these funtions just change scene's state (scene is a tree of rectangles and primitives).
On the render stage the renderer takes scene data as input and translates it to real drawing instructions. Now I am working on D3d renderer implementation, but OpenGL or Skia library based renderers may also be easily implemented.
## Status
Check https://github.com/RolandTaverner/ctp2/tree/d3dui

> Written with [StackEdit](https://stackedit.io/).
