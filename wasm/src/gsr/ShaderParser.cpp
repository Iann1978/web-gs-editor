#include "ShaderParser.h"
#include <regex>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <unordered_map>

// Helper to trim whitespace
static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

UniformBufferMember::Type ShaderParser::parseType(const std::string& typeStr) {
    std::string type = trim(typeStr);
    
    if (type == "f32") return UniformBufferMember::Type::Float;
    if (type == "vec2<f32>" || type == "vec2") return UniformBufferMember::Type::Float2;
    if (type == "vec3<f32>" || type == "vec3") return UniformBufferMember::Type::Float3;
    if (type == "vec4<f32>" || type == "vec4") return UniformBufferMember::Type::Float4;
    if (type == "mat3x3<f32>" || type == "mat3x3" || type == "mat3") return UniformBufferMember::Type::Mat3;
    if (type == "mat4x4<f32>" || type == "mat4x4" || type == "mat4") return UniformBufferMember::Type::Mat4;
    if (type == "i32" || type == "int") return UniformBufferMember::Type::Int;
    if (type == "u32" || type == "uint") return UniformBufferMember::Type::UInt;
    if (type == "bool") return UniformBufferMember::Type::Bool;
    
    return UniformBufferMember::Type::Unknown;
}

size_t ShaderParser::getTypeSize(UniformBufferMember::Type type) {
    switch (type) {
        case UniformBufferMember::Type::Float: return 4;
        case UniformBufferMember::Type::Float2: return 8;
        case UniformBufferMember::Type::Float3: return 12;
        case UniformBufferMember::Type::Float4: return 16;
        case UniformBufferMember::Type::Mat3: return 48;  // 3 * vec4 = 48 bytes (aligned)
        case UniformBufferMember::Type::Mat4: return 64;  // 4 * vec4 = 64 bytes
        case UniformBufferMember::Type::Int: return 4;
        case UniformBufferMember::Type::UInt: return 4;
        case UniformBufferMember::Type::Bool: return 4;  // Bool is 4 bytes in WGSL
        default: return 0;
    }
}

size_t ShaderParser::getTypeAlignment(UniformBufferMember::Type type) {
    switch (type) {
        case UniformBufferMember::Type::Float: return 4;
        case UniformBufferMember::Type::Float2: return 8;
        case UniformBufferMember::Type::Float3: return 16;  // vec3 is aligned to 16 bytes
        case UniformBufferMember::Type::Float4: return 16;
        case UniformBufferMember::Type::Mat3: return 16;  // Each column is vec4-aligned
        case UniformBufferMember::Type::Mat4: return 16;
        case UniformBufferMember::Type::Int: return 4;
        case UniformBufferMember::Type::UInt: return 4;
        case UniformBufferMember::Type::Bool: return 4;
        default: return 1;
    }
}

size_t ShaderParser::alignOffset(size_t offset, size_t alignment) {
    return (offset + alignment - 1) & ~(alignment - 1);
}

std::vector<UniformBufferMember> ShaderParser::parseStructMembers(const std::string& structDef) {
    std::vector<UniformBufferMember> members;
    
    // Extract struct body (content between { and })
    std::regex structRegex(R"(struct\s+\w+\s*\{([^}]+)\})");
    std::smatch structMatch;
    
    if (!std::regex_search(structDef, structMatch, structRegex)) {
        return members;
    }
    
    std::string body = structMatch[1].str();
    size_t currentOffset = 0;
    
    // Parse each member line
    // Pattern: member_name: type, (handle types with angle brackets like mat4x4<f32>)
    std::regex memberRegex(R"(\s*(\w+)\s*:\s*([^,\n]+),?\s*)");
    std::sregex_iterator iter(body.begin(), body.end(), memberRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::smatch match = *iter;
        std::string memberName = trim(match[1].str());
        std::string typeStr = trim(match[2].str());
        
        UniformBufferMember member;
        member.name = memberName;
        member.type = parseType(typeStr);
        
        if (member.type == UniformBufferMember::Type::Unknown) {
            std::cerr << "ShaderParser: Unknown type '" << typeStr << "' for member '" << memberName << "'" << std::endl;
            continue;
        }
        
        // Align offset
        size_t alignment = getTypeAlignment(member.type);
        currentOffset = alignOffset(currentOffset, alignment);
        
        member.offset = currentOffset;
        member.size = getTypeSize(member.type);
        
        // Update offset for next member
        currentOffset += member.size;
        
        members.push_back(member);
    }
    
    return members;
}

ShaderBinding ShaderParser::parseUniformBuffer(const std::string& structDef,
                                               const std::string& varDecl,
                                               uint32_t group,
                                               uint32_t binding,
                                               const std::string& varName) {
    ShaderBinding bindingInfo;
    bindingInfo.type = ShaderBinding::Type::UniformBuffer;
    bindingInfo.group = group;
    bindingInfo.binding = binding;
    bindingInfo.name = varName;
    bindingInfo.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;  // Default to both
    
    // Parse struct members
    bindingInfo.members = parseStructMembers(structDef);
    
    // Calculate total size (align to 16 bytes for uniform buffers)
    size_t totalSize = 0;
    for (const auto& member : bindingInfo.members) {
        size_t alignedOffset = alignOffset(totalSize, getTypeAlignment(member.type));
        totalSize = alignedOffset + member.size;
    }
    // Align total size to 16 bytes (WGSL uniform buffer requirement)
    bindingInfo.size = alignOffset(totalSize, 16);
    
    return bindingInfo;
}

std::vector<ShaderBinding> ShaderParser::parseBindings(const std::string& wgslSource) {
    std::vector<ShaderBinding> bindings;
    
    // Find all uniform buffer declarations: @group(N) @binding(M) var<uniform> name: Type;
    std::regex uniformRegex(R"(@group\s*\(\s*(\d+)\s*\)\s*@binding\s*\(\s*(\d+)\s*\)\s*var<uniform>\s+(\w+)\s*:\s*(\w+)\s*;)");
    std::sregex_iterator iter(wgslSource.begin(), wgslSource.end(), uniformRegex);
    std::sregex_iterator end;
    
    // Store struct definitions for lookup
    std::unordered_map<std::string, std::string> structDefs;
    std::regex structRegex(R"(struct\s+(\w+)\s*\{([^}]+)\})");
    std::sregex_iterator structIter(wgslSource.begin(), wgslSource.end(), structRegex);
    std::sregex_iterator structEnd;
    
    for (; structIter != structEnd; ++structIter) {
        std::smatch match = *structIter;
        std::string structName = match[1].str();
        std::string structBody = match[0].str();  // Full struct definition
        structDefs[structName] = structBody;
    }
    
    // Parse each uniform buffer
    for (; iter != end; ++iter) {
        std::smatch match = *iter;
        uint32_t group = static_cast<uint32_t>(std::stoul(match[1].str()));
        uint32_t bindingIndex = static_cast<uint32_t>(std::stoul(match[2].str()));
        std::string varName = match[3].str();
        std::string typeName = match[4].str();
        
        // Find struct definition
        auto structIt = structDefs.find(typeName);
        if (structIt == structDefs.end()) {
            std::cerr << "ShaderParser: Struct '" << typeName << "' not found for uniform '" << varName << "'" << std::endl;
            continue;
        }
        
        ShaderBinding binding = parseUniformBuffer(structIt->second, match[0].str(), group, bindingIndex, varName);
        bindings.push_back(binding);
    }
    
    return bindings;
}

