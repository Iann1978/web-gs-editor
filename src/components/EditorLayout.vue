<template>
  <div class="editor-container">
    <!-- Toolbar -->
    <EditorToolbar class="editor-toolbar" />

    <!-- Main Content Area -->
    <div class="editor-content">
      <!-- Hierarchy Panel (Left) -->
      <ResizablePanel
        v-model:width="uiStore.hierarchyWidth"
        :min-width="150"
        :max-width="500"
        side="left"
        :visible="uiStore.hierarchyVisible"
        @update:width="uiStore.setHierarchyWidth"
      >
        <HierarchyPanel />
      </ResizablePanel>

      <!-- Viewport Canvas (Center) -->
      <div class="viewport-container">
        <ViewportCanvas />
      </div>

      <!-- Inspector Panel (Right) -->
      <ResizablePanel
        v-model:width="uiStore.inspectorWidth"
        :min-width="200"
        :max-width="600"
        side="right"
        :visible="uiStore.inspectorVisible"
        @update:width="uiStore.setInspectorWidth"
      >
        <InspectorPanel />
      </ResizablePanel>
    </div>

    <!-- Status Bar (Optional) -->
    <StatusBar v-if="uiStore.statusBarVisible" class="status-bar" />
  </div>
</template>

<script setup lang="ts">
import { useUIStore } from '@/stores/ui'
import EditorToolbar from './EditorToolbar.vue'
import HierarchyPanel from './HierarchyPanel.vue'
import ViewportCanvas from './ViewportCanvas.vue'
import InspectorPanel from './InspectorPanel.vue'
import StatusBar from './StatusBar.vue'
import ResizablePanel from './ResizablePanel.vue'

const uiStore = useUIStore()
</script>

<style scoped>
.editor-container {
  display: flex;
  flex-direction: column;
  height: 100vh;
  width: 100vw;
  overflow: hidden;
  background: #1e1e1e;
  color: #d4d4d4;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
}

.editor-toolbar {
  height: 56px;
  flex-shrink: 0;
  border-bottom: 1px solid #3e3e3e;
  background: #2d2d30;
}

.editor-content {
  display: flex;
  flex: 1;
  overflow: hidden;
  min-height: 0;
}

.viewport-container {
  flex: 1;
  min-width: 0;
  position: relative;
  background: #1e1e1e;
}

.status-bar {
  height: 28px;
  flex-shrink: 0;
  border-top: 1px solid #3e3e3e;
  background: #007acc;
}
</style>

