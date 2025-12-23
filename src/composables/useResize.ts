import { ref, onUnmounted } from 'vue'

export function useResize(
  minWidth: number,
  maxWidth: number,
  initialWidth: number,
  side: 'left' | 'right'
) {
  const width = ref(initialWidth)
  const isResizing = ref(false)
  const startX = ref(0)
  const startWidth = ref(0)

  const startResize = (e: MouseEvent): void => {
    isResizing.value = true
    startX.value = e.clientX
    startWidth.value = width.value
    document.addEventListener('mousemove', handleResize)
    document.addEventListener('mouseup', stopResize)
    document.body.style.cursor = 'col-resize'
    document.body.style.userSelect = 'none'
    e.preventDefault()
  }

  const handleResize = (e: MouseEvent): void => {
    if (!isResizing.value) return

    const delta = side === 'left'
      ? e.clientX - startX.value
      : startX.value - e.clientX

    let newWidth = startWidth.value + delta
    newWidth = Math.max(minWidth, Math.min(maxWidth, newWidth))

    width.value = newWidth
  }

  const stopResize = (): void => {
    isResizing.value = false
    document.removeEventListener('mousemove', handleResize)
    document.removeEventListener('mouseup', stopResize)
    document.body.style.cursor = ''
    document.body.style.userSelect = ''
  }

  onUnmounted(() => {
    stopResize()
  })

  return {
    width,
    isResizing,
    startResize
  }
}

