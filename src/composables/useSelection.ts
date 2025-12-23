import { computed } from 'vue'
import { useSceneStore } from '@/stores/scene'
import type { Entity } from '@/types/editor'

export function useSelection() {
  const sceneStore = useSceneStore()

  const selectedEntities = computed(() => sceneStore.getSelectedEntities)
  const selectedIds = computed(() => sceneStore.selectedIds)
  const hasSelection = computed(() => sceneStore.selectedIds.length > 0)
  const selectionCount = computed(() => sceneStore.selectedIds.length)

  const selectEntity = (id: string, multi: boolean = false): void => {
    sceneStore.selectEntity(id, multi)
  }

  const clearSelection = (): void => {
    sceneStore.clearSelection()
  }

  const selectAll = (entities: Entity[]): void => {
    sceneStore.selectedIds = entities.map(e => e.id)
  }

  const isSelected = (id: string): boolean => {
    return sceneStore.selectedIds.includes(id)
  }

  const toggleSelection = (id: string): void => {
    if (isSelected(id)) {
      const index = sceneStore.selectedIds.indexOf(id)
      sceneStore.selectedIds.splice(index, 1)
    } else {
      sceneStore.selectedIds.push(id)
    }
  }

  return {
    selectedEntities,
    selectedIds,
    hasSelection,
    selectionCount,
    selectEntity,
    clearSelection,
    selectAll,
    isSelected,
    toggleSelection
  }
}

