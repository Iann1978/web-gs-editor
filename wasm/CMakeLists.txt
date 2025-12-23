cmake_minimum_required(VERSION 3.22)
project(gs_editor_wasm)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add GLM from git submodule
add_subdirectory(3dr/glm)

# Add Dawn subdirectory with dependency fetching
set(DAWN_FETCH_DEPENDENCIES ON)
add_subdirectory("dawn" EXCLUDE_FROM_ALL)

# Source files for app (WebGPU)
set(APP_SOURCES
    src/gsr/main.cpp
    src/gsr/WebGPUContext.cpp
    src/gsr/VertexAttribute.cpp
    src/gsr/Shader.cpp
    src/gsr/Mesh.cpp
    src/gsr/MeshRenderer.cpp
    src/gsr/Transform.cpp
    src/gsr/Camera.cpp
)

# Source files for test (simple WASM module)
set(TEST_SOURCES
    src/test.cpp
)

# Create app executable (WebGPU)
add_executable(app ${APP_SOURCES})

# Create test executable (simple WASM module)
add_executable(test ${TEST_SOURCES})

# Emscripten-specific settings
if(EMSCRIPTEN)
    # Output directory for compiled files
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/pkg)
    
    # Configure app target (non-modularized for direct script loading)
    set_target_properties(app PROPERTIES SUFFIX ".js")
    target_link_libraries(app PRIVATE emdawnwebgpu_cpp webgpu_glfw glm::glm)
    target_link_options(app PRIVATE 
        "-sASYNCIFY=1" 
        "-sUSE_GLFW=3"
        # Export Init(), Start(), and IsWebGPUInitialized() functions for manual calling
        "-sEXPORTED_FUNCTIONS=[\"_InitWebGPU\",\"_IsWebGPUInitialized\",\"_StartWebGPU\"]"
        # Export runtime methods for calling functions
        "-sEXPORTED_RUNTIME_METHODS=[\"ccall\",\"cwrap\"]"
    )
    
    # Configure test target (modularized ES6 module)
    set_target_properties(test PROPERTIES SUFFIX ".js")
    target_link_options(test PRIVATE 
        "-sMODULARIZE=1"
        "-sEXPORT_ES6=1"
        "-sUSE_ES6_IMPORT_META=1"
        "-sEXPORTED_RUNTIME_METHODS=[\"ccall\",\"cwrap\"]"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sINITIAL_MEMORY=16777216"
        "-lembind"
    )
else()
    # For native builds, link Dawn and GLFW for app
    target_link_libraries(app PRIVATE webgpu_dawn webgpu_glfw glfw glm::glm)
    # Test target doesn't need any special libraries for native builds
endif()

