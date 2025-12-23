#pragma once

#include "Entity.h"
#include <webgpu/webgpu_cpp.h>
#include <vector>

class Scene {
public:
    Scene();
    ~Scene();
    
    // Singleton access
    static Scene* ins;
    
    // Core lifecycle
    void initialize();
    void update(float deltaTime);
    void render(wgpu::RenderPassEncoder& pass);
    void cleanup();
    
    // Entity management
    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);
    void removeEntity(size_t index);
    void destroyAllEntities();
    
    // Entity queries
    size_t getEntityCount() const { return entities.size(); }
    Entity* getEntity(size_t index) const;

private:
    std::vector<Entity*> entities;
};

// Expose the Scene singleton pointer to JS/WASM side.
extern "C" Scene* gsr_get_scene();
extern "C" size_t gsr_scene_get_entity_count(Scene* scene);
extern "C" Entity* gsr_scene_get_entity(Scene* scene, size_t index);

