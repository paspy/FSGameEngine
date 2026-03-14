# CLAUDE.md - FSGameEngine

## Project Overview

FSGameEngine (FSGD Game Engine) is a **DirectX 11-based 3D game engine** written in C++ with an educational focus. It features deferred rendering, physics simulation, collision detection, skeletal animation, Python scripting, and a component-based architecture (GOCA - Game Object Component Architecture).

**Target platform:** Windows (Win32 and x64)

## Repository Structure

```
FSGameEngine/
├── FSGDGame/              # Main game application (~147 source files)
│   ├── Resources/         # Game assets (textures, models, shaders)
│   └── Common.props       # Shared MSBuild properties
├── EDGameCore/            # Core game objects, components, behaviors, messaging
├── EDRendererD3D/         # DirectX 11 renderer (deferred shading pipeline)
├── EDCollision/           # Collision detection (AABB, OBB, sphere, capsule, ray)
├── EDPhysics/             # Rigid body physics simulation
├── EDGOCA/                # Component system framework (GameObject, Behavior, Attribute)
├── EDSpatial/             # Dynamic AABB tree spatial partitioning
├── EDMath/                # Math utilities (vectors, matrices, PID controllers)
├── EDUtilities/           # Content management, asset loading, file packing, logging
├── EDMemoryManager/       # Custom memory management with heap logging
├── EDEventManager/        # Event system for decoupled communication
├── EDInput/               # Input handling (keyboard, mouse, gamepad)
├── EDThreadPool/          # Thread pool for parallel task execution
├── EDTerrain/             # Terrain management and rendering
├── RTAnim/                # Real-time skeletal animation system
├── DirectXTK/             # Microsoft DirectX Toolkit (sprites, effects)
├── tinyxml/               # XML parsing library
├── UnitTest++/            # Unit testing framework
├── Scripts/               # Python scripts for game behaviors
├── Python/                # Embedded Python 3.4 runtime
├── ED1LabDocs/            # Educational lab docs (memory manager, threading, scripting)
├── ED2LabDocs/            # Educational lab docs (rendering, shadows, terrain, LOD)
├── DebugLibsWin32/        # Pre-built debug libraries (Win32)
├── DebugLibsx64/          # Pre-built debug libraries (x64)
├── ReleaseLibsWin32/      # Pre-built release libraries (Win32)
├── ReleaseLibsx64/        # Pre-built release libraries (x64)
└── Logs/                  # Runtime log output
```

### Module naming convention

All engine modules use the `ED` prefix (Engine Design). The `FSGDGame` project is the game application built on top of the engine.

## Build System

- **IDE:** Visual Studio 2013+ (MSBuild)
- **Solution file:** `FSGDGame_Student.sln`
- **Configurations:** Debug, Release, Template
- **Platforms:** Win32 (x86), x64
- **Precompiled headers:** Used throughout for build optimization
- **Project count:** 21 `.vcxproj` files

This is a Windows-only project. There is no CMake, Makefile, or cross-platform build system. All builds go through Visual Studio / MSBuild.

## Languages & Technologies

| Language | Usage |
|----------|-------|
| C++ | Engine and game code (primary) |
| HLSL | GPU shaders (vertex, pixel, geometry, hull, domain) |
| Python 3.4 | Scripting for game behaviors |
| XML | Scene/object serialization (via tinyxml) |

## Architecture

### Component System (GOCA)

The engine uses a **Game Object Component Architecture**:

- `IComponent` - Base interface for all components
- `IBehavior` - Behavior component (Update, FixedUpdate, LateUpdate)
- `Transform` - Spatial positioning/rotation/scale
- `IRenderer` - Rendering interface
- `ICollider` - Collision shapes (Box, Sphere, Capsule, Mesh, Character, Wheel)
- `RigidBody` - Physics simulation
- `ILight` - Lighting (Directional, Point, Spot)
- `Camera` - Rendering camera
- `Animation` - Skeletal animation

### Rendering Pipeline

- **Deferred shading** with G-buffer
- Post-processing: bloom, FXAA, depth of field, fog
- Shadow mapping for directional, point, and spot lights
- Terrain tessellation (hull/domain shaders)
- 2D sprite rendering via DirectXTK

### Entry Points

1. `FSGDGame/FSGDWinMain.cpp` - Windows entry point (`_tWinMain`)
2. `FSGDGame/FSGDGame.cpp` - Main game class initialization
3. `FSGDGame/GameBase.h` - Base game class interface

### Event-Driven Messaging

Game objects communicate through a decoupled event/message system (`EDEventManager`). Components send and receive messages without direct references to each other.

## Testing

- **Framework:** UnitTest++ (included in repo under `UnitTest++/`)
- **Test projects:** `UnitTest++.vsnet` and `UnitTest++Process` in the solution
- Tests are built and run through Visual Studio

## Key Conventions

- **Header guards:** Standard `#pragma once` or `#ifndef` guards
- **Namespaces:** Each ED module has its own namespace (e.g., `EDCollision`, `EDRendererD3D`)
- **Memory management:** Custom allocator via `EDMemoryManager` with forced include of memory overload headers
- **Asset loading:** Content manager pattern with XML-based scene definitions
- **Shader files:** Named with suffix convention (`.VS.hlsl`, `.PS.hlsl`, `.GS.hlsl`, `.HS.hlsl`, `.DS.hlsl`)
- **Compiled shaders:** `.cso` files are gitignored

## Dependencies (included in repo)

- DirectX 11 SDK (system)
- DirectXTK (included)
- TinyXML (included)
- Python 3.4 runtime (included)
- UnitTest++ (included)

## Notes for AI Assistants

- This is a **Windows-only** C++ project. Do not suggest cross-platform tooling.
- The build system is Visual Studio / MSBuild only. No CMake or Makefile exists.
- Pre-built library binaries are checked into the repo under `DebugLibs*` / `ReleaseLibs*` directories.
- HLSL shaders cannot be compiled or tested outside of the DirectX toolchain.
- Python scripting uses an embedded Python 3.4 interpreter — do not assume modern Python features.
- The `.gitignore` excludes `.cso` (compiled shader) files, build outputs, and VS user files.
- Many source files contain educational lab solution code — be careful not to remove instructional comments or TODO markers that are part of the curriculum.
- The `ED1LabDocs/` and `ED2LabDocs/` directories contain `.doc` files with lab instructions for students.
