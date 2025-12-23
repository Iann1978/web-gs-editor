#pragma once

#include <webgpu/webgpu_cpp.h>
#include <map>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include "VertexAttribute.h"
#include "WebGPUContext.h"

class Mesh {
public:
    uint32_t vertexCount = 0;
    wgpu::PrimitiveTopology topology = wgpu::PrimitiveTopology::TriangleList;
    
    // Channel buffers - like PCL pattern
    std::map<std::string, wgpu::Buffer> buffers;
    VertexLayout layout;
    
    // Index buffer (optional)
    wgpu::Buffer indices = nullptr;
    wgpu::IndexFormat indexFormat = wgpu::IndexFormat::Uint16;
    uint32_t indexCount = 0;
    
    ~Mesh();
    
    // Buffer management
    void addBuffer(const std::string& channelName, wgpu::Buffer buffer);
    wgpu::Buffer getBuffer(const std::string& channelName) const;
    bool hasBuffer(const std::string& channelName) const;
    
    // Static predefined meshes
    static Mesh* triangle;
    static Mesh* quad;
    static void buildPredefined();
    static void clearPredefined();
};

class MeshBuilder {
private:
    std::vector<std::array<float, 3>> position;
    std::vector<std::array<float, 3>> color;
    std::vector<std::array<float, 2>> texcoord;
    std::vector<std::array<float, 3>> normal;
    std::vector<std::array<float, 3>> tangent;
    std::vector<std::array<float, 3>> bitangent;
    std::vector<uint16_t> indices16;
    std::vector<uint32_t> indices32;
    wgpu::PrimitiveTopology topology = wgpu::PrimitiveTopology::TriangleList;
    
    VertexLayout buildLayout();
    void createChannelBuffers(Mesh* mesh, WebGPUContext& context);

public:
    MeshBuilder() = default;
    
    void setTopology(wgpu::PrimitiveTopology topology);
    void addPosition(const std::array<float, 3>& pos);
    void addColor(const std::array<float, 3>& col);
    void addTexcoord(const std::array<float, 2>& uv);
    void addNormal(const std::array<float, 3>& norm);
    void addTangent(const std::array<float, 3>& tan);
    void addBitangent(const std::array<float, 3>& bitan);
    void addIndex(uint16_t index);
    void addIndex32(uint32_t index);
    
    void clearData();
    
    Mesh* build();
};

class MeshFactory {
public:
    static Mesh* buildTriangle();
    static Mesh* buildQuad();
};

