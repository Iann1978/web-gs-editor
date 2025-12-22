#pragma once

#include <string>
#include <map>
#include <cstdint>

class VertexAttributeType {
public:
    enum class TYPE : uint32_t {
        FLOAT = 0,
        FLOAT2 = 1,
        FLOAT3 = 2,
        FLOAT4 = 3,
        DOUBLE = 4,
        DOUBLE2 = 5,
        DOUBLE3 = 6,
        DOUBLE4 = 7,
        INT = 16,
        UINT = 8,
        UINT2 = 9,
        UINT3 = 10,
        UINT4 = 11,
        SHORT = 12,
        SHORT2 = 13,
        SHORT3 = 14,
        SHORT4 = 15,
    };

    static uint32_t typeToSize(TYPE type);
    static uint32_t typeToComponents(TYPE type);
};

struct VertexAttribute {
    std::string semantic;
    VertexAttributeType::TYPE type;
    uint32_t offset;  // Byte offset in interleaved buffer
    uint32_t size;    // Byte size
};

namespace VertexSemantic {
    constexpr const char* POSITION = "position";
    constexpr const char* COLOR = "color";
    constexpr const char* UV = "texcoord";
    constexpr const char* NORMAL = "normal";
    constexpr const char* TANGENT = "tangent";
    constexpr const char* BITANGENT = "bitangent";
}

class VertexLayout {
private:
    std::map<std::string, VertexAttribute> attributes;

public:
    VertexLayout() = default;

    void add(const std::string& semantic, VertexAttributeType::TYPE type);
    void remove(const std::string& semantic);
    bool has(const std::string& semantic) const;
    VertexAttribute* get(const std::string& semantic);
    const VertexAttribute* get(const std::string& semantic) const;
    VertexAttributeType::TYPE getType(const std::string& semantic) const;
    uint32_t getVertexSize() const;  // Total byte size
    const std::map<std::string, VertexAttribute>& getAttributes() const;
};

