#include "Mesh.h"
#include <iostream>
#include <cstring>

// Initialize static members
Mesh* Mesh::triangle = nullptr;
Mesh* Mesh::quad = nullptr;

Mesh::~Mesh() {
    // WebGPU buffers are reference-counted, so they'll be cleaned up automatically
    // Just clear the map
    buffers.clear();
    indices = nullptr;
}

void Mesh::addBuffer(const std::string& channelName, wgpu::Buffer buffer) {
    buffers[channelName] = buffer;
}

wgpu::Buffer Mesh::getBuffer(const std::string& channelName) const {
    auto it = buffers.find(channelName);
    if (it != buffers.end()) {
        return it->second;
    }
    return nullptr;
}

bool Mesh::hasBuffer(const std::string& channelName) const {
    return buffers.find(channelName) != buffers.end();
}

void Mesh::buildPredefined() {
    std::cout << "Mesh.buildPredefined" << std::endl;
    if (Mesh::triangle == nullptr) {
        Mesh::triangle = MeshFactory::buildTriangle();
    }
    if (Mesh::quad == nullptr) {
        Mesh::quad = MeshFactory::buildQuad();
    }
}

void Mesh::clearPredefined() {
    std::cout << "Mesh.clearPredefined" << std::endl;
    if (Mesh::triangle != nullptr) {
        delete Mesh::triangle;
        Mesh::triangle = nullptr;
    }
    if (Mesh::quad != nullptr) {
        delete Mesh::quad;
        Mesh::quad = nullptr;
    }
}

// MeshBuilder implementation
void MeshBuilder::setTopology(wgpu::PrimitiveTopology topology) {
    this->topology = topology;
}

void MeshBuilder::addPosition(const std::array<float, 3>& pos) {
    position.push_back(pos);
}

void MeshBuilder::addColor(const std::array<float, 3>& col) {
    color.push_back(col);
}

void MeshBuilder::addTexcoord(const std::array<float, 2>& uv) {
    texcoord.push_back(uv);
}

void MeshBuilder::addNormal(const std::array<float, 3>& norm) {
    normal.push_back(norm);
}

void MeshBuilder::addTangent(const std::array<float, 3>& tan) {
    tangent.push_back(tan);
}

void MeshBuilder::addBitangent(const std::array<float, 3>& bitan) {
    bitangent.push_back(bitan);
}

void MeshBuilder::addIndex(uint16_t index) {
    indices16.push_back(index);
}

void MeshBuilder::addIndex32(uint32_t index) {
    indices32.push_back(index);
}

void MeshBuilder::clearData() {
    position.clear();
    color.clear();
    texcoord.clear();
    normal.clear();
    tangent.clear();
    bitangent.clear();
    indices16.clear();
    indices32.clear();
}

VertexLayout MeshBuilder::buildLayout() {
    VertexLayout layout;
    if (!position.empty()) {
        layout.add(VertexSemantic::POSITION, VertexAttributeType::TYPE::FLOAT3);
    }
    if (!color.empty()) {
        layout.add(VertexSemantic::COLOR, VertexAttributeType::TYPE::FLOAT3);
    }
    if (!texcoord.empty()) {
        layout.add(VertexSemantic::UV, VertexAttributeType::TYPE::FLOAT2);
    }
    if (!normal.empty()) {
        layout.add(VertexSemantic::NORMAL, VertexAttributeType::TYPE::FLOAT3);
    }
    if (!tangent.empty()) {
        layout.add(VertexSemantic::TANGENT, VertexAttributeType::TYPE::FLOAT3);
    }
    if (!bitangent.empty()) {
        layout.add(VertexSemantic::BITANGENT, VertexAttributeType::TYPE::FLOAT3);
    }
    return layout;
}

void MeshBuilder::createChannelBuffers(Mesh* mesh, WebGPUContext& context) {
    uint32_t vertexCount = static_cast<uint32_t>(position.size());
    if (vertexCount == 0) return;
    
    // Create position buffer
    if (!position.empty()) {
        size_t size = position.size() * sizeof(std::array<float, 3>);
        wgpu::Buffer buffer = context.CreateBuffer(
            position.data(), 
            size, 
            wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst
        );
        mesh->addBuffer(VertexSemantic::POSITION, buffer);
    }
    
    // Create color buffer
    if (!color.empty()) {
        size_t size = color.size() * sizeof(std::array<float, 3>);
        wgpu::Buffer buffer = context.CreateBuffer(
            color.data(), 
            size, 
            wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst
        );
        mesh->addBuffer(VertexSemantic::COLOR, buffer);
    }
    
    // Create texcoord buffer
    if (!texcoord.empty()) {
        size_t size = texcoord.size() * sizeof(std::array<float, 2>);
        wgpu::Buffer buffer = context.CreateBuffer(
            texcoord.data(), 
            size, 
            wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst
        );
        mesh->addBuffer(VertexSemantic::UV, buffer);
    }
    
    // Create normal buffer
    if (!normal.empty()) {
        size_t size = normal.size() * sizeof(std::array<float, 3>);
        wgpu::Buffer buffer = context.CreateBuffer(
            normal.data(), 
            size, 
            wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst
        );
        mesh->addBuffer(VertexSemantic::NORMAL, buffer);
    }
    
    // Create tangent buffer
    if (!tangent.empty()) {
        size_t size = tangent.size() * sizeof(std::array<float, 3>);
        wgpu::Buffer buffer = context.CreateBuffer(
            tangent.data(), 
            size, 
            wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst
        );
        mesh->addBuffer(VertexSemantic::TANGENT, buffer);
    }
    
    // Create bitangent buffer
    if (!bitangent.empty()) {
        size_t size = bitangent.size() * sizeof(std::array<float, 3>);
        wgpu::Buffer buffer = context.CreateBuffer(
            bitangent.data(), 
            size, 
            wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst
        );
        mesh->addBuffer(VertexSemantic::BITANGENT, buffer);
    }
}

Mesh* MeshBuilder::build() {
    if (position.empty()) {
        std::cerr << "MeshBuilder::build - No position data!" << std::endl;
        return nullptr;
    }
    
    WebGPUContext& context = WebGPUContext::Ref();
    Mesh* mesh = new Mesh();
    
    // Build layout
    mesh->layout = buildLayout();
    mesh->vertexCount = static_cast<uint32_t>(position.size());
    mesh->topology = topology;
    
    // Create channel buffers
    createChannelBuffers(mesh, context);
    
    // Create index buffer if indices are provided
    if (!indices16.empty()) {
        size_t size = indices16.size() * sizeof(uint16_t);
        mesh->indices = context.CreateBuffer(
            indices16.data(),
            size,
            wgpu::BufferUsage::Index | wgpu::BufferUsage::CopyDst
        );
        mesh->indexFormat = wgpu::IndexFormat::Uint16;
        mesh->indexCount = static_cast<uint32_t>(indices16.size());
    } else if (!indices32.empty()) {
        size_t size = indices32.size() * sizeof(uint32_t);
        mesh->indices = context.CreateBuffer(
            indices32.data(),
            size,
            wgpu::BufferUsage::Index | wgpu::BufferUsage::CopyDst
        );
        mesh->indexFormat = wgpu::IndexFormat::Uint32;
        mesh->indexCount = static_cast<uint32_t>(indices32.size());
    }
    
    return mesh;
}

// MeshFactory implementation
Mesh* MeshFactory::buildTriangle() {
    MeshBuilder builder;
    builder.addPosition({-0.5f, -0.5f, 0.0f});
    builder.addColor({1.0f, 0.0f, 0.0f});
    builder.addPosition({0.5f, -0.5f, 0.0f});
    builder.addColor({0.0f, 1.0f, 0.0f});
    builder.addPosition({0.0f, 0.5f, 0.0f});
    builder.addColor({0.0f, 0.0f, 1.0f});
    builder.setTopology(wgpu::PrimitiveTopology::TriangleList);
    return builder.build();
}

Mesh* MeshFactory::buildQuad() {
    MeshBuilder builder;
    // First triangle (bottom left, bottom right, top right)
    builder.addPosition({-0.5f, -0.5f, 0.0f});
    builder.addColor({1.0f, 0.0f, 0.0f});
    builder.addTexcoord({0.0f, 0.0f});
    builder.addPosition({0.5f, -0.5f, 0.0f});
    builder.addColor({0.0f, 1.0f, 0.0f});
    builder.addTexcoord({1.0f, 0.0f});
    builder.addPosition({0.5f, 0.5f, 0.0f});
    builder.addColor({0.0f, 0.0f, 1.0f});
    builder.addTexcoord({1.0f, 1.0f});
    // Second triangle (bottom left, top right, top left)
    builder.addPosition({-0.5f, -0.5f, 0.0f});
    builder.addColor({1.0f, 0.0f, 0.0f});
    builder.addTexcoord({0.0f, 0.0f});
    builder.addPosition({0.5f, 0.5f, 0.0f});
    builder.addColor({0.0f, 0.0f, 1.0f});
    builder.addTexcoord({1.0f, 1.0f});
    builder.addPosition({-0.5f, 0.5f, 0.0f});
    builder.addColor({0.0f, 1.0f, 0.0f});
    builder.addTexcoord({0.0f, 1.0f});
    builder.setTopology(wgpu::PrimitiveTopology::TriangleList);
    return builder.build();
}

