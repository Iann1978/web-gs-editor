#include "Entity.h"
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
}
