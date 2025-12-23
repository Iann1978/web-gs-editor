<template>
  <div class="toolbar">
    <!-- File Operations -->
    <div class="toolbar-group">
      <button @click="handleNew" title="New Scene (Ctrl+N)" class="toolbar-button">
        <span class="toolbar-icon">📄</span>
        <span class="toolbar-label">New</span>
      </button>
      <button @click="handleOpen" title="Open File (Ctrl+O)" class="toolbar-button">
        <span class="toolbar-icon">📂</span>
        <span class="toolbar-label">Open</span>
      </button>
      <button @click="handleSave" title="Save (Ctrl+S)" class="toolbar-button">
        <span class="toolbar-icon">💾</span>
        <span class="toolbar-label">Save</span>
      </button>
    </div>

    <div class="toolbar-divider"></div>

    <!-- Edit Operations -->
    <div class="toolbar-group">
      <button
        @click="editorStore.undo"
        :disabled="!editorStore.canUndo"
        title="Undo (Ctrl+Z)"
        class="toolbar-button"
      >
        <span class="toolbar-icon">↶</span>
        <span class="toolbar-label">Undo</span>
      </button>
      <button
        @click="editorStore.redo"
        :disabled="!editorStore.canRedo"
        title="Redo (Ctrl+Y)"
        class="toolbar-button"
      >
        <span class="toolbar-icon">↷</span>
        <span class="toolbar-label">Redo</span>
      </button>
    </div>

    <div class="toolbar-divider"></div>

    <!-- Tool Selection -->
    <div class="toolbar-group">
      <button
        v-for="tool in tools"
        :key="tool"
        :class="['toolbar-button', 'tool-button', { active: editorStore.currentTool === tool }]"
        @click="editorStore.setTool(tool)"
        :title="getToolTitle(tool)"
      >
        <span class="toolbar-icon">{{ getToolIcon(tool) }}</span>
        <span class="toolbar-label">{{ tool.charAt(0).toUpperCase() + tool.slice(1) }}</span>
      </button>
    </div>

    <div class="toolbar-divider"></div>

    <!-- View Controls -->
    <div class="toolbar-group">
      <button @click="handleResetCamera" title="Reset Camera" class="toolbar-button">
        <span class="toolbar-icon">🎥</span>
        <span class="toolbar-label">Reset Camera</span>
      </button>
      <button @click="handleFocusSelection" title="Focus Selection (F)" class="toolbar-button">
        <span class="toolbar-icon">🔍</span>
        <span class="toolbar-label">Focus</span>
      </button>
    </div>

    <div class="toolbar-spacer"></div>

    <!-- Panel Toggles -->
    <div class="toolbar-group">
      <button
        @click="uiStore.toggleHierarchy"
        :class="['toolbar-button', { active: uiStore.hierarchyVisible }]"
        title="Toggle Hierarchy Panel"
      >
        <span class="toolbar-icon">📋</span>
      </button>
      <button
        @click="uiStore.toggleInspector"
        :class="['toolbar-button', { active: uiStore.inspectorVisible }]"
        title="Toggle Inspector Panel"
      >
        <span class="toolbar-icon">⚙️</span>
      </button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { onMounted, onUnmounted } from 'vue'
import { useEditorStore } from '@/stores/editor'
import { useUIStore } from '@/stores/ui'
import type { Tool } from '@/types/editor'

const editorStore = useEditorStore()
const uiStore = useUIStore()

const tools: Tool[] = ['select', 'move', 'rotate', 'scale']

const getToolIcon = (tool: Tool): string => {
  const icons: Record<Tool, string> = {
    select: '👆',
    move: '↔️',
    rotate: '🔄',
    scale: '📏'
  }
  return icons[tool] || '👆'
}

const getToolTitle = (tool: Tool): string => {
  const titles: Record<Tool, string> = {
    select: 'Select Tool (Q)',
    move: 'Move Tool (W)',
    rotate: 'Rotate Tool (E)',
    scale: 'Scale Tool (R)'
  }
  return titles[tool] || 'Select Tool'
}

const handleNew = (): void => {
  // TODO: Implement new scene
  console.log('New scene')
}

const handleOpen = (): void => {
  // TODO: Implement file open dialog
  console.log('Open file')
}

const handleSave = (): void => {
  // TODO: Implement save
  console.log('Save')
}

const handleResetCamera = (): void => {
  // TODO: Implement camera reset
  console.log('Reset camera')
}

const handleFocusSelection = (): void => {
  // TODO: Implement focus on selection
  console.log('Focus selection')
}

// Keyboard shortcuts
const handleKeyDown = (e: KeyboardEvent): void => {
  // Ctrl/Cmd combinations
  if (e.ctrlKey || e.metaKey) {
    switch (e.key.toLowerCase()) {
      case 's':
        e.preventDefault()
        handleSave()
        break
      case 'o':
        e.preventDefault()
        handleOpen()
        break
      case 'n':
        e.preventDefault()
        handleNew()
        break
      case 'z':
        if (e.shiftKey) {
          e.preventDefault()
          editorStore.redo()
        } else {
          e.preventDefault()
          editorStore.undo()
        }
        break
      case 'y':
        e.preventDefault()
        editorStore.redo()
        break
    }
    return
  }

  // Tool shortcuts (only when not in input field)
  if (e.target instanceof HTMLInputElement || e.target instanceof HTMLTextAreaElement) {
    return
  }

  switch (e.key.toLowerCase()) {
    case 'q':
      e.preventDefault()
      editorStore.setTool('select')
      break
    case 'w':
      e.preventDefault()
      editorStore.setTool('move')
      break
    case 'e':
      e.preventDefault()
      editorStore.setTool('rotate')
      break
    case 'r':
      e.preventDefault()
      editorStore.setTool('scale')
      break
    case 'f':
      e.preventDefault()
      handleFocusSelection()
      break
  }
}

onMounted(() => {
  window.addEventListener('keydown', handleKeyDown)
})

onUnmounted(() => {
  window.removeEventListener('keydown', handleKeyDown)
})
</script>

<style scoped>
.toolbar {
  display: flex;
  align-items: center;
  padding: 0 8px;
  gap: 4px;
  background: #2d2d30;
  user-select: none;
}

.toolbar-group {
  display: flex;
  gap: 2px;
  align-items: center;
}

.toolbar-divider {
  width: 1px;
  height: 24px;
  background: #3e3e3e;
  margin: 0 4px;
}

.toolbar-spacer {
  flex: 1;
}

.toolbar-button {
  display: flex;
  align-items: center;
  gap: 4px;
  padding: 6px 12px;
  background: transparent;
  border: none;
  color: #cccccc;
  cursor: pointer;
  border-radius: 3px;
  font-size: 13px;
  transition: background 0.15s;
}

.toolbar-button:hover:not(:disabled) {
  background: #3e3e3e;
}

.toolbar-button.active {
  background: #007acc;
  color: white;
}

.toolbar-button:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.toolbar-icon {
  font-size: 14px;
  line-height: 1;
}

.toolbar-label {
  font-size: 13px;
}

.tool-button {
  min-width: 70px;
}
</style>

