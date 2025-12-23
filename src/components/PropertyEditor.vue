<template>
  <div class="property-editor">
    <label v-if="label" class="property-label" :for="inputId">
      {{ label }}
      <span v-if="required" class="required">*</span>
    </label>
    <input
      :id="inputId"
      :type="type"
      :value="modelValue"
      :min="min"
      :max="max"
      :step="step"
      :disabled="disabled"
      class="property-input"
      @input="handleInput"
      @blur="handleBlur"
    />
    <span v-if="error" class="property-error">{{ error }}</span>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'

interface Props {
  label?: string
  modelValue: number | string
  type?: 'text' | 'number'
  min?: number
  max?: number
  step?: number
  required?: boolean
  disabled?: boolean
  validator?: (value: number | string) => string | null
}

const props = withDefaults(defineProps<Props>(), {
  type: 'text',
  required: false,
  disabled: false,
  step: 0.01
})

const emit = defineEmits<{
  'update:modelValue': [value: number | string]
}>()

const error = ref<string | null>(null)
const inputId = computed(() => `property-${Math.random().toString(36).substr(2, 9)}`)

const handleInput = (e: Event): void => {
  const target = e.target as HTMLInputElement
  let value: number | string = target.value

  if (props.type === 'number') {
    value = parseFloat(value) || 0
  }

  error.value = props.validator ? props.validator(value) : null
  emit('update:modelValue', value)
}

const handleBlur = (): void => {
  // Validate on blur
  if (props.validator) {
    error.value = props.validator(props.modelValue)
  }
}
</script>

<style scoped>
.property-editor {
  display: flex;
  flex-direction: column;
  gap: 4px;
  margin-bottom: 12px;
}

.property-label {
  font-size: 12px;
  color: #cccccc;
  font-weight: 500;
}

.required {
  color: #f48771;
}

.property-input {
  padding: 4px 8px;
  background: #3c3c3c;
  border: 1px solid #3e3e3e;
  color: #cccccc;
  border-radius: 3px;
  font-size: 13px;
  outline: none;
  transition: border-color 0.15s;
}

.property-input:focus {
  border-color: #007acc;
  background: #3e3e3e;
}

.property-input:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.property-error {
  font-size: 11px;
  color: #f48771;
}
</style>

