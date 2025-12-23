# Shader Conventions

## Code Style

### Uniform Naming Convention

All uniform variables must use the `u_` prefix to distinguish them from local variables and improve code readability.

**Examples:**
```wgsl
@group(0) @binding(0) var<uniform> u_model_view_proj: mat4x4<f32>;  // ✓ Correct
@group(0) @binding(1) var<uniform> u_screen_size: vec2<f32>;       // ✓ Correct

@group(0) @binding(0) var<uniform> model_view_proj: mat4x4<f32>;   // ✗ Incorrect
```

**Rationale:**
- Makes uniforms easily identifiable in shader code
- Prevents naming conflicts with local variables
- Consistent with common shader coding conventions

## Vertex Semantics

Vertex attribute semantics defined in `VertexSemantic` namespace:

- `POSITION` = "position" (slot 0)
- `COLOR` = "color" (slot 1)
- `UV` = "texcoord" (slot 2)
- `NORMAL` = "normal" (slot 3)
- `TANGENT` = "tangent" (slot 4)
- `BITANGENT` = "bitangent" (slot 5)

Each semantic has a **fixed slot number** assigned via `VertexSemantic::getSlot()`. This ensures consistent buffer binding regardless of insertion order or map iteration order.

## WGSL Shader Structure

Shaders are embedded as C++ string literals in `Shader::buildPredefined()` and created via `WebGPUContext::CreateShaderModule()`.

### Location Bindings

Vertex attributes use `@location` annotations matching the semantic's fixed slot number:

```wgsl
struct VertexInput {
    @location(0) position: vec3<f32>,  // POSITION semantic = slot 0
    @location(1) color: vec3<f32>,     // COLOR semantic = slot 1
};
```

Location indices correspond to fixed semantic slots:
- `@location(0)` = POSITION semantic
- `@location(1)` = COLOR semantic
- `@location(2)` = UV semantic
- `@location(3)` = NORMAL semantic
- `@location(4)` = TANGENT semantic
- `@location(5)` = BITANGENT semantic

**Important:** Shader `@location` annotations must match the semantic's fixed slot number, not the order in which attributes are added to the layout.

### Shader-Mesh Binding

`Shader::BindMeshBuffers()` matches shader locations to mesh channel buffers using each semantic's fixed slot number. The semantic determines the slot assignment, not the insertion order.

**Example:**
```cpp
// Add attributes in any order - slots are determined by semantic
shader->layout.add(VertexSemantic::COLOR, FLOAT3);     // Will bind to slot 1
shader->layout.add(VertexSemantic::POSITION, FLOAT3);   // Will bind to slot 0

// BindMeshBuffers uses semantic slots, not insertion order
// POSITION buffer → slot 0, COLOR buffer → slot 1
Shader::BindMeshBuffers(pass, mesh, shader);
```

**Key Points:**
- Slot numbers are fixed per semantic (POSITION=0, COLOR=1, etc.)
- Insertion order does not affect slot assignment
- Shader `@location` annotations must match semantic slots
- Buffer binding is consistent regardless of `std::map` iteration order

