<template>
  <div
    v-if="visible"
    class="resizable-panel"
    :class="{ 'panel-left': side === 'left', 'panel-right': side === 'right' }"
    :style="{ width: width + 'px' }"
  >
    <div class="panel-content">
      <slot />
    </div>
    <div
      class="resizer"
      :class="{ 'resizer-left': side === 'left', 'resizer-right': side === 'right' }"
      @mousedown="startResize"
    ></div>
  </div>
</template>

<script setup lang="ts">
import { ref, onUnmounted } from 'vue'

interface Props {
  width: number
  minWidth: number
  maxWidth: number
  side: 'left' | 'right'
  visible?: boolean
}

const props = withDefaults(defineProps<Props>(), {
  visible: true
})

const emit = defineEmits<{
  'update:width': [value: number]
}>()

const isResizing = ref(false)
const startX = ref(0)
const startWidth = ref(0)

const startResize = (e: MouseEvent) => {
  isResizing.value = true
  startX.value = e.clientX
  startWidth.value = props.width
  document.addEventListener('mousemove', handleResize)
  document.addEventListener('mouseup', stopResize)
  document.body.style.cursor = 'col-resize'
  document.body.style.userSelect = 'none'
  e.preventDefault()
}

const handleResize = (e: MouseEvent) => {
  if (!isResizing.value) return

  const delta = props.side === 'left'
    ? e.clientX - startX.value
    : startX.value - e.clientX

  let newWidth = startWidth.value + delta
  newWidth = Math.max(props.minWidth, Math.min(props.maxWidth, newWidth))

  emit('update:width', newWidth)
}

const stopResize = () => {
  isResizing.value = false
  document.removeEventListener('mousemove', handleResize)
  document.removeEventListener('mouseup', stopResize)
  document.body.style.cursor = ''
  document.body.style.userSelect = ''
}

onUnmounted(() => {
  stopResize()
})
</script>

<style scoped>
.resizable-panel {
  position: relative;
  display: flex;
  flex-direction: column;
  height: 100%;
  background: #252526;
  flex-shrink: 0;
}

.panel-content {
  flex: 1;
  overflow: hidden;
  display: flex;
  flex-direction: column;
}

.resizer {
  position: absolute;
  top: 0;
  bottom: 0;
  width: 4px;
  cursor: col-resize;
  background: transparent;
  z-index: 10;
  transition: background 0.1s;
}

.resizer-left {
  right: -2px;
}

.resizer-right {
  left: -2px;
}

.resizer:hover {
  background: #007acc;
}

.resizer::after {
  content: '';
  position: absolute;
  top: 0;
  bottom: 0;
  left: -2px;
  right: -2px;
}
</style>

