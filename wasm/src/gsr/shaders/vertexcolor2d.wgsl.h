#pragma once

// Ported from vertexcolor2d.vert.glsl and vertexcolor2d.frag.glsl
// Adapted to use vec3 position (from Mesh::triangle) instead of vec2
// Using NDC coordinates directly (no uniform needed)
constexpr const char* vertexcolor2d_shader_code = R"(
    struct VertexInput {
        @location(0) position: vec3<f32>,
        @location(1) color: vec3<f32>,
    };
    
    struct VertexOutput {
        @builtin(position) position: vec4<f32>,
        @location(0) color: vec3<f32>,
    };
    
    @vertex
    fn vertexMain(input: VertexInput) -> VertexOutput {
        var output: VertexOutput;
        // Use xy components from vec3 position (z is ignored for 2D)
        // Position is already in NDC coordinates from Mesh::triangle
        output.position = vec4<f32>(input.position.xy, 0.0, 1.0);
        output.color = input.color;
        return output;
    }
    
    @fragment
    fn fragmentMain(input: VertexOutput) -> @location(0) vec4<f32> {
        return vec4<f32>(input.color, 1.0);
    }
)";

