cmake_minimum_required(VERSION 3.20)
project(gs_editor_wasm)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emscripten-specific settings
if(EMSCRIPTEN)
    # Output directory for compiled files
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/pkg)
    
    # Compiler flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++17")
    
    # Linker flags for Emscripten
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s MODULARIZE=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s EXPORT_ES6=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_ES6_IMPORT_META=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s EXPORTED_RUNTIME_METHODS=[\"ccall\",\"cwrap\"]")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s INITIAL_MEMORY=16777216")
    
    # WebGPU API support (for future use)
    # Note: WebGPU support in Emscripten requires specific flags
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_WEBGPU=1")
endif()

# Source files
set(SOURCES
    src/test.cpp
)

# Create executable
add_executable(test ${SOURCES})

# Set output name
set_target_properties(test PROPERTIES
    OUTPUT_NAME "test"
    SUFFIX ".js"
)

