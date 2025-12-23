#include "WebGPUContext.h"
#include "VertexAttribute.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

WebGPUContext* WebGPUContext::ins = nullptr;

WebGPUContext& WebGPUContext::Ref() {
    return *ins;
}

WebGPUContext::WebGPUContext() {
    if (ins == nullptr)
        ins = this;
}

WebGPUContext::~WebGPUContext() {
    Destroy();
}

void WebGPUContext::Initialize() {
    static const auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
    wgpu::InstanceDescriptor instanceDesc{.requiredFeatureCount = 1,
                                          .requiredFeatures = &kTimedWaitAny};
    instance = wgpu::CreateInstance(&instanceDesc);

    wgpu::Future f1 = instance.RequestAdapter(
        nullptr, wgpu::CallbackMode::WaitAnyOnly,
        [this](wgpu::RequestAdapterStatus status, wgpu::Adapter a,
               wgpu::StringView message) {
          if (status != wgpu::RequestAdapterStatus::Success) {
            std::string msg(message.data ? message.data : "", message.length);
            std::cout << "RequestAdapter: " << msg << "\n";
            std::exit(0);
          }
          adapter = std::move(a);
        });
    instance.WaitAny(f1, UINT64_MAX);

    wgpu::DeviceDescriptor desc{};
    desc.SetUncapturedErrorCallback([](const wgpu::Device&,
                                       wgpu::ErrorType errorType,
                                       wgpu::StringView message) {
      std::string msg(message.data ? message.data : "", message.length);
      std::cout << "Error: " << static_cast<int>(errorType) << " - message: " << msg << "\n";
    });

    wgpu::Future f2 = adapter.RequestDevice(
        &desc, wgpu::CallbackMode::WaitAnyOnly,
        [this](wgpu::RequestDeviceStatus status, wgpu::Device d,
               wgpu::StringView message) {
          if (status != wgpu::RequestDeviceStatus::Success) {
            std::string msg(message.data ? message.data : "", message.length);
            std::cout << "RequestDevice: " << msg << "\n";
            std::exit(0);
          }
          device = std::move(d);
        });
    instance.WaitAny(f2, UINT64_MAX);
    
    queue = device.GetQueue();
    isInitialized = true;  // Mark as initialized after device is ready
}

void WebGPUContext::CreateSurface(GLFWwindow* window) {
    surface = wgpu::glfw::CreateSurfaceForWindow(instance, window);
}

void WebGPUContext::ConfigureSurface() {
    wgpu::SurfaceCapabilities capabilities;
    surface.GetCapabilities(adapter, &capabilities);
    format = capabilities.formats[0];

    wgpu::SurfaceConfiguration config{.device = device,
                                      .format = format,
                                      .width = width,
                                      .height = height};
    surface.Configure(&config);
}

wgpu::ShaderModule WebGPUContext::CreateShaderModule(const char* wgslCode) {
    wgpu::ShaderSourceWGSL wgsl{{.code = wgslCode}};
    wgpu::ShaderModuleDescriptor shaderModuleDescriptor{.nextInChain = &wgsl};
    return device.CreateShaderModule(&shaderModuleDescriptor);
}

wgpu::RenderPipeline WebGPUContext::CreateRenderPipeline(
    wgpu::ShaderModule shaderModule, wgpu::TextureFormat targetFormat) {
    wgpu::ColorTargetState colorTargetState{.format = targetFormat};

    wgpu::FragmentState fragmentState{
        .module = shaderModule, .targetCount = 1, .targets = &colorTargetState};

    wgpu::RenderPipelineDescriptor descriptor{.vertex = {.module = shaderModule},
                                              .fragment = &fragmentState};
    return device.CreateRenderPipeline(&descriptor);
}

wgpu::SurfaceTexture WebGPUContext::GetCurrentSurfaceTexture() {
    wgpu::SurfaceTexture surfaceTexture;
    surface.GetCurrentTexture(&surfaceTexture);
    return surfaceTexture;
}

wgpu::CommandEncoder WebGPUContext::CreateCommandEncoder() {
    return device.CreateCommandEncoder();
}

void WebGPUContext::SubmitCommandBuffer(wgpu::CommandBuffer commandBuffer) {
    queue.Submit(1, &commandBuffer);
}

void WebGPUContext::Present() {
    surface.Present();
}

void WebGPUContext::ProcessEvents() {
    instance.ProcessEvents();
}

wgpu::Buffer WebGPUContext::CreateBuffer(const void* data, size_t size, wgpu::BufferUsage usage) {
    wgpu::BufferDescriptor descriptor{};
    descriptor.size = size;
    descriptor.usage = usage;
    descriptor.mappedAtCreation = (data != nullptr);
    
    wgpu::Buffer buffer = device.CreateBuffer(&descriptor);
    
    if (data != nullptr) {
        void* mappedData = buffer.GetMappedRange();
        if (mappedData) {
            memcpy(mappedData, data, size);
            buffer.Unmap();
        } else {
            // If mapping failed, use queue write
            queue.WriteBuffer(buffer, 0, data, size);
        }
    }
    
    return buffer;
}

WebGPUContext::VertexBufferLayoutData WebGPUContext::CreateVertexBufferLayouts(const VertexLayout& layout) {
    VertexBufferLayoutData data;
    
    // For WebGPU, we need to create separate buffer layouts for each channel
    // Since we're using channel buffers (like PCL), each channel is a separate buffer
    for (const auto& [semantic, attr] : layout.getAttributes()) {
        uint32_t slot = VertexSemantic::getSlot(semantic);
        if (slot == UINT32_MAX) continue; // Skip invalid semantics
        
        // Create vertex attribute
        wgpu::VertexAttribute vertexAttr;
        vertexAttr.shaderLocation = slot;  // Use semantic's fixed slot
        vertexAttr.offset = 0;
        
        // Map VertexAttributeType to WebGPU format
        switch (attr.type) {
            case VertexAttributeType::TYPE::FLOAT:
                vertexAttr.format = wgpu::VertexFormat::Float32;
                break;
            case VertexAttributeType::TYPE::FLOAT2:
                vertexAttr.format = wgpu::VertexFormat::Float32x2;
                break;
            case VertexAttributeType::TYPE::FLOAT3:
                vertexAttr.format = wgpu::VertexFormat::Float32x3;
                break;
            case VertexAttributeType::TYPE::FLOAT4:
                vertexAttr.format = wgpu::VertexFormat::Float32x4;
                break;
            default:
                vertexAttr.format = wgpu::VertexFormat::Float32;
                break;
        }
        
        data.attributes.push_back(vertexAttr);
        
        wgpu::VertexBufferLayout bufferLayout;
        bufferLayout.arrayStride = VertexAttributeType::typeToSize(attr.type);
        bufferLayout.stepMode = wgpu::VertexStepMode::Vertex;
        bufferLayout.attributeCount = 1;
        bufferLayout.attributes = &data.attributes.back(); // Point to the stored attribute
        
        data.layouts.push_back(bufferLayout);
    }
    
    return data;
}

void WebGPUContext::Destroy() {
    // WebGPU objects are reference-counted, so cleanup is simpler than Vulkan
    // Just reset the objects - they will be cleaned up automatically
    surface = nullptr;
    queue = nullptr;
    device = nullptr;
    adapter = nullptr;
    instance = nullptr;
    isInitialized = false;
}

