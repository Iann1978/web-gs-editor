# Gaussian Splatting Editor - Architecture

## Project Overview

Web-based 3D Gaussian Splatting editor built with Vue 3 and TypeScript, enabling users to load, view, edit, and export Gaussian splat files in the browser. Uses WebGPU for rendering through WebAssembly (gsr C++ render engine compiled to WASM via emdawnwebgpu) and WebAssembly for performance-critical operations.

## Technology Stack

**Frontend:** Vue 3 (Composition API), TypeScript, Vite, Pinia, Vuetify/Element Plus

**3D Rendering:** gsr (self-developed C++ render engine compiled to WASM), emdawnwebgpu, WebGPU

**WebAssembly:** C++ with Emscripten for rendering (gsr render engine), PLY parsing, spatial indexing (octree/k-d tree), raycasting, transform computations, bulk data processing. WebGPU rendering operations are performed in WASM via emdawnwebgpu bindings.

**Development:** ESLint, Prettier, Vitest, vite-plugin-wasm

## Project Structure

```
gs-editor-web/
├── docs/          # Documentation
├── public/        # Static assets (shaders, sample models)
├── wasm/          # WebAssembly source (C++), pkg/ compiled output
└── src/
    ├── components/    # Vue components (Scene3D, Toolbar, PropertiesPanel, FileManager, Layout)
    ├── composables/   # Composition functions (useSplatLoader, useSplatRenderer, useEditor, useSelection, useCamera)
    ├── core/
    │   ├── renderer/  # JavaScript wrapper/bindings for gsr WASM renderer
    │   ├── loader/    # File loaders (PLYLoader, SplatLoader, WASMLoader)
    │   ├── wasm/      # WASM bindings (renderer, parser, spatial, math)
    │   ├── editor/    # Editing (Editor, Transform, Selection, History)
    │   └── utils/     # Utilities (math, geometry, file)
    ├── stores/        # Pinia stores (editor, scene, ui)
    └── types/         # TypeScript types
```

## Architecture Overview

**Component Hierarchy:** App.vue → AppLayout.vue → [Toolbar, Scene3D (Renderer + CameraControls), PropertiesPanel, FileManager]

**Data Flow:**
1. File Load: PLY → WASM Parser → Splat Data → Editor Store → WASM Renderer (gsr) → Scene3D
2. User Interaction: Mouse/Keyboard → Editor → WASM Raycast → Selection Store → PropertiesPanel
3. Edit Operations: Transform → WASM Calculations → History → Editor Store → WASM Renderer Update

**State Management (Pinia):** Editor Store (tool, history, mode), Scene Store (splats, selection, properties, camera), UI Store (panels, theme, viewport)

## Key Technical Details

**Splat Format:** Position (vec3), Rotation (vec4 quaternion), Scale (vec3), Opacity (float), Color (vec3 RGB or spherical harmonics)

**Rendering Pipeline:** Load (PLY → WASM Parser → Splat Data) → Initialize (WASM Renderer init via emdawnwebgpu → WebGPU device/context) → Upload (WASM creates GPU buffers → instanced rendering setup) → Render (camera update → WASM culling/sorting → WASM renderer draw via emdawnwebgpu → WebGPU) → Interaction (ray → WASM raycast → selection → gizmos)

**WASM Modules:** Renderer (gsr C++ engine compiled to WASM, uses emdawnwebgpu for WebGPU access, handles GPU buffer creation, shader management, rendering commands), Parser (PLY parsing/validation), Spatial (octree/k-d tree, queries, culling), Math (raycasting, transforms, vectors/quaternions), Processing (filtering, sorting, bulk transforms)

**Algorithms:** Raycasting (WASM projects ray, checks bounding spheres, returns closest), Spatial Indexing (WASM maintains octree/k-d tree), Transparency (depth sort, back-to-front render)

## Performance & Optimization

**Targets:** Small (<100K): 60 FPS, Medium (100K-1M): 30-60 FPS, Large (>1M): 15-30 FPS with LOD

**Strategies:** LOD, frustum/occlusion culling, GPU instanced rendering, minimize JS/WASM transfer, batch operations, lazy loading
