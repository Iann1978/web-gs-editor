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

- `POSITION` = "position"
- `COLOR` = "color"
- `UV` = "texcoord"
- `NORMAL` = "normal"
- `TANGENT` = "tangent"
- `BITANGENT` = "bitangent"

## WGSL Shader Structure

Shaders are embedded as C++ string literals in `Shader::buildPredefined()` and created via `WebGPUContext::CreateShaderModule()`.

### Location Bindings

Vertex attributes use `@location` annotations matching the shader's `VertexLayout` order:

```wgsl
struct VertexInput {
    @location(0) position: vec3<f32>,
    @location(1) color: vec3<f32>,
};
```

Location indices correspond to buffer slots: `@location(0)` binds to slot 0, `@location(1)` to slot 1, etc.

### Shader-Mesh Binding

`Shader::BindMeshBuffers()` matches shader locations to mesh channel buffers by iterating through the shader's `VertexLayout` in order. The shader's layout determines slot assignments, not the mesh's buffer order.

**Example:**
```cpp
// Shader layout order: POSITION (slot 0), COLOR (slot 1)
shader->layout.add(VertexSemantic::POSITION, FLOAT3);
shader->layout.add(VertexSemantic::COLOR, FLOAT3);

// BindMeshBuffers binds mesh buffers in this order
Shader::BindMeshBuffers(pass, mesh, shader);
```

