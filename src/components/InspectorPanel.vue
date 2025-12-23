<template>
  <div class="inspector-panel">
    <div class="panel-header">
      <h3 class="panel-title">Inspector</h3>
    </div>

    <div class="inspector-content">
      <div v-if="selectedEntities.length === 0" class="empty-state">
        <p>No selection</p>
        <p class="empty-hint">Select an entity to view properties</p>
      </div>

      <div v-else class="property-sections">
        <!-- Multiple Selection Info -->
        <div v-if="selectedEntities.length > 1" class="property-section">
          <div class="section-header">
            <span class="section-title">Selection</span>
          </div>
          <div class="section-content">
            <p class="selection-info">{{ selectedEntities.length }} entities selected</p>
            <p class="selection-hint">Only common properties are editable</p>
          </div>
        </div>

        <!-- Transform Section -->
        <div v-if="hasCommonProperty('transform')" class="property-section">
          <div class="section-header" @click="toggleSection('transform')">
            <span class="section-title">Transform</span>
            <span class="section-toggle">{{ expandedSections.transform ? '▼' : '▶' }}</span>
          </div>
          <div v-if="expandedSections.transform" class="section-content">
            <PropertyEditor
              v-for="(_, index) in transform.position"
              :key="`pos-${index}`"
              :label="['X', 'Y', 'Z'][index]"
              :model-value="transform.position[index] ?? 0"
              type="number"
              :step="0.1"
              @update:modelValue="(val) => updateTransform('position', index, val as number)"
            />
            <PropertyEditor
              v-for="(_, index) in transform.rotation"
              :key="`rot-${index}`"
              :label="['X', 'Y', 'Z', 'W'][index]"
              :model-value="transform.rotation[index] ?? (index === 3 ? 1 : 0)"
              type="number"
              :step="0.01"
              :min="-1"
              :max="1"
              @update:modelValue="(val) => updateTransform('rotation', index, val as number)"
            />
            <PropertyEditor
              v-for="(_, index) in transform.scale"
              :key="`scale-${index}`"
              :label="['X', 'Y', 'Z'][index]"
              :model-value="transform.scale[index] ?? 1"
              type="number"
              :step="0.1"
              :min="0.01"
              @update:modelValue="(val) => updateTransform('scale', index, val as number)"
            />
          </div>
        </div>

        <!-- Gaussian Splat Properties -->
        <div v-if="hasCommonProperty('gaussian')" class="property-section">
          <div class="section-header" @click="toggleSection('gaussian')">
            <span class="section-title">Gaussian Splat</span>
            <span class="section-toggle">{{ expandedSections.gaussian ? '▼' : '▶' }}</span>
          </div>
          <div v-if="expandedSections.gaussian" class="section-content">
            <PropertyEditor
              label="Opacity"
              v-model="gaussianProperties.opacity"
              type="number"
              :step="0.01"
              :min="0"
              :max="1"
              @update:modelValue="(val) => updateGaussianProperty('opacity', val as number)"
            />
            <!-- Add more Gaussian properties as needed -->
          </div>
        </div>

        <!-- Entity Info -->
        <div v-for="entity in selectedEntities" :key="entity.id" class="property-section">
          <div class="section-header" @click="toggleSection(`entity-${entity.id}`)">
            <span class="section-title">{{ entity.name }}</span>
            <span class="section-toggle">{{ expandedSections[`entity-${entity.id}`] ? '▼' : '▶' }}</span>
          </div>
          <div v-if="expandedSections[`entity-${entity.id}`]" class="section-content">
            <div class="property-row">
              <span class="property-label">ID:</span>
              <span class="property-value">{{ entity.id }}</span>
            </div>
            <div class="property-row">
              <span class="property-label">Type:</span>
              <span class="property-value">{{ entity.type }}</span>
            </div>
            <div class="property-row">
              <span class="property-label">Visible:</span>
              <input
                type="checkbox"
                :checked="entity.visible"
                @change="toggleVisibility(entity.id)"
                class="property-checkbox"
              />
            </div>
            <div class="property-row">
              <span class="property-label">Locked:</span>
              <input
                type="checkbox"
                :checked="entity.locked"
                @change="toggleLocked(entity.id)"
                class="property-checkbox"
              />
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, watch } from 'vue'
import { useSceneStore } from '@/stores/scene'
import { useEditorStore } from '@/stores/editor'
import PropertyEditor from './PropertyEditor.vue'
import type { Transform, Entity } from '@/types/editor'

const sceneStore = useSceneStore()
const editorStore = useEditorStore()

const expandedSections = ref<Record<string, boolean>>({
  transform: true,
  gaussian: true
})

const selectedEntities = computed(() => sceneStore.getSelectedEntities)

// Transform properties (for single or common values)
const transform = ref<Transform>({
  position: [0, 0, 0],
  rotation: [0, 0, 0, 1],
  scale: [1, 1, 1]
})

// Gaussian properties
const gaussianProperties = ref({
  opacity: 1.0
})

  // Check if all selected entities have a common property
  const hasCommonProperty = (property: string): boolean => {
    if (selectedEntities.value.length === 0) return false
    // For now, assume all entities have transform
    if (property === 'transform') return true
    // Check for Gaussian splat entities
    if (property === 'gaussian') {
      return selectedEntities.value.some((e: Entity) => e.type === 'gaussian')
    }
    return false
  }

  // Update transform property
  const updateTransform = (property: 'position' | 'rotation' | 'scale', index: number, value: number): void => {
    if (selectedEntities.value.length === 0) return

    // Create history entry
    const oldValues = selectedEntities.value.map((e: Entity) => ({ ...e.transform }))

    // Update all selected entities
    selectedEntities.value.forEach((entity: Entity) => {
      const newTransform = { ...entity.transform }
      newTransform[property][index] = value
      sceneStore.updateEntity(entity.id, { transform: newTransform })
    })

    // Add to history
    editorStore.addHistoryEntry({
      id: `transform-${Date.now()}`,
      type: 'modify',
      timestamp: Date.now(),
      data: { property, index, value, entities: selectedEntities.value.map((e: Entity) => e.id) },
      undo: () => {
        selectedEntities.value.forEach((entity: Entity, i: number) => {
          sceneStore.updateEntity(entity.id, { transform: oldValues[i] })
        })
      },
      redo: () => {
        selectedEntities.value.forEach((entity: Entity) => {
          const newTransform = { ...entity.transform }
          newTransform[property][index] = value
          sceneStore.updateEntity(entity.id, { transform: newTransform })
        })
      }
    })
  }

// Update Gaussian property
const updateGaussianProperty = (property: string, value: number): void => {
  // TODO: Implement Gaussian property updates
  console.log('Update Gaussian property:', property, value)
}

// Toggle section expansion
const toggleSection = (section: string): void => {
  expandedSections.value[section] = !expandedSections.value[section]
}

// Toggle visibility
const toggleVisibility = (id: string): void => {
  const entity = sceneStore.getEntity(id)
  if (entity) {
    sceneStore.updateEntity(id, { visible: !entity.visible })
  }
}

// Toggle locked
const toggleLocked = (id: string): void => {
  const entity = sceneStore.getEntity(id)
  if (entity) {
    sceneStore.updateEntity(id, { locked: !entity.locked })
  }
}

// Update transform when selection changes
watch(selectedEntities, (entities) => {
  if (entities.length === 1 && entities[0]) {
    // Single selection: use entity's transform
    transform.value = { ...entities[0].transform }
  } else if (entities.length > 1 && entities[0]) {
    // Multiple selection: use average or first entity's transform
    transform.value = { ...entities[0].transform }
  }
}, { immediate: true })
</script>

<style scoped>
.inspector-panel {
  display: flex;
  flex-direction: column;
  height: 100%;
  background: #252526;
  border-left: 1px solid #3e3e3e;
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
  margin: 0;
}

.inspector-content {
  flex: 1;
  overflow-y: auto;
  overflow-x: hidden;
  padding: 8px;
}

.inspector-content::-webkit-scrollbar {
  width: 10px;
}

.inspector-content::-webkit-scrollbar-track {
  background: #1e1e1e;
}

.inspector-content::-webkit-scrollbar-thumb {
  background: #424242;
  border-radius: 5px;
}

.inspector-content::-webkit-scrollbar-thumb:hover {
  background: #4e4e4e;
}

.empty-state {
  padding: 40px 20px;
  text-align: center;
  color: #858585;
}

.empty-hint {
  font-size: 11px;
  margin-top: 8px;
}

.property-sections {
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.property-section {
  background: #2d2d30;
  border-radius: 4px;
  overflow: hidden;
}

.section-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 8px 12px;
  background: #3c3c3c;
  cursor: pointer;
  user-select: none;
  transition: background 0.15s;
}

.section-header:hover {
  background: #404040;
}

.section-title {
  font-size: 12px;
  font-weight: 600;
  color: #cccccc;
}

.section-toggle {
  font-size: 10px;
  color: #858585;
}

.section-content {
  padding: 12px;
}

.property-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 6px 0;
  font-size: 12px;
}

.property-label {
  color: #cccccc;
}

.property-value {
  color: #858585;
  font-family: monospace;
}

.property-checkbox {
  cursor: pointer;
}

.selection-info {
  font-size: 12px;
  color: #cccccc;
  margin-bottom: 4px;
}

.selection-hint {
  font-size: 11px;
  color: #858585;
  font-style: italic;
}
</style>

