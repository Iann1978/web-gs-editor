#pragma once

#include <webgpu/webgpu_cpp.h>
#include "VertexAttribute.h"
#include "WebGPUContext.h"

class Mesh;

class Shader {
public:
    VertexLayout layout;
    wgpu::RenderPipeline pipeline;

    Shader() = default;
    ~Shader() = default;

    // Get the render pipeline for use in rendering
    wgpu::RenderPipeline GetPipeline() const { return pipeline; }
    
    // Bind mesh buffers to render pass
    // Uses the shader's layout order to determine correct slot assignments
    static void BindMeshBuffers(wgpu::RenderPassEncoder& pass, Mesh* mesh, const Shader* shader);

    // Static predefined shader
    static Shader* triangle;
    static Shader* vertexcolor2d;
    static void buildPredefined();
};

