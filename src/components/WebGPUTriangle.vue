<template>
  <div class="webgpu-container">
    <canvas ref="canvasRef" width="800" height="600"></canvas>
    <div v-if="error" class="error">{{ error }}</div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'

const canvasRef = ref<HTMLCanvasElement | null>(null)
const error = ref<string>('')

let device: GPUDevice | null = null
let context: GPUCanvasContext | null = null
let renderPipeline: GPURenderPipeline | null = null
let vertexBuffer: GPUBuffer | null = null
let animationFrameId: number | null = null

onMounted(async () => {
  if (!canvasRef.value) return

  try {
    // Check if WebGPU is supported
    if (!navigator.gpu) {
      throw new Error('WebGPU is not supported in this browser')
    }

    // Request adapter and device
    const adapter = await navigator.gpu.requestAdapter()
    if (!adapter) {
      throw new Error('Failed to get GPU adapter')
    }

    device = await adapter.requestDevice()
    context = canvasRef.value.getContext('webgpu')
    
    if (!context) {
      throw new Error('Failed to get WebGPU context')
    }

    const canvasFormat = navigator.gpu.getPreferredCanvasFormat()
    context.configure({
      device: device,
      format: canvasFormat,
    })

    // Define triangle vertices (position + color)
    // Each vertex: [x, y, r, g, b, a]
    const vertices = new Float32Array([
      // Top vertex (red)
      0.0, 0.5, 1.0, 0.0, 0.0, 1.0,
      // Bottom left (green)
      -0.5, -0.5, 0.0, 1.0, 0.0, 1.0,
      // Bottom right (blue)
      0.5, -0.5, 0.0, 0.0, 1.0, 1.0,
    ])

    // Create vertex buffer
    vertexBuffer = device.createBuffer({
      label: 'Triangle vertices',
      size: vertices.byteLength,
      usage: GPUBufferUsage.VERTEX | GPUBufferUsage.COPY_DST,
    })
    device.queue.writeBuffer(vertexBuffer, 0, vertices)

    // Create shader module
    const shaderModule = device.createShaderModule({
      label: 'Triangle shaders',
      code: `
        struct VertexInput {
          @location(0) position: vec2<f32>,
          @location(1) color: vec4<f32>,
        }

        struct VertexOutput {
          @builtin(position) position: vec4<f32>,
          @location(0) color: vec4<f32>,
        }

        @vertex
        fn vs(input: VertexInput) -> VertexOutput {
          var output: VertexOutput;
          output.position = vec4<f32>(input.position, 0.0, 1.0);
          output.color = input.color;
          return output;
        }

        @fragment
        fn fs(input: VertexOutput) -> @location(0) vec4<f32> {
          return input.color;
        }
      `,
    })

    // Create render pipeline
    renderPipeline = device.createRenderPipeline({
      label: 'Triangle pipeline',
      layout: 'auto',
      vertex: {
        module: shaderModule,
        entryPoint: 'vs',
        buffers: [
          {
            arrayStride: 6 * Float32Array.BYTES_PER_ELEMENT, // 6 floats per vertex
            attributes: [
              {
                shaderLocation: 0, // position
                offset: 0,
                format: 'float32x2',
              },
              {
                shaderLocation: 1, // color
                offset: 2 * Float32Array.BYTES_PER_ELEMENT,
                format: 'float32x4',
              },
            ],
          },
        ],
      },
      fragment: {
        module: shaderModule,
        entryPoint: 'fs',
        targets: [
          {
            format: canvasFormat,
          },
        ],
      },
      primitive: {
        topology: 'triangle-list',
      },
    })

    // Start render loop
    render()
  } catch (err) {
    error.value = err instanceof Error ? err.message : 'Unknown error occurred'
    console.error('WebGPU initialization error:', err)
  }
})

onUnmounted(() => {
  if (animationFrameId !== null) {
    cancelAnimationFrame(animationFrameId)
  }
})

function render() {
  if (!device || !context || !renderPipeline || !vertexBuffer || !canvasRef.value) return

  const commandEncoder = device.createCommandEncoder()
  const view = context.getCurrentTexture().createView()

  const renderPassDescriptor: GPURenderPassDescriptor = {
    label: 'Triangle render pass',
    colorAttachments: [
      {
        view: view,
        clearValue: { r: 0.1, g: 0.1, b: 0.1, a: 1.0 },
        loadOp: 'clear',
        storeOp: 'store',
      },
    ],
  }

  const pass = commandEncoder.beginRenderPass(renderPassDescriptor)
  pass.setPipeline(renderPipeline)
  pass.setVertexBuffer(0, vertexBuffer)
  pass.draw(3)
  pass.end()

  device.queue.submit([commandEncoder.finish()])

  animationFrameId = requestAnimationFrame(render)
}
</script>

<style scoped>
.webgpu-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 1rem;
}

canvas {
  border: 2px solid #333;
  border-radius: 8px;
  background: #1a1a1a;
}

.error {
  color: #ff4444;
  font-size: 0.9rem;
  padding: 1rem;
  background: rgba(255, 68, 68, 0.1);
  border-radius: 4px;
  border: 1px solid rgba(255, 68, 68, 0.3);
}
</style>

