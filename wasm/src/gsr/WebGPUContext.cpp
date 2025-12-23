#include "WebGPUContext.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

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

