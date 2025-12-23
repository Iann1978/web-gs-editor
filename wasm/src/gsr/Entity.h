#pragma once

#include <webgpu/webgpu_cpp.h>
#include <string>

class Transform;  // Forward declaration

class Entity {
public:
    virtual ~Entity() = default;
    
    // Core lifecycle methods
    virtual void render(wgpu::RenderPassEncoder& pass) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void cleanup() = 0;
    
    // Transform access
    virtual Transform& getTransform() = 0;
    
    // Name and visibility
    std::string getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }
    bool isVisible() const { return visible; }
    void setVisible(bool vis) { visible = vis; }

protected:
    std::string name = "Entity";
    bool visible = true;
};

// Expose selected Entity fields to JS/WASM side.
extern "C" const char* gsr_entity_get_name(Entity* entity);
extern "C" size_t gsr_entity_get_name_len(Entity* entity);
extern "C" void gsr_entity_get_transform(Entity* entity, float* outBuffer);

