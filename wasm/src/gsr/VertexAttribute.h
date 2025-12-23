#pragma once

#include <string>
#include <map>
#include <cstdint>
#include <climits>

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
    
    // Get fixed slot number for each semantic
    // This ensures consistent slot assignment regardless of insertion order
    // Slots are fixed: POSITION=0, COLOR=1, UV=2, NORMAL=3, TANGENT=4, BITANGENT=5
    inline uint32_t getSlot(const std::string& semantic) {
        if (semantic == POSITION) return 0;
        if (semantic == COLOR) return 1;
        if (semantic == UV) return 2;
        if (semantic == NORMAL) return 3;
        if (semantic == TANGENT) return 4;
        if (semantic == BITANGENT) return 5;
        return UINT32_MAX; // Invalid semantic
    }
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

