# Basic Viewer - Development Plan

## Overview

Development plan for Basic Viewer functionality. Enables loading, parsing, and rendering Gaussian Splatting files in a web-based viewer with camera controls.

## Development Tasks (Week 1-2)

### Dependencies Setup
- [ ] Install Pinia for state management
- [ ] Install VueUse for composition utilities
- [ ] Install vite-plugin-wasm for WASM module loading
- [ ] Configure Vite for WASM support

### WASM Module Setup
- [ ] Initialize C++ WASM project with Emscripten
- [ ] Create WASM module structure (wasm/src/, wasm/pkg/)
- [ ] Setup build configuration (Emscripten compilation)
- [ ] Implement WASM module loader (src/core/loader/WASMLoader.ts)

### PLY File Parser
- [ ] Implement C++ PLY parser in WASM
- [ ] Parse binary PLY format for Gaussian Splatting
- [ ] Extract splat data (position, rotation, scale, opacity, color)
- [ ] Validate parsed data structure
- [ ] Create TypeScript bindings for WASM parser
- [ ] Transfer parsed data from WASM to JavaScript (typed arrays)

### gsr Render Engine Integration
- [ ] Integrate gsr render engine into project
- [ ] Create GSRRenderer wrapper (src/core/renderer/gsr/GSRRenderer.ts)
- [ ] Initialize WebGPU renderer in gsr
- [ ] Create GPU buffers for splat data (position, rotation, scale, color, opacity)
- [ ] Implement instanced rendering pipeline
- [ ] Setup shader management for Gaussian Splatting

### Camera Controls
- [ ] Implement orbit camera controls (rotate around center)
- [ ] Implement pan controls (translate view)
- [ ] Implement zoom controls (dolly in/out)
- [ ] Create CameraControls component
- [ ] Integrate camera with gsr renderer

### File Loading Interface
- [ ] Create FileLoader component
- [ ] Implement drag-and-drop file interface
- [ ] Handle PLY file selection (file input)
- [ ] Display loading progress indicator
- [ ] Error handling for invalid files

### State Management
- [ ] Create scene store (stores/scene.ts) for splat data
- [ ] Create UI store (stores/ui.ts) for loading state
- [ ] Integrate stores with components

### Core Components
- [ ] Create Scene3D component (main viewport)
- [ ] Create Renderer component (gsr renderer wrapper)
- [ ] Update AppLayout to include Scene3D
- [ ] Implement render loop (requestAnimationFrame)

## Development Workflow

**Setup:** `npm install pinia @vueuse/core vite-plugin-wasm -D` (Emscripten for C++ WASM)

**Commands:** `npm run dev` (development), `npm run build` (build)

## Deliverables

PLY file loader (WASM parser), WebGPU renderer (gsr), camera controls (orbit/pan/zoom), drag-and-drop interface, error handling
