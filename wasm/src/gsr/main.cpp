#include <iostream>

#include <GLFW/glfw3.h>
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif
#include <dawn/webgpu_cpp_print.h>
#include <webgpu/webgpu_cpp.h>
#include "WebGPUContext.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshRenderer.h"

WebGPUContext* g_context = nullptr;
GLFWwindow* g_window = nullptr;
MeshRenderer* g_meshRenderer = nullptr;

const uint32_t kWidth = 512;
const uint32_t kHeight = 512;

void Render() {
  wgpu::SurfaceTexture surfaceTexture = g_context->GetCurrentSurfaceTexture();

  wgpu::RenderPassColorAttachment attachment{
      .view = surfaceTexture.texture.CreateView(),
      .loadOp = wgpu::LoadOp::Clear,
      .storeOp = wgpu::StoreOp::Store};

  wgpu::RenderPassDescriptor renderpass{.colorAttachmentCount = 1,
                                        .colorAttachments = &attachment};

  wgpu::CommandEncoder encoder = g_context->CreateCommandEncoder();
  wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderpass);
  
  if (g_meshRenderer) {
    g_meshRenderer->Render(pass);
  } else {
    // Fallback to old triangle shader
    pass.SetPipeline(Shader::triangle->GetPipeline());
    pass.Draw(3);
  }
  
  pass.End();
  wgpu::CommandBuffer commands = encoder.Finish();
  g_context->SubmitCommandBuffer(commands);
}

void Start() {
  if (!glfwInit()) {
    return;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  g_window = glfwCreateWindow(kWidth, kHeight, "WebGPU window", nullptr, nullptr);
  
  g_context->CreateSurface(g_window);
  g_context->ConfigureSurface();
  Shader::buildPredefined();
  Mesh::buildPredefined();
  
  // Create mesh renderer with predefined shader and mesh
  g_meshRenderer = new MeshRenderer(Mesh::triangle, Shader::vertexcolor2d);

#if defined(__EMSCRIPTEN__)
  emscripten_set_main_loop(Render, 0, false);
#else
  while (!glfwWindowShouldClose(g_window)) {
    glfwPollEvents();
    Render();
    g_context->Present();
    g_context->ProcessEvents();
  }
#endif
}

// Export Init() and Start() for manual calling from JavaScript
extern "C" {
  void InitWebGPU() {
    g_context = new WebGPUContext();
    g_context->Initialize();
  }

  int IsWebGPUInitialized() {
    return (g_context && g_context->isInitialized) ? 1 : 0;
  }

  void StartWebGPU() {
    Start();
  }
}

// main() removed - Init() and Start() are now called manually from JavaScript
