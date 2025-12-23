<template>
  <div class="hierarchy-panel">
    <div class="panel-header">
      <h3 class="panel-title">Hierarchy</h3>
      <input
        v-model="searchQuery"
        placeholder="Search..."
        class="search-input"
        type="text"
      />
    </div>

    <div class="tree-view" ref="treeViewRef">
      <div v-if="filteredTree.length === 0" class="empty-state">
        <p v-if="searchQuery">No entities found</p>
        <p v-else>No entities in scene</p>
      </div>
      <TreeNode
        v-for="entity in filteredTree"
        :key="entity.id"
        :entity="entity"
        :level="0"
      />
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted } from 'vue'
import { useSceneStore } from '@/stores/scene'
import TreeNode from './TreeNode.vue'
import type { Entity } from '@/types/editor'

const sceneStore = useSceneStore()
const searchQuery = ref('')

// Data is populated by ViewportCanvas after WASM init; no eager load here.
onMounted(() => {})

// Filter entities based on search query
const filteredTree = computed(() => {
  const tree = sceneStore.getEntityTree
  if (!searchQuery.value.trim()) {
    return tree
  }

  const query = searchQuery.value.toLowerCase()
  return filterTree(tree, query)
})

// Recursively filter tree
const filterTree = (entities: Entity[], query: string): Entity[] => {
  const result: Entity[] = []

  for (const entity of entities) {
    const matches = entity.name.toLowerCase().includes(query)
    const children = entity.children ? filterTree(entity.children, query) : []

    if (matches || children.length > 0) {
      result.push({
        ...entity,
        children: children.length > 0 ? children : entity.children
      })
    }
  }

  return result
}
</script>

<style scoped>
.hierarchy-panel {
  display: flex;
  flex-direction: column;
  height: 100%;
  background: #252526;
  border-right: 1px solid #3e3e3e;
}

.panel-header {
  padding: 8px;
  border-bottom: 1px solid #3e3e3e;
  background: #2d2d30;
}

.panel-title {
  font-size: 13px;
  font-weight: 600;
  color: #cccccc;
  margin: 0 0 8px 0;
}

.search-input {
  width: 100%;
  padding: 4px 8px;
  background: #3c3c3c;
  border: 1px solid #3e3e3e;
  color: #cccccc;
  border-radius: 3px;
  font-size: 12px;
  outline: none;
}

.search-input:focus {
  border-color: #007acc;
  background: #3e3e3e;
}

.search-input::placeholder {
  color: #858585;
}

.tree-view {
  flex: 1;
  overflow-y: auto;
  overflow-x: hidden;
  padding: 4px;
}

.tree-view::-webkit-scrollbar {
  width: 10px;
}

.tree-view::-webkit-scrollbar-track {
  background: #1e1e1e;
}

.tree-view::-webkit-scrollbar-thumb {
  background: #424242;
  border-radius: 5px;
}

.tree-view::-webkit-scrollbar-thumb:hover {
  background: #4e4e4e;
}

.empty-state {
  padding: 20px;
  text-align: center;
  color: #858585;
  font-size: 12px;
}
</style>

