# Editor Design - Layout Structure

## Overview

The Gaussian Splatting Editor follows a standard 3D editor layout pattern, providing a comprehensive interface for viewing, editing, and managing 3D Gaussian splat scenes. The layout is designed to maximize workspace efficiency while keeping essential tools and information easily accessible.

The editor interface is organized into distinct functional areas: a top toolbar for primary actions, a three-panel layout for scene management and editing, and an optional status bar for system feedback.

## Layout Structure

The editor uses a classic three-panel layout with a toolbar at the top:

```
┌─────────────────────────────────────────────────────────────┐
│  Toolbar (Top)                                               │
├──────────┬──────────────────────────────────────┬───────────┤
│ Hierarchy│      Viewport Canvas (Center)        │ Inspector │
│  Panel   │                                      │   Panel   │
│  (Left)  │                                      │  (Right)  │
├──────────┴──────────────────────────────────────┴───────────┤
│  Status Bar (Bottom) - Optional                              │
└─────────────────────────────────────────────────────────────┘
```

### Layout Breakdown

- **Toolbar (Top)**: Full-width horizontal bar containing primary editor actions, file operations, and tool selection
- **Hierarchy Panel (Left)**: Vertical panel displaying the scene graph and entity hierarchy
- **Viewport Canvas (Center)**: Main 3D rendering area where the Gaussian splat scene is displayed and interacted with
- **Inspector Panel (Right)**: Vertical panel for viewing and editing properties of selected entities
- **Status Bar (Bottom)**: Optional horizontal bar for displaying system status, performance metrics, and notifications

## Component Descriptions

### Toolbar (Top)

The toolbar provides quick access to the most commonly used editor functions and tools.

**Primary Functions:**
- File operations (New, Open, Save, Export)
- Edit operations (Undo, Redo, Cut, Copy, Paste)
- Scene operations (Add Entity, Delete Selected)
- Tool selection (Select, Move, Rotate, Scale)
- View controls (Reset Camera, Focus Selection)
- Render settings (Quality presets, Debug overlays)

**Design Considerations:**
- Icons with tooltips for space efficiency
- Grouped related actions
- Keyboard shortcuts displayed in tooltips
- Responsive layout that adapts to window width

### Hierarchy Panel (Left)

The hierarchy panel displays the scene structure as a tree view, showing all entities in the current scene.

**Primary Functions:**
- Display scene graph hierarchy
- Entity selection (click to select, Ctrl/Cmd+click for multi-select)
- Entity organization (drag-and-drop reordering)
- Entity visibility toggles
- Entity locking/unlocking
- Search and filter capabilities
- Context menu for entity operations (rename, duplicate, delete)

**Data Structure:**
- Tree view with expandable/collapsible nodes
- Icons indicating entity types (Mesh, Light, Camera, etc.)
- Visual indicators for selection state
- Indentation showing parent-child relationships

### Viewport Canvas (Center)

The viewport is the primary workspace where the 3D scene is rendered and where users interact with the Gaussian splat data.

**Primary Functions:**
- 3D scene rendering via WebGPU (gsr render engine)
- Camera controls (orbit, pan, zoom)
- Entity selection via mouse picking/raycasting
- Transform gizmos for Move, Rotate, and Scale tools
- Viewport overlays (grid, axes, gizmos)
- Multiple viewport support (optional: split view for different camera angles)

**Interaction:**
- Mouse controls for camera manipulation
- Keyboard shortcuts for tool switching
- Context menu for viewport-specific actions
- Drag-and-drop file loading
- Real-time rendering feedback

**Rendering:**
- WebGPU-based rendering through WASM (gsr engine)
- Real-time performance metrics
- Debug visualization modes
- Quality/performance presets

### Inspector Panel (Right)

The inspector panel displays and allows editing of properties for the currently selected entity or entities.

**Primary Functions:**
- Display selected entity properties
- Edit transform properties (Position, Rotation, Scale)
- Edit material properties (Color, Opacity, Shader parameters)
- Edit component-specific properties
- Multi-entity editing (common properties only)
- Property validation and constraints
- Property search/filter

**Property Categories:**
- **Transform**: Position (vec3), Rotation (quaternion/ Euler), Scale (vec3)
- **Gaussian Splat Properties**: Opacity, Color, Scale, Rotation
- **Rendering**: Material settings, shader parameters
- **Components**: Additional component-specific properties
- **Metadata**: Name, tags, custom properties

**Design Considerations:**
- Grouped properties by category
- Collapsible sections
- Input validation and constraints
- Real-time updates as values change
- Support for undo/redo per property change

### Status Bar (Bottom) - Optional

The status bar provides system feedback and information about the current editor state.

**Primary Functions:**
- Display current selection information
- Show rendering performance (FPS, frame time)
- Display file status (saved/unsaved indicator)
- Show coordinate information (cursor position in 3D space)
- Display operation progress (loading, processing)
- Show error/warning messages
- Display current tool mode

**Information Display:**
- Left side: Selection info, coordinates
- Center: Status messages, progress indicators
- Right side: Performance metrics, file status

## Layout Specifications

### Dimensions and Sizing

**Default Layout:**
- Toolbar height: 48-56px (fixed)
- Hierarchy panel width: 250-300px (resizable, min: 150px, max: 500px)
- Inspector panel width: 300-350px (resizable, min: 200px, max: 600px)
- Viewport: Flexible, fills remaining space
- Status bar height: 24-32px (fixed, when visible)

**Responsive Behavior:**
- Panels can be collapsed to icons only
- Panels can be hidden completely via toggle buttons
- Panels can be docked to different sides (optional feature)
- Minimum viewport size enforced to prevent unusable space

### Resizing Behavior

- **Panel Resizers**: Draggable dividers between panels allow manual resizing
- **Proportional Resizing**: When window resizes, viewport maintains priority while side panels maintain their widths
- **Snap Points**: Panels snap to common widths for consistency
- **Memory**: Panel widths are saved in user preferences

### Panel Interactions

- **Selection Sync**: Selecting an entity in Hierarchy updates Inspector and Viewport selection
- **Viewport Selection**: Clicking in Viewport updates Hierarchy and Inspector
- **Inspector Changes**: Property edits in Inspector immediately reflect in Viewport
- **Hierarchy Reordering**: Drag-and-drop in Hierarchy updates scene structure in real-time

## Component Relationships

### Data Flow

```
Toolbar Actions
    ↓
Editor Store (Pinia)
    ↓
    ├─→ Scene Store → Viewport Canvas (Rendering)
    ├─→ UI Store → Panel Updates
    └─→ Editor Store → History/Undo System
```

### Selection Flow

```
User Interaction
    ↓
    ├─→ Viewport (Mouse Pick) ──┐
    ├─→ Hierarchy (Click) ──────┼─→ Selection Store
    └─→ Inspector (Property) ──┘
    ↓
All Panels Update (Reactive)
```

### Property Editing Flow

```
Inspector Panel (User Input)
    ↓
Property Validation
    ↓
Editor Store (History Entry)
    ↓
Scene Store (Entity Update)
    ↓
WASM Renderer (gsr) Update
    ↓
Viewport Canvas (Visual Update)
```

### Component Dependencies

- **Toolbar** → Editor Store, UI Store
- **Hierarchy Panel** → Scene Store, Selection Store
- **Viewport Canvas** → Scene Store, WASM Renderer (gsr), Camera Store
- **Inspector Panel** → Selection Store, Scene Store, Editor Store
- **Status Bar** → UI Store, Scene Store, Performance Monitor

