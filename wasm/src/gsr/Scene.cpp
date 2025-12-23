#include "Scene.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

Scene* Scene::ins = nullptr;

Scene::Scene() {
    assert(ins == nullptr);
    ins = this;
}

Scene::~Scene() {
    cleanup();
}

void Scene::initialize() {
    // Initialize scene resources if needed
}

void Scene::update(float deltaTime) {
    for (Entity* entity : entities) {
        if (entity) {
            entity->update(deltaTime);
        }
    }
}

void Scene::render(wgpu::RenderPassEncoder& pass) {
    for (Entity* entity : entities) {
        if (entity && entity->isVisible()) {
            entity->render(pass);
        }
    }
}

void Scene::cleanup() {
    destroyAllEntities();
}

void Scene::addEntity(Entity* entity) {
    if (entity) {
        entities.push_back(entity);
    }
}

void Scene::removeEntity(Entity* entity) {
    if (entity) {
        auto it = std::find(entities.begin(), entities.end(), entity);
        if (it != entities.end()) {
            (*it)->cleanup();
            delete *it;
            entities.erase(it);
        }
    }
}

void Scene::removeEntity(size_t index) {
    if (index < entities.size()) {
        Entity* entity = entities[index];
        if (entity) {
            entity->cleanup();
            delete entity;
        }
        entities.erase(entities.begin() + index);
    }
}

void Scene::destroyAllEntities() {
    for (Entity* entity : entities) {
        if (entity) {
            entity->cleanup();
            delete entity;
        }
    }
    entities.clear();
}

Entity* Scene::getEntity(size_t index) const {
    if (index < entities.size()) {
        return entities[index];
    }
    return nullptr;
}

extern "C" {
#if defined(__EMSCRIPTEN__)
EMSCRIPTEN_KEEPALIVE
#endif
Scene* get_scene() {
    return Scene::ins;
}

#if defined(__EMSCRIPTEN__)
EMSCRIPTEN_KEEPALIVE
#endif
size_t gsr_scene_get_entity_count(Scene* scene) {
    return scene->getEntityCount();
}

#if defined(__EMSCRIPTEN__)
EMSCRIPTEN_KEEPALIVE
#endif
Entity* gsr_scene_get_entity(Scene* scene, size_t index) {
    return scene->getEntity(index);
}
}

