#include "Shader.h"
#include <iostream>

// Initialize static member
Shader* Shader::triangle = nullptr;

void Shader::buildPredefined() {
    std::cout << "Shader.buildPredefined" << std::endl;

    if (Shader::triangle == nullptr) {
        // Triangle shader code from main.cpp
        const char triangleShaderCode[] = R"(
            @vertex fn vertexMain(@builtin(vertex_index) i : u32) ->
              @builtin(position) vec4f {
                const pos = array(vec2f(0, 1), vec2f(-1, -1), vec2f(1, -1));
                return vec4f(pos[i], 0, 1);
            }
            @fragment fn fragmentMain() -> @location(0) vec4f {
                return vec4f(1, 1, 0, 1);
            }
        )";

        // Get the WebGPU context
        WebGPUContext& context = WebGPUContext::Ref();

        // Create shader module
        wgpu::ShaderModule shaderModule = context.CreateShaderModule(triangleShaderCode);

        // Create render pipeline
        wgpu::RenderPipeline pipeline = context.CreateRenderPipeline(shaderModule, context.format);

        // Create shader object
        Shader::triangle = new Shader();
        Shader::triangle->layout = VertexLayout();  // Empty layout - triangle shader uses builtin vertex_index
        Shader::triangle->pipeline = pipeline;
    }
}

