<template>
  <div class="tree-node">
    <div
      class="tree-node-content"
      :class="{
        selected: isSelected,
        expanded: isExpanded,
        hasChildren: hasChildren
      }"
      :style="{ paddingLeft: level * 16 + 8 + 'px' }"
      @click.stop="handleClick"
      @contextmenu.prevent="handleContextMenu"
    >
      <!-- Expand/Collapse Icon -->
      <span
        v-if="hasChildren"
        class="tree-node-toggle"
        @click.stop="toggleExpand"
      >
        {{ isExpanded ? '▼' : '▶' }}
      </span>
      <span v-else class="tree-node-spacer"></span>

      <!-- Entity Icon -->
      <span class="tree-node-icon">{{ getEntityIcon(entity.type) }}</span>

      <!-- Entity Name -->
      <span class="tree-node-name" :title="entity.name">
        {{ entity.name }}
      </span>

      <!-- Visibility Toggle -->
      <button
        class="tree-node-visibility"
        @click.stop="toggleVisibility"
        :title="entity.visible ? 'Hide' : 'Show'"
      >
        {{ entity.visible ? '👁️' : '🚫' }}
      </button>
    </div>

    <!-- Children -->
    <div v-if="isExpanded && hasChildren" class="tree-node-children">
      <TreeNode
        v-for="child in entity.children"
        :key="child.id"
        :entity="child"
        :level="level + 1"
      />
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import { useSceneStore } from '@/stores/scene'
import type { Entity, EntityType } from '@/types/editor'

interface Props {
  entity: Entity
  level?: number
}

const props = withDefaults(defineProps<Props>(), {
  level: 0
})

const sceneStore = useSceneStore()
const isExpanded = ref(true)

const hasChildren = computed(() => {
  return props.entity.children && props.entity.children.length > 0
})

const isSelected = computed(() => {
  return sceneStore.selectedIds.includes(props.entity.id)
})

const getEntityIcon = (type: EntityType): string => {
  const icons: Record<EntityType, string> = {
    mesh: '📦',
    light: '💡',
    camera: '📷',
    gaussian: '✨',
    group: '📁',
    other: '📄'
  }
  return icons[type] ?? icons.other
}

const toggleExpand = (): void => {
  isExpanded.value = !isExpanded.value
}

const handleClick = (e: MouseEvent): void => {
  const multi = e.ctrlKey || e.metaKey || e.shiftKey
  sceneStore.selectEntity(props.entity.id, multi)
}

const toggleVisibility = (): void => {
  sceneStore.updateEntity(props.entity.id, {
    visible: !props.entity.visible
  })
}

const handleContextMenu = (): void => {
  // TODO: Implement context menu
  console.log('Context menu for:', props.entity.name)
}
</script>

<style scoped>
.tree-node {
  user-select: none;
}

.tree-node-content {
  display: flex;
  align-items: center;
  gap: 4px;
  padding: 2px 4px;
  cursor: pointer;
  border-radius: 2px;
  transition: background 0.1s;
  min-height: 22px;
}

.tree-node-content:hover {
  background: rgba(255, 255, 255, 0.05);
}

.tree-node-content.selected {
  background: #007acc;
  color: white;
}

.tree-node-toggle {
  width: 12px;
  height: 12px;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 10px;
  color: #cccccc;
  cursor: pointer;
  flex-shrink: 0;
}

.tree-node-spacer {
  width: 12px;
  flex-shrink: 0;
}

.tree-node-icon {
  font-size: 14px;
  flex-shrink: 0;
}

.tree-node-name {
  flex: 1;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
  font-size: 13px;
}

.tree-node-visibility {
  background: transparent;
  border: none;
  cursor: pointer;
  padding: 2px 4px;
  font-size: 12px;
  opacity: 0.6;
  transition: opacity 0.1s;
  flex-shrink: 0;
}

.tree-node-content:hover .tree-node-visibility {
  opacity: 1;
}

.tree-node-visibility:hover {
  opacity: 1;
}

.tree-node-children {
  margin-left: 0;
}
</style>

