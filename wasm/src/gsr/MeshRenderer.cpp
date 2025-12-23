#include "MeshRenderer.h"
#include "VertexAttribute.h"
#include <iostream>

MeshRenderer::MeshRenderer(Mesh* mesh, Shader* shader)
    : mesh(mesh), shader(shader) {
}

void MeshRenderer::BindBuffers(wgpu::RenderPassEncoder& pass) {
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

void MeshRenderer::Render(wgpu::RenderPassEncoder& pass) {
    if (!mesh || !shader) {
        std::cerr << "MeshRenderer::Render: mesh or shader is null" << std::endl;
        return;
    }
    
    // Set the pipeline
    pass.SetPipeline(shader->GetPipeline());
    
    // Bind uniforms (bind group 0 by default, can be extended for multiple groups)
    if (!shader->bindings.empty()) {
        shader->bindUniforms(pass, 0);
    }
    
    // Bind mesh buffers to the render pass
    BindBuffers(pass);
    
    // Draw the mesh
    if (mesh->indices) {
        // Use indexed drawing if indices are available
        pass.DrawIndexed(mesh->indexCount);
    } else {
        // Use non-indexed drawing
        pass.Draw(mesh->vertexCount);
    }
}

