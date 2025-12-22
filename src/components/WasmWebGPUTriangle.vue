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

const canvasRef = ref<HTMLCanvasElement | null>(null)
const error = ref<string>('')
const loading = ref<boolean>(true)

let scriptElement: HTMLScriptElement | null = null
let wasmModule: any = null

onMounted(async () => {
  if (!canvasRef.value) {
    error.value = 'Canvas element not found'
    return
  }

  try {
    // Check if WebGPU is supported
    if (!('gpu' in navigator)) {
      throw new Error('WebGPU is not supported in this browser')
    }

    // Configure the Emscripten Module object before loading app.js
    // app.js is not modularized, so it expects a global Module object
    const Module: any = {
      print(...args: any[]) {
        console.log(...args)
      },
      canvas: canvasRef.value,
      setStatus(text: string) {
        // Optional: can be used for status updates
        if (text) {
          console.log('Status:', text)
        }
      },
      totalDependencies: 0,
      monitorRunDependencies(left: number) {
        Module.totalDependencies = Math.max(Module.totalDependencies, left)
        if (left === 0) {
          loading.value = false
        }
      },
      locateFile(path: string) {
        // Ensure app.wasm is found at the correct path
        if (path.endsWith('.wasm')) {
          return new URL('../../wasm/pkg/app.wasm', import.meta.url).href
        }
        return path
      },
      onRuntimeInitialized() {
        // Called when WASM is ready
        wasmModule = Module
        loading.value = false
        console.log('WASM module initialized')
      }
    }

    // Make Module available globally (app.js expects it)
    ;(window as any).Module = Module

    // Handle WebGPU context loss
    canvasRef.value.addEventListener('webglcontextlost', (e) => {
      console.error('WebGL context lost')
      error.value = 'WebGPU context lost. Please reload the page.'
      e.preventDefault()
    }, false)

    // Handle window errors
    const originalErrorHandler = window.onerror
    window.onerror = (event) => {
      console.error('Error occurred:', event)
      error.value = 'An error occurred. Check the console for details.'
      loading.value = false
      if (originalErrorHandler) {
        return originalErrorHandler.call(window, event)
      }
      return false
    }

    // Dynamically load app.js
    // Note: app.js is not modularized, so it must be loaded as a script tag
    // The path assumes wasm/pkg is accessible (may need to be in public folder or configured in Vite)
    scriptElement = document.createElement('script')
    scriptElement.type = 'text/javascript'
    scriptElement.async = true
    
    // Try to resolve the path - Vite may need files in public folder or configured
    // For now, use a relative path from the component location
    try {
      const scriptUrl = new URL('../../wasm/pkg/app.js', import.meta.url).href
      scriptElement.src = scriptUrl
    } catch (e) {
      // Fallback: assume files are in public folder or served from root
      scriptElement.src = '/wasm/pkg/app.js'
    }
    
    // Wait for script to load
    await new Promise<void>((resolve, reject) => {
      scriptElement!.onload = () => {
        resolve()
      }
      scriptElement!.onerror = () => {
        reject(new Error('Failed to load app.js. Make sure wasm/pkg/app.js is accessible.'))
      }
      document.head.appendChild(scriptElement!)
    })

    // The script will automatically call main() and start the render loop
    // No additional initialization needed

  } catch (err) {
    error.value = err instanceof Error ? err.message : 'Unknown error occurred'
    loading.value = false
    console.error('WASM WebGPU initialization error:', err)
  }
})

onUnmounted(() => {
  // Cleanup
  if (scriptElement && scriptElement.parentNode) {
    scriptElement.parentNode.removeChild(scriptElement)
  }
  
  // Clean up global Module if it exists
  if ((window as any).Module === wasmModule) {
    delete (window as any).Module
  }

  // Emscripten main loop cleanup is handled automatically
  // The render loop will stop when the component is unmounted
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

