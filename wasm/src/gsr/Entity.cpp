#include "Entity.h"
#include "Transform.h"
#include <cstddef>
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

extern "C" {

#if defined(__EMSCRIPTEN__)
EMSCRIPTEN_KEEPALIVE
#endif
const char* gsr_entity_get_name(Entity* entity) {
    if (!entity) return "";
    return entity->getName().c_str();
}

#if defined(__EMSCRIPTEN__)
EMSCRIPTEN_KEEPALIVE
#endif
size_t gsr_entity_get_name_len(Entity* entity) {
    if (!entity) return 0;
    return entity->getName().size();
}

#if defined(__EMSCRIPTEN__)
EMSCRIPTEN_KEEPALIVE
#endif
void gsr_entity_get_transform(Entity* entity, float* outBuffer) {
    if (!entity || !outBuffer) {
        // Return default transform if entity is null
        outBuffer[0] = outBuffer[1] = outBuffer[2] = 0.0f; // position
        outBuffer[3] = outBuffer[4] = outBuffer[5] = 0.0f; // rotation xyz
        outBuffer[6] = 1.0f; // rotation w
        outBuffer[7] = outBuffer[8] = outBuffer[9] = 1.0f; // scale
        return;
    }
    
    Transform& transform = entity->getTransform();
    glm::vec3 pos = transform.getPosition();
    glm::quat rot = transform.getRotation();
    glm::vec3 scale = transform.getScale();
    
    // Write position (3 floats)
    outBuffer[0] = pos.x;
    outBuffer[1] = pos.y;
    outBuffer[2] = pos.z;
    
    // Write rotation quaternion (4 floats)
    outBuffer[3] = rot.x;
    outBuffer[4] = rot.y;
    outBuffer[5] = rot.z;
    outBuffer[6] = rot.w;
    
    // Write scale (3 floats)
    outBuffer[7] = scale.x;
    outBuffer[8] = scale.y;
    outBuffer[9] = scale.z;
}
}
