#include "VertexAttribute.h"

uint32_t VertexAttributeType::typeToSize(TYPE type) {
    switch (type) {
        case TYPE::FLOAT: return 4;
        case TYPE::FLOAT2: return 8;
        case TYPE::FLOAT3: return 12;
        case TYPE::FLOAT4: return 16;
        case TYPE::DOUBLE: return 8;
        case TYPE::DOUBLE2: return 16;
        case TYPE::DOUBLE3: return 24;
        case TYPE::DOUBLE4: return 32;
        case TYPE::INT: return 4;
        case TYPE::UINT: return 4;
        case TYPE::UINT2: return 8;
        case TYPE::UINT3: return 12;
        case TYPE::UINT4: return 16;
        case TYPE::SHORT: return 2;
        case TYPE::SHORT2: return 4;
        case TYPE::SHORT3: return 6;
        case TYPE::SHORT4: return 8;
        default: return 0;
    }
}

uint32_t VertexAttributeType::typeToComponents(TYPE type) {
    switch (type) {
        case TYPE::FLOAT: return 1;
        case TYPE::FLOAT2: return 2;
        case TYPE::FLOAT3: return 3;
        case TYPE::FLOAT4: return 4;
        case TYPE::DOUBLE: return 1;
        case TYPE::DOUBLE2: return 2;
        case TYPE::DOUBLE3: return 3;
        case TYPE::DOUBLE4: return 4;
        case TYPE::INT: return 1;
        case TYPE::UINT: return 1;
        case TYPE::UINT2: return 2;
        case TYPE::UINT3: return 3;
        case TYPE::UINT4: return 4;
        case TYPE::SHORT: return 1;
        case TYPE::SHORT2: return 2;
        case TYPE::SHORT3: return 3;
        case TYPE::SHORT4: return 4;
        default: return 0;
    }
}

void VertexLayout::add(const std::string& semantic, VertexAttributeType::TYPE type) {
    uint32_t offset = getVertexSize();
    uint32_t size = VertexAttributeType::typeToSize(type);
    
    VertexAttribute attr;
    attr.semantic = semantic;
    attr.type = type;
    attr.offset = offset;
    attr.size = size;
    
    attributes[semantic] = attr;
}

void VertexLayout::remove(const std::string& semantic) {
    attributes.erase(semantic);
}

bool VertexLayout::has(const std::string& semantic) const {
    return attributes.find(semantic) != attributes.end();
}

VertexAttribute* VertexLayout::get(const std::string& semantic) {
    auto it = attributes.find(semantic);
    if (it != attributes.end()) {
        return &it->second;
    }
    return nullptr;
}

const VertexAttribute* VertexLayout::get(const std::string& semantic) const {
    auto it = attributes.find(semantic);
    if (it != attributes.end()) {
        return &it->second;
    }
    return nullptr;
}

VertexAttributeType::TYPE VertexLayout::getType(const std::string& semantic) const {
    auto it = attributes.find(semantic);
    if (it != attributes.end()) {
        return it->second.type;
    }
    return VertexAttributeType::TYPE::FLOAT; // Default return
}

uint32_t VertexLayout::getVertexSize() const {
    uint32_t totalSize = 0;
    for (const auto& [semantic, attr] : attributes) {
        totalSize += attr.size;
    }
    return totalSize;
}

const std::map<std::string, VertexAttribute>& VertexLayout::getAttributes() const {
    return attributes;
}

