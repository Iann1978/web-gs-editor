<template>
  <div class="status-bar">
    <!-- Left: Selection Info & Coordinates -->
    <div class="status-section status-left">
      <span v-if="selectionInfo" class="status-item">
        {{ selectionInfo }}
      </span>
      <span v-if="coordinates" class="status-item">
        {{ coordinates }}
      </span>
    </div>

    <!-- Center: Status Messages & Progress -->
    <div class="status-section status-center">
      <span v-if="statusMessage" class="status-item" :class="{ error: isError, warning: isWarning }">
        {{ statusMessage }}
      </span>
      <div v-if="isLoading" class="status-progress">
        <span class="status-spinner">⏳</span>
        <span>Loading...</span>
      </div>
    </div>

    <!-- Right: Performance & File Status -->
    <div class="status-section status-right">
      <span v-if="fps !== null" class="status-item">
        FPS: {{ fps.toFixed(1) }}
      </span>
      <span v-if="frameTime !== null" class="status-item">
        {{ frameTime.toFixed(2) }}ms
      </span>
      <span v-if="fileStatus" class="status-item" :class="{ 'file-unsaved': !fileSaved }">
        {{ fileStatus }}
      </span>
      <span class="status-item">
        Tool: {{ currentTool }}
      </span>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, onUnmounted } from 'vue'
import { useSceneStore } from '@/stores/scene'
import { useEditorStore } from '@/stores/editor'

const sceneStore = useSceneStore()
const editorStore = useEditorStore()

// Status message
const statusMessage = ref('')
const isError = ref(false)
const isWarning = ref(false)
const isLoading = ref(false)

// Performance metrics
const fps = ref<number | null>(null)
const frameTime = ref<number | null>(null)
let frameCount = 0
let lastTime = performance.now()
let animationFrameId: number | null = null

// File status
const fileSaved = ref(true)
const fileStatus = ref('')

// Coordinates (cursor position in 3D space)
const coordinates = ref('')

// Selection info
const selectionInfo = computed(() => {
  const count = sceneStore.selectedIds.length
  if (count === 0) {
    return 'No selection'
  } else if (count === 1) {
    const firstId = sceneStore.selectedIds[0]
    if (firstId) {
      const entity = sceneStore.getEntity(firstId)
      return entity ? `Selected: ${entity.name}` : '1 selected'
    }
    return '1 selected'
  } else {
    return `${count} selected`
  }
})

// Current tool
const currentTool = computed(() => {
  return editorStore.currentTool.charAt(0).toUpperCase() + editorStore.currentTool.slice(1)
})

// FPS calculation
const updatePerformance = (): void => {
  frameCount++
  const currentTime = performance.now()
  const delta = currentTime - lastTime

  if (delta >= 1000) {
    fps.value = (frameCount * 1000) / delta
    frameTime.value = delta / frameCount
    frameCount = 0
    lastTime = currentTime
  }

  animationFrameId = requestAnimationFrame(updatePerformance)
}

// Set status message
const setStatus = (message: string, type: 'info' | 'error' | 'warning' = 'info', duration: number = 3000): void => {
  statusMessage.value = message
  isError.value = type === 'error'
  isWarning.value = type === 'warning'

  if (duration > 0) {
    setTimeout(() => {
      statusMessage.value = ''
      isError.value = false
      isWarning.value = false
    }, duration)
  }
}

// Set loading state
const setLoading = (loading: boolean): void => {
  isLoading.value = loading
}

// Set file status
const setFileStatus = (saved: boolean, filename?: string): void => {
  fileSaved.value = saved
  if (filename) {
    fileStatus.value = saved ? `Saved: ${filename}` : `Unsaved: ${filename}`
  } else {
    fileStatus.value = saved ? 'Saved' : 'Unsaved'
  }
}

// Set coordinates
const setCoordinates = (x: number, y: number, z: number): void => {
  coordinates.value = `X: ${x.toFixed(2)} Y: ${y.toFixed(2)} Z: ${z.toFixed(2)}`
}

onMounted(() => {
  lastTime = performance.now()
  updatePerformance()
})

onUnmounted(() => {
  if (animationFrameId !== null) {
    cancelAnimationFrame(animationFrameId)
  }
})

// Expose methods for external use
defineExpose({
  setStatus,
  setLoading,
  setFileStatus,
  setCoordinates
})
</script>

<style scoped>
.status-bar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0 8px;
  background: #007acc;
  color: white;
  font-size: 12px;
  user-select: none;
  height: 100%;
}

.status-section {
  display: flex;
  align-items: center;
  gap: 12px;
}

.status-left {
  flex: 0 0 auto;
}

.status-center {
  flex: 1;
  justify-content: center;
}

.status-right {
  flex: 0 0 auto;
  justify-content: flex-end;
}

.status-item {
  white-space: nowrap;
  padding: 2px 6px;
  border-radius: 2px;
}

.status-item.error {
  background: rgba(255, 0, 0, 0.3);
}

.status-item.warning {
  background: rgba(255, 165, 0, 0.3);
}

.status-item.file-unsaved {
  color: #ffcc00;
  font-weight: 500;
}

.status-progress {
  display: flex;
  align-items: center;
  gap: 6px;
}

.status-spinner {
  animation: spin 1s linear infinite;
}

@keyframes spin {
  from {
    transform: rotate(0deg);
  }
  to {
    transform: rotate(360deg);
  }
}
</style>

