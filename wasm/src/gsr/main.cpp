#include <iostream>
#include <cmath>

#include <GLFW/glfw3.h>
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif
#include <dawn/webgpu_cpp_print.h>
#include <webgpu/webgpu_cpp.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "WebGPUContext.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Camera.h"

WebGPUContext* g_context = nullptr;
GLFWwindow* g_window = nullptr;
MeshRenderer* g_meshRenderer = nullptr;
Camera* g_camera = nullptr;

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
  
  // Update camera rotation around origin
  if (g_camera) {
    // Get time for rotation (use GLFW time or emscripten time)
    float time;
#if defined(__EMSCRIPTEN__)
    time = emscripten_get_now() / 1000.0f;  // Convert ms to seconds
#else
    time = static_cast<float>(glfwGetTime());
#endif
    
    // Rotate camera around origin in a circle
    // Radius of rotation
    float radius = 3.0f;
    // Height of camera
    float height = 1.5f;
    
    // Calculate camera position in a circle around origin
    float angle = time * 0.5f;  // Rotate slowly (0.5 radians per second)
    float x = radius * cosf(angle);
    float z = radius * sinf(angle);
    
    // Camera looks at origin
    glm::vec3 eye(x, height, z);
    glm::vec3 center(0.0f, 0.0f, 0.0f);  // Look at origin
    glm::vec3 up(0.0f, 1.0f, 0.0f);      // Up vector
    
    g_camera->lookAt(eye, center, up);
  }
  
  if (g_meshRenderer) {
    g_meshRenderer->Render(pass, g_camera);
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
  
  // Create camera
  g_camera = new Camera();
  g_camera->setAspect(static_cast<float>(kWidth) / static_cast<float>(kHeight));
  g_camera->setNearFar(0.1f, 100.0f);
  
  // Create mesh renderer with 3D shader (vertexcolor) and triangle mesh
  g_meshRenderer = new MeshRenderer(Mesh::triangle, Shader::vertexcolor);

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
