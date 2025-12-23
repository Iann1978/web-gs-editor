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
import { useSceneStore } from '@/stores/scene'
import { gsr } from '@/core/gsr/gsr'

const canvasRef = ref<HTMLCanvasElement | null>(null)
const error = ref<string>('')
const loading = ref<boolean>(true)
const sceneStore = useSceneStore()
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
      onInitialized: async (module: any) => {
        console.log('WASM module initialized and ready to use')
        
        // Manually call Init() and Start() now that the module is initialized
        try {
          // Call InitWebGPU() first
          if (module._InitWebGPU) {
            console.log('Calling InitWebGPU()...')
            module._InitWebGPU()
          } else if (module.ccall) {
            console.log('Calling InitWebGPU() via ccall...')
            module.ccall('InitWebGPU', null, [])
          } else {
            console.warn('InitWebGPU() function not found. Make sure it is exported in CMakeLists.txt')
            return
          }

          // Poll until initialization is complete
          const checkInitialized = (): boolean => {
            if (module._IsWebGPUInitialized) {
              return module._IsWebGPUInitialized() === 1
            } else if (module.ccall) {
              return module.ccall('IsWebGPUInitialized', 'number', []) === 1
            }
            return false
          }

          // Wait for initialization with timeout
          let attempts = 0
          const maxAttempts = 100  // 10 seconds max (100 * 100ms)
          const pollInterval = 100  // 100ms between checks

          console.log('Waiting for WebGPU initialization...')
          while (!checkInitialized() && attempts < maxAttempts) {
            await new Promise(resolve => setTimeout(resolve, pollInterval))
            attempts++
          }

          if (!checkInitialized()) {
            throw new Error('WebGPU initialization timed out after 10 seconds')
          }

          console.log('WebGPU initialization complete, starting render loop...')


          
           // Populate scene store from WASM scene
           const scene = gsr.getScene()
          if (scene && scene.ptr) {
            const count = scene.getEntityCount()
            console.log('Entities Count:', count)
            const list = []
            for (let i = 0; i < count; i++) {
              const ent = scene.getEntity(i)
              if (!ent) continue
              const name = ent.getName() || `Entity ${i}`
              list.push({
                id: `native-${i}`,
                name,
                type: 'other' as const,
                transform: {
                  position: [0, 0, 0] as [number, number, number],
                  rotation: [0, 0, 0, 1] as [number, number, number, number],
                  scale: [1, 1, 1] as [number, number, number]
                },
                visible: true,
                locked: false,
                children: []
              })
            }
            sceneStore.clearScene()
            list.forEach((e) => sceneStore.addEntity(e))
          }
          
          // Then call StartWebGPU()
          if (module._StartWebGPU) {
            console.log('Calling StartWebGPU()...')
            module._StartWebGPU()
          } else if (module.ccall) {
            console.log('Calling StartWebGPU() via ccall...')
            module.ccall('StartWebGPU', null, [])
          } else {
            console.warn('StartWebGPU() function not found. Make sure it is exported in CMakeLists.txt')
          }


        } catch (err) {
          console.error('Error initializing WebGPU:', err)
          error.value = `Failed to start WebGPU: ${err instanceof Error ? err.message : 'Unknown error'}`
        }
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

