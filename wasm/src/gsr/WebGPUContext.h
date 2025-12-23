#pragma once

#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_glfw.h>
#include <GLFW/glfw3.h>

class WebGPUContext {
public:
    wgpu::Instance instance;
    wgpu::Adapter adapter;
    wgpu::Device device;
    wgpu::Queue queue;
    wgpu::Surface surface;
    wgpu::TextureFormat format;
    
    uint32_t width = 512;
    uint32_t height = 512;
    bool isInitialized = false;

public:
    WebGPUContext();
    ~WebGPUContext();
    
    // Initialization
    void Initialize();
    void CreateSurface(GLFWwindow* window);
    void ConfigureSurface();
    
    // Resource creation helpers
    wgpu::ShaderModule CreateShaderModule(const char* wgslCode);
    wgpu::RenderPipeline CreateRenderPipeline(wgpu::ShaderModule shaderModule, wgpu::TextureFormat targetFormat);
    
    // Rendering operations
    wgpu::SurfaceTexture GetCurrentSurfaceTexture();
    wgpu::CommandEncoder CreateCommandEncoder();
    void SubmitCommandBuffer(wgpu::CommandBuffer commandBuffer);
    
    // Surface operations
    void Present();
    void ProcessEvents();
    
    // Cleanup
    void Destroy();
    
    // Singleton pattern
    static WebGPUContext* ins;
    static WebGPUContext& Ref();
};

