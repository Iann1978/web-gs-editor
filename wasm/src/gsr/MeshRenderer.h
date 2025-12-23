#pragma once

#include <webgpu/webgpu_cpp.h>
#include "Mesh.h"
#include "Shader.h"

class Camera;  // Forward declaration

class MeshRenderer {
private:
    Mesh* mesh;
    Shader* shader;

public:
    MeshRenderer(Mesh* mesh, Shader* shader);
    
    // Render the mesh using the shader
    // Takes a RenderPassEncoder that should already be started
    // camera: optional camera for setting view/projection matrices
    void Render(wgpu::RenderPassEncoder& pass, Camera* camera = nullptr);
    
    // Bind mesh buffers to render pass
    // Uses the shader's layout order to determine correct slot assignments
    void BindBuffers(wgpu::RenderPassEncoder& pass);
    
    // Getters and setters
    Mesh* GetMesh() const { return mesh; }
    Shader* GetShader() const { return shader; }
    void SetMesh(Mesh* m) { mesh = m; }
    void SetShader(Shader* s) { shader = s; }
};

