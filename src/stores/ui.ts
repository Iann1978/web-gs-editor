import { defineStore } from 'pinia'
import { ref } from 'vue'
import type { PanelLayout } from '@/types/editor'

export const useUIStore = defineStore('ui', () => {
  // Panel visibility
  const hierarchyVisible = ref(true)
  const inspectorVisible = ref(true)
  const statusBarVisible = ref(true)

  // Theme
  const theme = ref<'dark' | 'light'>('dark')

  // Panel widths (defaults from design doc)
  const hierarchyWidth = ref(280)
  const inspectorWidth = ref(320)

  // Load saved layout from localStorage
  const loadLayout = (): void => {
    const saved = localStorage.getItem('editor-layout')
    if (saved) {
      try {
        const layout: PanelLayout = JSON.parse(saved)
        hierarchyWidth.value = layout.hierarchyWidth || 280
        inspectorWidth.value = layout.inspectorWidth || 320
        hierarchyVisible.value = layout.hierarchyVisible ?? true
        inspectorVisible.value = layout.inspectorVisible ?? true
        statusBarVisible.value = layout.statusBarVisible ?? true
      } catch (e) {
        console.warn('Failed to load saved layout:', e)
      }
    }
  }

  // Save layout to localStorage
  const saveLayout = (): void => {
    const layout: PanelLayout = {
      hierarchyWidth: hierarchyWidth.value,
      inspectorWidth: inspectorWidth.value,
      hierarchyVisible: hierarchyVisible.value,
      inspectorVisible: inspectorVisible.value,
      statusBarVisible: statusBarVisible.value
    }
    localStorage.setItem('editor-layout', JSON.stringify(layout))
  }

  // Toggle functions
  const toggleHierarchy = (): void => {
    hierarchyVisible.value = !hierarchyVisible.value
    saveLayout()
  }

  const toggleInspector = (): void => {
    inspectorVisible.value = !inspectorVisible.value
    saveLayout()
  }

  const toggleStatusBar = (): void => {
    statusBarVisible.value = !statusBarVisible.value
    saveLayout()
  }

  // Update panel width
  const setHierarchyWidth = (width: number): void => {
    hierarchyWidth.value = Math.max(150, Math.min(500, width))
    saveLayout()
  }

  const setInspectorWidth = (width: number): void => {
    inspectorWidth.value = Math.max(200, Math.min(600, width))
    saveLayout()
  }

  // Initialize on store creation
  loadLayout()

  return {
    // State
    hierarchyVisible,
    inspectorVisible,
    statusBarVisible,
    theme,
    hierarchyWidth,
    inspectorWidth,
    // Actions
    toggleHierarchy,
    toggleInspector,
    toggleStatusBar,
    setHierarchyWidth,
    setInspectorWidth,
    loadLayout,
    saveLayout
  }
})

