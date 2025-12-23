#pragma once

#include <webgpu/webgpu_cpp.h>
#include "VertexAttribute.h"
#include "WebGPUContext.h"

class Shader {
public:
    VertexLayout layout;
    wgpu::RenderPipeline pipeline;

    Shader() = default;
    ~Shader() = default;

    // Get the render pipeline for use in rendering
    wgpu::RenderPipeline GetPipeline() const { return pipeline; }

    // Static predefined shader
    static Shader* triangle;
    static void buildPredefined();
};

