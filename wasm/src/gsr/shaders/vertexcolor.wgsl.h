#pragma once

// Ported from vertexcolor.vert.glsl and vertexcolor.frag.glsl
// 3D vertex color shader with model/view/projection matrix transformations
constexpr const char* vertexcolor_shader_code = R"(
    // Uniform buffer structure for transformation matrices
    struct Uniforms {
        model: mat4x4<f32>,
        view: mat4x4<f32>,
        proj: mat4x4<f32>,
    };
    
    @group(0) @binding(0) var<uniform> u_uniforms: Uniforms;
    
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
        // Transform position: proj * view * model * vec4(position, 1.0)
        output.position = u_uniforms.proj * u_uniforms.view * u_uniforms.model * vec4<f32>(input.position, 1.0);
        output.color = input.color;
        return output;
    }
    
    @fragment
    fn fragmentMain(input: VertexOutput) -> @location(0) vec4<f32> {
        return vec4<f32>(input.color, 1.0);
    }
)";

