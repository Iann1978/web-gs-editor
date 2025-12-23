#include "Shader.h"
#include "Mesh.h"
#include <iostream>
#include <vector>

// Initialize static members
Shader* Shader::triangle = nullptr;
Shader* Shader::vertexcolor2d = nullptr;

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
    
    // Create vertexcolor2d shader
    if (Shader::vertexcolor2d == nullptr) {
        // Ported from vertexcolor2d.vert.glsl and vertexcolor2d.frag.glsl
        // Adapted to use vec3 position (from Mesh::triangle) instead of vec2
        // Using NDC coordinates directly (no uniform needed)
        const char vertexcolor2dShaderCode[] = R"(
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
        
        WebGPUContext& context = WebGPUContext::Ref();
        
        // Create shader module
        wgpu::ShaderModule shaderModule = context.CreateShaderModule(vertexcolor2dShaderCode);
        
        // Create vertex buffer layouts for vec3 position and vec3 color
        // Use fixed slot numbers from VertexSemantic
        wgpu::VertexAttribute posAttr;
        posAttr.shaderLocation = VertexSemantic::getSlot(VertexSemantic::POSITION); // 0
        posAttr.offset = 0;
        posAttr.format = wgpu::VertexFormat::Float32x3;
        
        wgpu::VertexAttribute colorAttr;
        colorAttr.shaderLocation = VertexSemantic::getSlot(VertexSemantic::COLOR); // 1
        colorAttr.offset = 0;
        colorAttr.format = wgpu::VertexFormat::Float32x3;
        
        wgpu::VertexBufferLayout posLayout;
        posLayout.arrayStride = 12; // 3 floats * 4 bytes
        posLayout.stepMode = wgpu::VertexStepMode::Vertex;
        posLayout.attributeCount = 1;
        posLayout.attributes = &posAttr;
        
        wgpu::VertexBufferLayout colorLayout;
        colorLayout.arrayStride = 12; // 3 floats * 4 bytes
        colorLayout.stepMode = wgpu::VertexStepMode::Vertex;
        colorLayout.attributeCount = 1;
        colorLayout.attributes = &colorAttr;
        
        std::vector<wgpu::VertexBufferLayout> vertexLayouts;
        vertexLayouts.push_back(posLayout);
        vertexLayouts.push_back(colorLayout);
        
        // Create render pipeline
        wgpu::ColorTargetState colorTarget;
        colorTarget.format = context.format;
        
        wgpu::FragmentState fragmentState;
        fragmentState.module = shaderModule;
        fragmentState.targetCount = 1;
        fragmentState.targets = &colorTarget;
        
        wgpu::VertexState vertexState;
        vertexState.module = shaderModule;
        vertexState.entryPoint = "vertexMain";
        vertexState.bufferCount = static_cast<uint32_t>(vertexLayouts.size());
        vertexState.buffers = vertexLayouts.data();
        
        wgpu::RenderPipelineDescriptor pipelineDesc;
        pipelineDesc.vertex = vertexState;
        pipelineDesc.fragment = &fragmentState;
        
        wgpu::RenderPipeline pipeline = context.device.CreateRenderPipeline(&pipelineDesc);
        
        // Create shader object with proper layout
        Shader::vertexcolor2d = new Shader();
        Shader::vertexcolor2d->layout.add(VertexSemantic::POSITION, VertexAttributeType::TYPE::FLOAT3);
        Shader::vertexcolor2d->layout.add(VertexSemantic::COLOR, VertexAttributeType::TYPE::FLOAT3);
        Shader::vertexcolor2d->pipeline = pipeline;
    }
}

void Shader::BindMeshBuffers(wgpu::RenderPassEncoder& pass, Mesh* mesh, const Shader* shader) {
    if (!mesh || !shader) return;
    
    // Bind each channel buffer using the semantic's fixed slot number
    // This ensures correct binding regardless of map iteration order
    for (const auto& [semantic, attr] : shader->layout.getAttributes()) {
        uint32_t slot = VertexSemantic::getSlot(semantic);
        if (slot == UINT32_MAX) continue; // Skip invalid semantics
        
        wgpu::Buffer buffer = mesh->getBuffer(semantic);
        if (buffer) {
            pass.SetVertexBuffer(slot, buffer, 0, wgpu::kWholeSize);
        }
    }
    
    // Bind index buffer if present
    if (mesh->indices) {
        pass.SetIndexBuffer(mesh->indices, mesh->indexFormat, 0, wgpu::kWholeSize);
    }
}

