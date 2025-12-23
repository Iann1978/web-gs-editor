<template>
  <div class="viewport-canvas" ref="viewportRef">
    <canvas
      ref="canvasRef"
      id="editor-viewport"
      class="viewport-canvas-element"
      @mousedown="handleMouseDown"
      @mousemove="handleMouseMove"
      @mouseup="handleMouseUp"
      @wheel="handleWheel"
      @contextmenu.prevent
      tabindex="0"
    ></canvas>

    <!-- Viewport Overlays -->
    <div class="viewport-overlay">
      <div class="viewport-info">
        <span>Tool: {{ currentToolName }}</span>
        <span v-if="isDragging" class="viewport-drag-indicator">Dragging</span>
      </div>
    </div>

    <div v-if="loading" class="viewport-loading">
      <div class="loading-spinner"></div>
      <span>Loading WebGPU...</span>
    </div>

    <div v-if="error" class="viewport-error">
      {{ error }}
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, onUnmounted, watch } from 'vue'
import { useEditorStore } from '@/stores/editor'
import { useSceneStore } from '@/stores/scene'
import { gsr } from '@/core/gsr/gsr'
import { EmscriptenWasmModule } from '@/core/wasm/EmscriptenWasmModule'

const editorStore = useEditorStore()
const sceneStore = useSceneStore()

const canvasRef = ref<HTMLCanvasElement>()
const error = ref<string>('')
const loading = ref<boolean>(true)
const isDragging = ref(false)
const lastMousePos = ref({ x: 0, y: 0 })

const currentToolName = computed(() => {
  return editorStore.currentTool.charAt(0).toUpperCase() + editorStore.currentTool.slice(1)
})

// Initialize WebGPU renderer
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
        loading.value = false


        // Initialize WebGPU
        try {
          if (module._InitWebGPU) {
            console.log('Calling InitWebGPU()...')
            module._InitWebGPU()
          } else if (module.ccall) {
            console.log('Calling InitWebGPU() via ccall...')
            module.ccall('InitWebGPU', null, [])
          } else {
            console.warn('InitWebGPU() function not found')
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

          let attempts = 0
          const maxAttempts = 100
          const pollInterval = 100

          console.log('Waiting for WebGPU initialization...')
          while (!checkInitialized() && attempts < maxAttempts) {
            await new Promise(resolve => setTimeout(resolve, pollInterval))
            attempts++
          }

          if (!checkInitialized()) {
            throw new Error('WebGPU initialization timed out after 10 seconds')
          }

          console.log('WebGPU initialization complete, starting render loop...')



          // Start render loop
          if (module._StartWebGPU) {
            console.log('Calling StartWebGPU()...')
            module._StartWebGPU()
            console.log('StartWebGPU() called')

                      // Populate scene store from WASM scene
          const scene = gsr.getScene()
          console.log('Scene:', scene)
          console.log('Scene ptr:', scene.ptr)
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
          } else if (module.ccall) {
            console.log('Calling StartWebGPU() via ccall...')
            module.ccall('StartWebGPU', null, [])
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

// Mouse interaction handlers
const handleMouseDown = (e: MouseEvent): void => {
  if (!canvasRef.value) return

  isDragging.value = true
  lastMousePos.value = { x: e.clientX, y: e.clientY }

  // Handle selection or tool interaction
  if (editorStore.currentTool === 'select') {
    // TODO: Perform raycast for selection
    // For now, just log
    console.log('Select tool: mouse down at', e.clientX, e.clientY)
  } else {
    // TODO: Handle transform tools (move, rotate, scale)
    console.log(`${editorStore.currentTool} tool: mouse down`)
  }

  canvasRef.value.focus()
}

const handleMouseMove = (e: MouseEvent): void => {
  if (!isDragging.value) return

  // Update camera based on current tool
  switch (editorStore.currentTool) {
    case 'select':
      // TODO: Orbit camera
      break
    case 'move':
      // TODO: Pan camera
      break
    case 'rotate':
      // TODO: Rotate camera
      break
    case 'scale':
      // TODO: Zoom camera
      break
  }

  lastMousePos.value = { x: e.clientX, y: e.clientY }
}

const handleMouseUp = (): void => {
  isDragging.value = false
}

const handleWheel = (e: WheelEvent): void => {
  // Handle zoom
  e.preventDefault()
  // TODO: Implement zoom
  console.log('Wheel zoom:', e.deltaY)
}

// Watch for tool changes to update cursor
watch(() => editorStore.currentTool, (tool) => {
  if (canvasRef.value) {
    const cursors: Record<string, string> = {
      select: 'default',
      move: 'move',
      rotate: 'grab',
      scale: 'ns-resize'
    }
    canvasRef.value.style.cursor = cursors[tool] || 'default'
  }
})
</script>

<style scoped>
.viewport-canvas {
  width: 100%;
  height: 100%;
  position: relative;
  background: #1e1e1e;
  overflow: hidden;
}

.viewport-canvas-element {
  width: 100%;
  height: 100%;
  display: block;
  outline: none;
}

.viewport-overlay {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  pointer-events: none;
  z-index: 10;
}

.viewport-info {
  position: absolute;
  top: 8px;
  left: 8px;
  background: rgba(0, 0, 0, 0.7);
  padding: 6px 12px;
  border-radius: 4px;
  color: white;
  font-size: 12px;
  display: flex;
  gap: 12px;
  align-items: center;
}

.viewport-drag-indicator {
  color: #4ec9b0;
  font-weight: 500;
}

.viewport-loading {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 12px;
  color: #cccccc;
  font-size: 14px;
}

.loading-spinner {
  width: 32px;
  height: 32px;
  border: 3px solid #3e3e3e;
  border-top-color: #007acc;
  border-radius: 50%;
  animation: spin 1s linear infinite;
}

@keyframes spin {
  to {
    transform: rotate(360deg);
  }
}

.viewport-error {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  background: rgba(255, 68, 68, 0.9);
  color: white;
  padding: 16px 24px;
  border-radius: 4px;
  max-width: 400px;
  text-align: center;
  font-size: 14px;
}
</style>

