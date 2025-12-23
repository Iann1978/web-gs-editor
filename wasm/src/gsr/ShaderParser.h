#pragma once

#include "ShaderBinding.h"
#include <string>
#include <vector>

class ShaderParser {
public:
    // Parse WGSL source and extract all bindings
    static std::vector<ShaderBinding> parseBindings(const std::string& wgslSource);
    
    // Parse a uniform buffer binding
    static ShaderBinding parseUniformBuffer(const std::string& structDef, 
                                           const std::string& varDecl,
                                           uint32_t group,
                                           uint32_t binding,
                                           const std::string& varName);
    
    // Parse struct members and calculate offsets
    static std::vector<UniformBufferMember> parseStructMembers(const std::string& structDef);
    
private:
    // Parse WGSL type string to UniformBufferMember::Type
    static UniformBufferMember::Type parseType(const std::string& typeStr);
    
    // Get size in bytes for a type
    static size_t getTypeSize(UniformBufferMember::Type type);
    
    // Calculate alignment for a type (WGSL alignment rules)
    static size_t getTypeAlignment(UniformBufferMember::Type type);
    
    // Align offset to the required alignment
    static size_t alignOffset(size_t offset, size_t alignment);
};

