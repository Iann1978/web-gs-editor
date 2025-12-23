import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import type { Tool, HistoryEntry } from '@/types/editor'

export const useEditorStore = defineStore('editor', () => {
  // Current tool
  const currentTool = ref<Tool>('select')

  // History for undo/redo
  const history = ref<HistoryEntry[]>([])
  const historyIndex = ref(-1)
  const maxHistorySize = 50

  // Set current tool
  const setTool = (tool: Tool): void => {
    currentTool.value = tool
  }

  // History management
  const addHistoryEntry = (entry: HistoryEntry): void => {
    // Remove any entries after current index (when undoing then making new changes)
    if (historyIndex.value < history.value.length - 1) {
      history.value = history.value.slice(0, historyIndex.value + 1)
    }

    // Add new entry
    history.value.push(entry)
    historyIndex.value = history.value.length - 1

    // Limit history size
    if (history.value.length > maxHistorySize) {
      history.value.shift()
      historyIndex.value--
    }
  }

  const undo = (): void => {
    const entry = history.value[historyIndex.value]
    if (canUndo.value && entry) {
      entry.undo()
      historyIndex.value--
    }
  }

  const redo = (): void => {
    if (canRedo.value && historyIndex.value < history.value.length - 1) {
      historyIndex.value++
      const entry = history.value[historyIndex.value]
      if (entry) {
        entry.redo()
      }
    }
  }

  const canUndo = computed(() => historyIndex.value >= 0)
  const canRedo = computed(() => historyIndex.value < history.value.length - 1)

  // Clear history
  const clearHistory = (): void => {
    history.value = []
    historyIndex.value = -1
  }

  return {
    // State
    currentTool,
    history,
    historyIndex,
    // Computed
    canUndo,
    canRedo,
    // Actions
    setTool,
    addHistoryEntry,
    undo,
    redo,
    clearHistory
  }
})

