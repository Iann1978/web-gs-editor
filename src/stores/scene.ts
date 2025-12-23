import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import type { Entity } from '@/types/editor'

export const useSceneStore = defineStore('scene', () => {
  // Scene entities (flat list, parent-child relationships via parentId)
  const entities = ref<Entity[]>([])

  // Selected entity IDs
  const selectedIds = ref<string[]>([])

  // Get entity by ID
  const getEntity = (id: string): Entity | undefined => {
    return entities.value.find((e: Entity) => e.id === id)
  }

  // Get selected entities
  const getSelectedEntities = computed(() => {
    return entities.value.filter((e: Entity) => selectedIds.value.includes(e.id))
  })

  // Select entity
  const selectEntity = (id: string, multi: boolean = false): void => {
    if (multi) {
      const index = selectedIds.value.indexOf(id)
      if (index > -1) {
        selectedIds.value.splice(index, 1)
      } else {
        selectedIds.value.push(id)
      }
    } else {
      selectedIds.value = [id]
    }
  }

  // Clear selection
  const clearSelection = (): void => {
    selectedIds.value = []
  }

  // Add entity
  const addEntity = (entity: Entity): void => {
    entities.value.push(entity)
  }

  // Remove entity
  const removeEntity = (id: string): void => {
    const index = entities.value.findIndex((e: Entity) => e.id === id)
    if (index > -1) {
      entities.value.splice(index, 1)
      // Remove from selection if selected
      const selectedIndex = selectedIds.value.indexOf(id)
      if (selectedIndex > -1) {
        selectedIds.value.splice(selectedIndex, 1)
      }
      // Remove children recursively
      const children = entities.value.filter((e: Entity) => e.parentId === id)
      children.forEach((child: Entity) => removeEntity(child.id))
    }
  }

  // Update entity
  const updateEntity = (id: string, updates: Partial<Entity>): void => {
    const entity = getEntity(id)
    if (entity) {
      Object.assign(entity, updates)
    }
  }

  // Get children of an entity
  const getChildren = (parentId: string): Entity[] => {
    return entities.value.filter((e: Entity) => e.parentId === parentId)
  }

  // Build tree structure (for hierarchy panel)
  const getEntityTree = computed(() => {
    const tree: Entity[] = []
    const entityMap = new Map<string, Entity>()

    // Create map of all entities
    entities.value.forEach((entity: Entity) => {
      entityMap.set(entity.id, { ...entity, children: [] })
    })

    // Build tree structure
    entities.value.forEach((entity: Entity) => {
      const node = entityMap.get(entity.id)!
      if (entity.parentId && entityMap.has(entity.parentId)) {
        const parent = entityMap.get(entity.parentId)!
        if (!parent.children) {
          parent.children = []
        }
        parent.children.push(node)
      } else {
        tree.push(node)
      }
    })

    return tree
  })

  // Clear all entities
  const clearScene = (): void => {
    entities.value = []
    selectedIds.value = []
  }

  return {
    // State
    entities,
    selectedIds,
    // Computed
    getSelectedEntities,
    getEntityTree,
    // Actions
    getEntity,
    selectEntity,
    clearSelection,
    addEntity,
    removeEntity,
    updateEntity,
    getChildren,
    clearScene
  }
})

