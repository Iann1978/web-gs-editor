#include "EntMesh.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <iostream>

EntMesh::EntMesh(Mesh* mesh, Shader* shader, const std::string& name)
    : mesh(mesh), shader(shader) {
    this->name = name;
}

EntMesh::~EntMesh() {
    cleanup();
}

void EntMesh::render(wgpu::RenderPassEncoder& pass) {
    if (!visible || !mesh || !shader) return;
    
    // Create temporary MeshRenderer for rendering
    MeshRenderer renderer(mesh, shader);
    
    // Get model matrix from transform
    glm::mat4 modelMatrix = transform.getMatrix();
    
    // Set model matrix if shader supports it
    if (shader->getBinding("u_uniforms")) {
        shader->setUniform("u_uniforms", "model", modelMatrix);
        
        // Set view and projection from main camera if available
        if (Camera::main) {
            shader->setUniform("u_uniforms", "view", Camera::main->getView());
            shader->setUniform("u_uniforms", "proj", Camera::main->getProjection());
        }
        
        shader->uploadUniformBuffers();
    }
    
    // Render using MeshRenderer
    renderer.Render(pass, Camera::main);
}

void EntMesh::update(float deltaTime) {
    // Mesh entities don't need per-frame updates by default
    // Can be overridden for animated meshes
}

void EntMesh::cleanup() {
    // Don't delete mesh/shader as they might be shared
    mesh = nullptr;
    shader = nullptr;
}

