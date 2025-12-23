#pragma once

#include <webgpu/webgpu_cpp.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

struct ShaderBindingMember {
    enum class Type {
        Unknown = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        UInt,
        Bool,
    };
    
    Type type = Type::Unknown;
    std::string name;
    size_t offset;  // Byte offset in uniform buffer
    size_t size;    // Size in bytes
};

struct ShaderBinding {
    enum class Type {
        Unknown = 0,
        UniformBuffer,
        StorageBuffer,
        Texture,
        StorageTexture,
        Sampler,
    };
    
    Type type = Type::Unknown;
    uint32_t group;      // @group(N)
    uint32_t binding;    // @binding(N)
    size_t size;         // Buffer size in bytes (for buffers)
    std::string name;    // Variable name (e.g., "u_uniforms")
    wgpu::ShaderStage visibility;  // Vertex, Fragment, or both
    std::vector<ShaderBindingMember> members;  // For uniform buffers
};

struct UniformBuffer {
    std::string name;
    size_t size;
    wgpu::Buffer buffer;
    wgpu::BufferUsage usage = wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst;
    std::vector<uint8_t> data;  // CPU-side data cache
    bool dirty = false;  // Track if data needs to be uploaded
};

