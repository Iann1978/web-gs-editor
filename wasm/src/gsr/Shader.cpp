#include "Shader.h"
#include "ShaderParser.h"
#include "shaders/triangle.wgsl.h"
#include "shaders/vertexcolor2d.wgsl.h"
#include "shaders/vertexcolor.wgsl.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cstring>

// Initialize static members
Shader* Shader::triangle = nullptr;
Shader* Shader::vertexcolor2d = nullptr;
Shader* Shader::vertexcolor = nullptr;

void Shader::buildPredefined() {
    std::cout << "Shader.buildPredefined" << std::endl;

    if (Shader::triangle == nullptr) {
        // Get the WebGPU context
        WebGPUContext& context = WebGPUContext::Ref();

        // Create shader module
        wgpu::ShaderModule shaderModule = context.CreateShaderModule(triangle_shader_code);

        // Create render pipeline
        wgpu::RenderPipeline pipeline = context.CreateRenderPipeline(shaderModule, context.format);

        // Create shader object
        Shader::triangle = new Shader();
        Shader::triangle->layout = VertexLayout();  // Empty layout - triangle shader uses builtin vertex_index
        Shader::triangle->pipeline = pipeline;
    }
    
    // Create vertexcolor2d shader
    if (Shader::vertexcolor2d == nullptr) {
        WebGPUContext& context = WebGPUContext::Ref();
        
        // Create shader module
        wgpu::ShaderModule shaderModule = context.CreateShaderModule(vertexcolor2d_shader_code);
        
        // Create vertex buffer layouts for vec3 position and vec3 color
        // Use fixed slot numbers from VertexSemantic
        wgpu::VertexAttribute posAttr;
        posAttr.shaderLocation = VertexSemantic::getSlot(VertexSemantic::POSITION); // 0
        posAttr.offset = 0;
        posAttr.format = wgpu::VertexFormat::Float32x3;
        
        wgpu::VertexAttribute colorAttr;
        colorAttr.shaderLocation = VertexSemantic::getSlot(VertexSemantic::COLOR); // 1
        colorAttr.offset = 0;
        colorAttr.format = wgpu::VertexFormat::Float32x3;
        
        wgpu::VertexBufferLayout posLayout;
        posLayout.arrayStride = 12; // 3 floats * 4 bytes
        posLayout.stepMode = wgpu::VertexStepMode::Vertex;
        posLayout.attributeCount = 1;
        posLayout.attributes = &posAttr;
        
        wgpu::VertexBufferLayout colorLayout;
        colorLayout.arrayStride = 12; // 3 floats * 4 bytes
        colorLayout.stepMode = wgpu::VertexStepMode::Vertex;
        colorLayout.attributeCount = 1;
        colorLayout.attributes = &colorAttr;
        
        std::vector<wgpu::VertexBufferLayout> vertexLayouts;
        vertexLayouts.push_back(posLayout);
        vertexLayouts.push_back(colorLayout);
        
        // Create render pipeline
        wgpu::ColorTargetState colorTarget;
        colorTarget.format = context.format;
        
        wgpu::FragmentState fragmentState;
        fragmentState.module = shaderModule;
        fragmentState.targetCount = 1;
        fragmentState.targets = &colorTarget;
        
        wgpu::VertexState vertexState;
        vertexState.module = shaderModule;
        vertexState.entryPoint = "vertexMain";
        vertexState.bufferCount = static_cast<uint32_t>(vertexLayouts.size());
        vertexState.buffers = vertexLayouts.data();
        
        wgpu::RenderPipelineDescriptor pipelineDesc;
        pipelineDesc.vertex = vertexState;
        pipelineDesc.fragment = &fragmentState;
        
        wgpu::RenderPipeline pipeline = context.device.CreateRenderPipeline(&pipelineDesc);
        
        // Create shader object with proper layout
        Shader::vertexcolor2d = new Shader();
        Shader::vertexcolor2d->layout.add(VertexSemantic::POSITION, VertexAttributeType::TYPE::FLOAT3);
        Shader::vertexcolor2d->layout.add(VertexSemantic::COLOR, VertexAttributeType::TYPE::FLOAT3);
        Shader::vertexcolor2d->pipeline = pipeline;
    }
    
    // Create vertexcolor shader (3D with MVP matrices)
    if (Shader::vertexcolor == nullptr) {
        WebGPUContext& context = WebGPUContext::Ref();
        
        // Create shader module
        wgpu::ShaderModule shaderModule = context.CreateShaderModule(vertexcolor_shader_code);
        
        // Parse bindings from shader source
        Shader::vertexcolor = new Shader();
        Shader::vertexcolor->parseBindings(vertexcolor_shader_code);
        
        // Create bind group layouts and pipeline layout if we have bindings
        if (!Shader::vertexcolor->bindings.empty()) {
            Shader::vertexcolor->pipelineLayout = Shader::vertexcolor->createPipelineLayout(context.device);
        }
        
        // Create vertex buffer layouts for vec3 position and vec3 color (same as vertexcolor2d)
        wgpu::VertexAttribute posAttr;
        posAttr.shaderLocation = VertexSemantic::getSlot(VertexSemantic::POSITION); // 0
        posAttr.offset = 0;
        posAttr.format = wgpu::VertexFormat::Float32x3;
        
        wgpu::VertexAttribute colorAttr;
        colorAttr.shaderLocation = VertexSemantic::getSlot(VertexSemantic::COLOR); // 1
        colorAttr.offset = 0;
        colorAttr.format = wgpu::VertexFormat::Float32x3;
        
        wgpu::VertexBufferLayout posLayout;
        posLayout.arrayStride = 12; // 3 floats * 4 bytes
        posLayout.stepMode = wgpu::VertexStepMode::Vertex;
        posLayout.attributeCount = 1;
        posLayout.attributes = &posAttr;
        
        wgpu::VertexBufferLayout colorLayout;
        colorLayout.arrayStride = 12; // 3 floats * 4 bytes
        colorLayout.stepMode = wgpu::VertexStepMode::Vertex;
        colorLayout.attributeCount = 1;
        colorLayout.attributes = &colorAttr;
        
        std::vector<wgpu::VertexBufferLayout> vertexLayouts;
        vertexLayouts.push_back(posLayout);
        vertexLayouts.push_back(colorLayout);
        
        // Create render pipeline
        wgpu::ColorTargetState colorTarget;
        colorTarget.format = context.format;
        
        wgpu::FragmentState fragmentState;
        fragmentState.module = shaderModule;
        fragmentState.entryPoint = "fragmentMain";
        fragmentState.targetCount = 1;
        fragmentState.targets = &colorTarget;
        
        wgpu::VertexState vertexState;
        vertexState.module = shaderModule;
        vertexState.entryPoint = "vertexMain";
        vertexState.bufferCount = static_cast<uint32_t>(vertexLayouts.size());
        vertexState.buffers = vertexLayouts.data();
        
        wgpu::RenderPipelineDescriptor pipelineDesc;
        pipelineDesc.vertex = vertexState;
        pipelineDesc.fragment = &fragmentState;
        
        // Set pipeline layout if we have bindings
        if (!Shader::vertexcolor->bindings.empty()) {
            pipelineDesc.layout = Shader::vertexcolor->pipelineLayout;
        }
        
        wgpu::RenderPipeline pipeline = context.device.CreateRenderPipeline(&pipelineDesc);
        
        // Set layout and pipeline
        Shader::vertexcolor->layout.add(VertexSemantic::POSITION, VertexAttributeType::TYPE::FLOAT3);
        Shader::vertexcolor->layout.add(VertexSemantic::COLOR, VertexAttributeType::TYPE::FLOAT3);
        Shader::vertexcolor->pipeline = pipeline;
        
        // Initialize uniform buffers
        for (const auto& binding : Shader::vertexcolor->bindings) {
            if (binding.type == ShaderBinding::Type::UniformBuffer) {
                UniformBuffer* ub = Shader::vertexcolor->getUniformBuffer(binding.name);
                if (ub) {
                    // Buffer will be created lazily when first uniform is set
                }
            }
        }
    }
}

void Shader::parseBindings(const std::string& wgslSource) {
    bindings = ShaderParser::parseBindings(wgslSource);
    
    // Build lookup map
    bindingMap.clear();
    for (auto& binding : bindings) {
        bindingMap[binding.name] = &binding;
    }
}

ShaderBinding* Shader::getBinding(const std::string& name) {
    auto it = bindingMap.find(name);
    if (it != bindingMap.end()) {
        return it->second;
    }
    return nullptr;
}

UniformBuffer* Shader::getUniformBuffer(const std::string& name) {
    auto it = uniformBuffers.find(name);
    if (it != uniformBuffers.end()) {
        return &it->second;
    }
    
    // Find binding to get size
    ShaderBinding* binding = getBinding(name);
    if (!binding || binding->type != ShaderBinding::Type::UniformBuffer) {
        std::cerr << "Shader::getUniformBuffer: Binding '" << name << "' not found or not a uniform buffer" << std::endl;
        return nullptr;
    }
    
    // Create uniform buffer
    UniformBuffer ub;
    ub.name = name;
    ub.size = binding->size;
    ub.data.resize(binding->size, 0);
    ub.dirty = false;
    
    uniformBuffers[name] = ub;
    return &uniformBuffers[name];
}

void Shader::setUniform(const std::string& bufferName, const std::string& memberName, float value) {
    UniformBuffer* ub = getUniformBuffer(bufferName);
    if (!ub) return;
    
    ShaderBinding* binding = getBinding(bufferName);
    if (!binding) return;
    
    // Find member
    ShaderBindingMember* member = nullptr;
    for (auto& m : binding->members) {
        if (m.name == memberName) {
            member = &m;
            break;
        }
    }
    
    if (!member || member->type != ShaderBindingMember::Type::Float) {
        std::cerr << "Shader::setUniform: Member '" << memberName << "' not found or wrong type in buffer '" << bufferName << "'" << std::endl;
        return;
    }
    
    memcpy(ub->data.data() + member->offset, &value, sizeof(float));
    ub->dirty = true;
}

void Shader::setUniform(const std::string& bufferName, const std::string& memberName, const glm::vec2& value) {
    UniformBuffer* ub = getUniformBuffer(bufferName);
    if (!ub) return;
    
    ShaderBinding* binding = getBinding(bufferName);
    if (!binding) return;
    
    ShaderBindingMember* member = nullptr;
    for (auto& m : binding->members) {
        if (m.name == memberName) {
            member = &m;
            break;
        }
    }
    
    if (!member || member->type != ShaderBindingMember::Type::Float2) {
        std::cerr << "Shader::setUniform: Member '" << memberName << "' not found or wrong type in buffer '" << bufferName << "'" << std::endl;
        return;
    }
    
    memcpy(ub->data.data() + member->offset, glm::value_ptr(value), sizeof(glm::vec2));
    ub->dirty = true;
}

void Shader::setUniform(const std::string& bufferName, const std::string& memberName, const glm::vec3& value) {
    UniformBuffer* ub = getUniformBuffer(bufferName);
    if (!ub) return;
    
    ShaderBinding* binding = getBinding(bufferName);
    if (!binding) return;
    
    ShaderBindingMember* member = nullptr;
    for (auto& m : binding->members) {
        if (m.name == memberName) {
            member = &m;
            break;
        }
    }
    
    if (!member || member->type != ShaderBindingMember::Type::Float3) {
        std::cerr << "Shader::setUniform: Member '" << memberName << "' not found or wrong type in buffer '" << bufferName << "'" << std::endl;
        return;
    }
    
    memcpy(ub->data.data() + member->offset, glm::value_ptr(value), sizeof(glm::vec3));
    ub->dirty = true;
}

void Shader::setUniform(const std::string& bufferName, const std::string& memberName, const glm::vec4& value) {
    UniformBuffer* ub = getUniformBuffer(bufferName);
    if (!ub) return;
    
    ShaderBinding* binding = getBinding(bufferName);
    if (!binding) return;
    
    ShaderBindingMember* member = nullptr;
    for (auto& m : binding->members) {
        if (m.name == memberName) {
            member = &m;
            break;
        }
    }
    
    if (!member || member->type != ShaderBindingMember::Type::Float4) {
        std::cerr << "Shader::setUniform: Member '" << memberName << "' not found or wrong type in buffer '" << bufferName << "'" << std::endl;
        return;
    }
    
    memcpy(ub->data.data() + member->offset, glm::value_ptr(value), sizeof(glm::vec4));
    ub->dirty = true;
}

void Shader::setUniform(const std::string& bufferName, const std::string& memberName, const glm::mat3& value) {
    UniformBuffer* ub = getUniformBuffer(bufferName);
    if (!ub) return;
    
    ShaderBinding* binding = getBinding(bufferName);
    if (!binding) return;
    
    ShaderBindingMember* member = nullptr;
    for (auto& m : binding->members) {
        if (m.name == memberName) {
            member = &m;
            break;
        }
    }
    
    if (!member || member->type != ShaderBindingMember::Type::Mat3) {
        std::cerr << "Shader::setUniform: Member '" << memberName << "' not found or wrong type in buffer '" << bufferName << "'" << std::endl;
        return;
    }
    
    memcpy(ub->data.data() + member->offset, glm::value_ptr(value), sizeof(glm::mat3));
    ub->dirty = true;
}

void Shader::setUniform(const std::string& bufferName, const std::string& memberName, const glm::mat4& value) {
    UniformBuffer* ub = getUniformBuffer(bufferName);
    if (!ub) return;
    
    ShaderBinding* binding = getBinding(bufferName);
    if (!binding) return;
    
    ShaderBindingMember* member = nullptr;
    for (auto& m : binding->members) {
        if (m.name == memberName) {
            member = &m;
            break;
        }
    }
    
    if (!member || member->type != ShaderBindingMember::Type::Mat4) {
        std::cerr << "Shader::setUniform: Member '" << memberName << "' not found or wrong type in buffer '" << bufferName << "'" << std::endl;
        return;
    }
    
    memcpy(ub->data.data() + member->offset, glm::value_ptr(value), sizeof(glm::mat4));
    ub->dirty = true;
}

void Shader::setUniform(const std::string& bufferName, const std::string& memberName, int value) {
    UniformBuffer* ub = getUniformBuffer(bufferName);
    if (!ub) return;
    
    ShaderBinding* binding = getBinding(bufferName);
    if (!binding) return;
    
    ShaderBindingMember* member = nullptr;
    for (auto& m : binding->members) {
        if (m.name == memberName) {
            member = &m;
            break;
        }
    }
    
    if (!member || member->type != ShaderBindingMember::Type::Int) {
        std::cerr << "Shader::setUniform: Member '" << memberName << "' not found or wrong type in buffer '" << bufferName << "'" << std::endl;
        return;
    }
    
    memcpy(ub->data.data() + member->offset, &value, sizeof(int));
    ub->dirty = true;
}

void Shader::setUniform(const std::string& bufferName, const std::string& memberName, uint32_t value) {
    UniformBuffer* ub = getUniformBuffer(bufferName);
    if (!ub) return;
    
    ShaderBinding* binding = getBinding(bufferName);
    if (!binding) return;
    
    ShaderBindingMember* member = nullptr;
    for (auto& m : binding->members) {
        if (m.name == memberName) {
            member = &m;
            break;
        }
    }
    
    if (!member || member->type != ShaderBindingMember::Type::UInt) {
        std::cerr << "Shader::setUniform: Member '" << memberName << "' not found or wrong type in buffer '" << bufferName << "'" << std::endl;
        return;
    }
    
    memcpy(ub->data.data() + member->offset, &value, sizeof(uint32_t));
    ub->dirty = true;
}

void Shader::setUniform(const std::string& bufferName, const std::string& memberName, bool value) {
    UniformBuffer* ub = getUniformBuffer(bufferName);
    if (!ub) return;
    
    ShaderBinding* binding = getBinding(bufferName);
    if (!binding) return;
    
    ShaderBindingMember* member = nullptr;
    for (auto& m : binding->members) {
        if (m.name == memberName) {
            member = &m;
            break;
        }
    }
    
    if (!member || member->type != ShaderBindingMember::Type::Bool) {
        std::cerr << "Shader::setUniform: Member '" << memberName << "' not found or wrong type in buffer '" << bufferName << "'" << std::endl;
        return;
    }
    
    uint32_t boolValue = value ? 1 : 0;
    memcpy(ub->data.data() + member->offset, &boolValue, sizeof(uint32_t));
    ub->dirty = true;
}

void Shader::uploadUniformBuffers() {
    WebGPUContext& context = WebGPUContext::Ref();
    
    for (auto& [name, uniformBuffer] : uniformBuffers) {
        if (!uniformBuffer.dirty) continue;
        
        // Create buffer if needed
        if (!uniformBuffer.buffer) {
            wgpu::BufferDescriptor desc;
            desc.size = uniformBuffer.size;
            desc.usage = uniformBuffer.usage;
            desc.mappedAtCreation = false;
            uniformBuffer.buffer = context.device.CreateBuffer(&desc);
        }
        
        // Upload data
        context.queue.WriteBuffer(uniformBuffer.buffer, 0, 
                                 uniformBuffer.data.data(), 
                                 uniformBuffer.size);
        uniformBuffer.dirty = false;
    }
}

wgpu::BindGroupLayout Shader::createBindGroupLayout(wgpu::Device& device, uint32_t groupIndex) {
    std::vector<wgpu::BindGroupLayoutEntry> entries;
    
    // Find all bindings for this group
    for (const auto& binding : bindings) {
        if (binding.group != groupIndex) continue;
        
        wgpu::BindGroupLayoutEntry entry;
        entry.binding = binding.binding;
        entry.visibility = binding.visibility;
        
        switch (binding.type) {
            case ShaderBinding::Type::UniformBuffer: {
                wgpu::BufferBindingLayout bufferLayout;
                bufferLayout.type = wgpu::BufferBindingType::Uniform;
                bufferLayout.hasDynamicOffset = false;
                bufferLayout.minBindingSize = binding.size;
                entry.buffer = bufferLayout;
                break;
            }
            case ShaderBinding::Type::StorageBuffer: {
                wgpu::BufferBindingLayout bufferLayout;
                bufferLayout.type = wgpu::BufferBindingType::Storage;
                bufferLayout.hasDynamicOffset = false;
                bufferLayout.minBindingSize = binding.size;
                entry.buffer = bufferLayout;
                break;
            }
            // TODO: Add texture, sampler, storage texture support
            default:
                std::cerr << "Shader::createBindGroupLayout: Unsupported binding type for group " << groupIndex << std::endl;
                continue;
        }
        
        entries.push_back(entry);
    }
    
    if (entries.empty()) {
        // Return empty layout if no bindings
        wgpu::BindGroupLayoutDescriptor desc;
        desc.entryCount = 0;
        desc.entries = nullptr;
        return device.CreateBindGroupLayout(&desc);
    }
    
    wgpu::BindGroupLayoutDescriptor desc;
    desc.entryCount = static_cast<uint32_t>(entries.size());
    desc.entries = entries.data();
    
    return device.CreateBindGroupLayout(&desc);
}

wgpu::PipelineLayout Shader::createPipelineLayout(wgpu::Device& device) {
    // Find maximum group index
    uint32_t maxGroup = 0;
    for (const auto& binding : bindings) {
        if (binding.group > maxGroup) {
            maxGroup = binding.group;
        }
    }
    
    // Create bind group layouts for each group
    bindGroupLayouts.clear();
    bindGroupLayouts.resize(maxGroup + 1);
    
    for (uint32_t i = 0; i <= maxGroup; ++i) {
        bindGroupLayouts[i] = createBindGroupLayout(device, i);
    }
    
    // Create pipeline layout
    wgpu::PipelineLayoutDescriptor desc;
    desc.bindGroupLayoutCount = static_cast<uint32_t>(bindGroupLayouts.size());
    desc.bindGroupLayouts = bindGroupLayouts.data();
    
    return device.CreatePipelineLayout(&desc);
}

wgpu::BindGroup Shader::createBindGroup(wgpu::Device& device, uint32_t groupIndex) {
    std::vector<wgpu::BindGroupEntry> entries;
    
    // Find all bindings for this group
    for (const auto& binding : bindings) {
        if (binding.group != groupIndex) continue;
        
        wgpu::BindGroupEntry entry;
        entry.binding = binding.binding;
        
        switch (binding.type) {
            case ShaderBinding::Type::UniformBuffer: {
                UniformBuffer* ub = getUniformBuffer(binding.name);
                if (!ub) {
                    std::cerr << "Shader::createBindGroup: Uniform buffer '" << binding.name << "' not found" << std::endl;
                    continue;
                }
                
                // Create buffer if it doesn't exist yet
                if (!ub->buffer) {
                    wgpu::BufferDescriptor desc;
                    desc.size = ub->size;
                    desc.usage = ub->usage;
                    desc.mappedAtCreation = false;
                    ub->buffer = device.CreateBuffer(&desc);
                    
                    // Upload initial data if available
                    if (!ub->data.empty()) {
                        WebGPUContext& context = WebGPUContext::Ref();
                        context.queue.WriteBuffer(ub->buffer, 0, ub->data.data(), ub->size);
                        ub->dirty = false;
                    }
                }
                
                entry.buffer = ub->buffer;
                entry.offset = 0;
                entry.size = ub->size;
                break;
            }
            // TODO: Add texture, sampler, storage texture support
            default:
                std::cerr << "Shader::createBindGroup: Unsupported binding type for group " << groupIndex << std::endl;
                continue;
        }
        
        entries.push_back(entry);
    }
    
    if (entries.empty()) {
        std::cerr << "Shader::createBindGroup: No bindings found for group " << groupIndex << std::endl;
        return nullptr;
    }
    
    wgpu::BindGroupDescriptor desc;
    desc.layout = bindGroupLayouts[groupIndex];
    desc.entryCount = static_cast<uint32_t>(entries.size());
    desc.entries = entries.data();
    
    return device.CreateBindGroup(&desc);
}

void Shader::bindUniforms(wgpu::RenderPassEncoder& pass, uint32_t groupIndex) {
    // Check if bind group already exists
    auto it = bindGroups.find(groupIndex);
    if (it == bindGroups.end()) {
        // Create bind group if it doesn't exist
        WebGPUContext& context = WebGPUContext::Ref();
        wgpu::BindGroup bindGroup = createBindGroup(context.device, groupIndex);
        if (bindGroup) {
            bindGroups[groupIndex] = bindGroup;
            pass.SetBindGroup(groupIndex, bindGroup, 0, nullptr);
        }
    } else {
        pass.SetBindGroup(groupIndex, it->second, 0, nullptr);
    }
}

