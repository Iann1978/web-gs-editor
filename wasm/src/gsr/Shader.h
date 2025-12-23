#pragma once

#include <webgpu/webgpu_cpp.h>
#include "VertexAttribute.h"
#include "WebGPUContext.h"
#include "ShaderBinding.h"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <string>

class Shader {
public:
    VertexLayout layout;
    wgpu::RenderPipeline pipeline;

    Shader() = default;
    ~Shader() = default;

    // Get the render pipeline for use in rendering
    wgpu::RenderPipeline GetPipeline() const { return pipeline; }

    // Binding information
    std::vector<ShaderBinding> bindings;
    std::unordered_map<std::string, ShaderBinding*> bindingMap;  // Fast lookup by name
    
    // Uniform buffer management
    std::unordered_map<std::string, UniformBuffer> uniformBuffers;
    
    // Pipeline layout and bind groups
    wgpu::PipelineLayout pipelineLayout;
    std::vector<wgpu::BindGroupLayout> bindGroupLayouts;
    std::unordered_map<uint32_t, wgpu::BindGroup> bindGroups;  // Map group index to bind group

    // Methods
    void parseBindings(const std::string& wgslSource);  // Parse shader source
    ShaderBinding* getBinding(const std::string& name);  // Lookup binding
    UniformBuffer* getUniformBuffer(const std::string& name);  // Get/create uniform buffer
    
    // Set uniform values (convenience methods)
    void setUniform(const std::string& bufferName, const std::string& memberName, float value);
    void setUniform(const std::string& bufferName, const std::string& memberName, const glm::vec2& value);
    void setUniform(const std::string& bufferName, const std::string& memberName, const glm::vec3& value);
    void setUniform(const std::string& bufferName, const std::string& memberName, const glm::vec4& value);
    void setUniform(const std::string& bufferName, const std::string& memberName, const glm::mat3& value);
    void setUniform(const std::string& bufferName, const std::string& memberName, const glm::mat4& value);
    void setUniform(const std::string& bufferName, const std::string& memberName, int value);
    void setUniform(const std::string& bufferName, const std::string& memberName, uint32_t value);
    void setUniform(const std::string& bufferName, const std::string& memberName, bool value);
    
    // Upload uniform buffers to GPU
    void uploadUniformBuffers();
    
    // Create bind group layout from parsed bindings
    wgpu::BindGroupLayout createBindGroupLayout(wgpu::Device& device, uint32_t groupIndex);
    wgpu::PipelineLayout createPipelineLayout(wgpu::Device& device);
    
    // Create bind group with buffers
    wgpu::BindGroup createBindGroup(wgpu::Device& device, uint32_t groupIndex);
    
    // Bind uniforms to render pass
    void bindUniforms(wgpu::RenderPassEncoder& pass, uint32_t groupIndex);

    // Static predefined shader
    static Shader* triangle;
    static Shader* vertexcolor2d;
    static Shader* vertexcolor;
    static void buildPredefined();
};

