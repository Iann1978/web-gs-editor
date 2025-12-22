<template>
  <div class="webgpu-container">
    <canvas 
      ref="canvasRef" 
      id="wasm-webgpu-canvas" 
      class="emscripten"
      @contextmenu.prevent
      tabindex="-1"
    ></canvas>
    <div v-if="loading" class="status">Loading...</div>
    <div v-if="error" class="error">{{ error }}</div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'
import { EmscriptenWasmModule } from '../core/wasm/EmscriptenWasmModule'

const canvasRef = ref<HTMLCanvasElement | null>(null)
const error = ref<string>('')
const loading = ref<boolean>(true)

onMounted(async () => {
  if (!canvasRef.value) {
    error.value = 'Canvas element not found'
    return
  }

  try {
    await EmscriptenWasmModule.init({
      canvas: canvasRef.value,
      wasmPath: new URL('../../wasm/pkg/app.wasm', import.meta.url).href,
      jsPath: new URL('../../wasm/pkg/app.js', import.meta.url).href,
      onStatusUpdate: (text: string) => {
        // Optional: can be used for status updates
        console.log('Status:', text)
      },
      onLoadingChange: (isLoading: boolean) => {
        loading.value = isLoading
      },
      onError: (errorMessage: string) => {
        error.value = errorMessage
        loading.value = false
      },
      onInitialized: () => {
        console.log('WASM module initialized and ready to use')
        // You can now access exported functions via:
        // EmscriptenWasmModule.module.someFunction()
        // or
        // EmscriptenWasmModule.getExportedFunction('someFunction')()
      }
    })
  } catch (err) {
    error.value = err instanceof Error ? err.message : 'Unknown error occurred'
    loading.value = false
    console.error('WASM WebGPU initialization error:', err)
  }
})

onUnmounted(() => {
  EmscriptenWasmModule.cleanup()
})
</script>

<style scoped>
.webgpu-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 1rem;
  padding: 1rem;
}

canvas.emscripten {
  border: 0px none;
  background-color: black;
  display: block;
  /* Canvas dimensions are set in C++ code (512x512) */
  /* Can be styled via CSS if needed */
}

.status {
  color: #666;
  font-size: 0.9rem;
  font-style: italic;
}

.error {
  color: #ff4444;
  font-size: 0.9rem;
  padding: 1rem;
  background: rgba(255, 68, 68, 0.1);
  border-radius: 4px;
  border: 1px solid rgba(255, 68, 68, 0.3);
  max-width: 600px;
  text-align: center;
}
</style>

