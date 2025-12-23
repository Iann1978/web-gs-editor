#pragma once

#include "Entity.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"

class Camera;  // Forward declaration

class EntMesh : public Entity {
public:
    EntMesh(Mesh* mesh, Shader* shader, const std::string& name = "Mesh Entity");
    virtual ~EntMesh();
    
    // Entity interface
    void render(wgpu::RenderPassEncoder& pass) override;
    void update(float deltaTime) override;
    void cleanup() override;
    Transform& getTransform() override { return transform; }
    
    // Mesh-specific accessors
    Mesh* getMesh() const { return mesh; }
    void setMesh(Mesh* m) { mesh = m; }
    Shader* getShader() const { return shader; }
    void setShader(Shader* s) { shader = s; }

private:
    Mesh* mesh;
    Shader* shader;
    Transform transform;
};

