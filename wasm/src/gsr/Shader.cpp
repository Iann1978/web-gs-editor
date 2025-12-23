#include "Shader.h"
#include "shaders/triangle.wgsl.h"
#include "shaders/vertexcolor2d.wgsl.h"
#include <iostream>
#include <vector>

// Initialize static members
Shader* Shader::triangle = nullptr;
Shader* Shader::vertexcolor2d = nullptr;

void Shader::buildPredefined() {
    std::cout << "Shader.buildPredefined" << std::endl;

    if (Shader::triangle == nullptr) {
        // Get the WebGPU context
        WebGPUContext& context = WebGPUContext::Ref();

        // Create shader module
        wgpu::ShaderModule shaderModule = context.CreateShaderModule(triangle_shader_code);

        // Create render pipeline
        wgpu::RenderPipeline pipeline = context.CreateRenderPipeline(shaderModule, context.format);

        // Create shader object
        Shader::triangle = new Shader();
        Shader::triangle->layout = VertexLayout();  // Empty layout - triangle shader uses builtin vertex_index
        Shader::triangle->pipeline = pipeline;
    }
    
    // Create vertexcolor2d shader
    if (Shader::vertexcolor2d == nullptr) {
        WebGPUContext& context = WebGPUContext::Ref();
        
        // Create shader module
        wgpu::ShaderModule shaderModule = context.CreateShaderModule(vertexcolor2d_shader_code);
        
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

