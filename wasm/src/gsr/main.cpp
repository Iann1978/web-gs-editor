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

WebGPUContext* g_context = nullptr;
GLFWwindow* g_window = nullptr;
Mesh* g_triangleMesh = nullptr;
Shader* g_meshShader = nullptr;

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
  
  if (g_meshShader && g_triangleMesh) {
    pass.SetPipeline(g_meshShader->GetPipeline());
    Shader::BindMeshBuffers(pass, g_triangleMesh);
    
    if (g_triangleMesh->indices) {
      pass.DrawIndexed(g_triangleMesh->indexCount);
    } else {
      pass.Draw(g_triangleMesh->vertexCount);
    }
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
  
  // Create a shader for mesh rendering
  const char meshShaderCode[] = R"(
    struct VertexInput {
      @location(0) position: vec3<f32>,
      @location(1) color: vec3<f32>,
    };
    
    struct VertexOutput {
      @builtin(position) position: vec4<f32>,
      @location(0) color: vec3<f32>,
    };
    
    @vertex
    fn vertexMain(input: VertexInput) -> VertexOutput {
      var output: VertexOutput;
      output.position = vec4<f32>(input.position, 1.0);
      output.color = input.color;
      return output;
    }
    
    @fragment
    fn fragmentMain(input: VertexOutput) -> @location(0) vec4<f32> {
      return vec4<f32>(input.color, 1.0);
    }
  )";
  
  WebGPUContext& context = WebGPUContext::Ref();
  wgpu::ShaderModule shaderModule = context.CreateShaderModule(meshShaderCode);
  
  // Create vertex buffer layouts for position and color
  // Store attributes to keep them alive
  wgpu::VertexAttribute posAttr;
  posAttr.shaderLocation = 0;
  posAttr.offset = 0;
  posAttr.format = wgpu::VertexFormat::Float32x3;
  
  wgpu::VertexAttribute colorAttr;
  colorAttr.shaderLocation = 1;
  colorAttr.offset = 0;
  colorAttr.format = wgpu::VertexFormat::Float32x3;
  
  wgpu::VertexBufferLayout posLayout;
  posLayout.arrayStride = 12; // 3 floats * 4 bytes
  posLayout.stepMode = wgpu::VertexStepMode::Vertex;
  posLayout.attributeCount = 1;
  posLayout.attributes = &posAttr;
  
  wgpu::VertexBufferLayout colorLayout;
  colorLayout.arrayStride = 12; // 3 floats * 4 bytes
  colorLayout.stepMode = wgpu::VertexStepMode::Vertex;
  colorLayout.attributeCount = 1;
  colorLayout.attributes = &colorAttr;
  
  std::vector<wgpu::VertexBufferLayout> vertexLayouts;
  vertexLayouts.push_back(posLayout);
  vertexLayouts.push_back(colorLayout);
  
  // Create render pipeline with vertex buffers
  wgpu::ColorTargetState colorTarget;
  colorTarget.format = context.format;
  
  wgpu::FragmentState fragmentState;
  fragmentState.module = shaderModule;
  fragmentState.targetCount = 1;
  fragmentState.targets = &colorTarget;
  
  wgpu::VertexState vertexState;
  vertexState.module = shaderModule;
  vertexState.entryPoint = "vertexMain";
  vertexState.bufferCount = static_cast<uint32_t>(vertexLayouts.size());
  vertexState.buffers = vertexLayouts.data();
  
  wgpu::RenderPipelineDescriptor pipelineDesc;
  pipelineDesc.vertex = vertexState;
  pipelineDesc.fragment = &fragmentState;
  
  wgpu::RenderPipeline pipeline = context.device.CreateRenderPipeline(&pipelineDesc);
  
  g_meshShader = new Shader();
  g_meshShader->pipeline = pipeline;
  g_triangleMesh = Mesh::triangle;

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
