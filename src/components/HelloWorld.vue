<script setup lang="ts">
import { useTestStore } from '../stores/test'
import { useMouse } from '@vueuse/core'
import { ref, onMounted } from 'vue'

defineProps<{ msg: string }>()

const testStore = useTestStore()
const { x, y } = useMouse()

// WASM testing
const wasmLoaded = ref(false)
const wasmError = ref('')
const wasmInstance = ref<any>(null)

// Test results
const addResult = ref<number | null>(null)
const multiplyResult = ref<number | null>(null)
const greetResult = ref<string>('')

// Input values for testing
const num1 = ref(10)
const num2 = ref(20)
const greetName = ref('Vue')

onMounted(async () => {
  try {
    // Import the WASM module
    const wasmModule = await import('../../wasm/pkg/test.js')
    wasmInstance.value = await wasmModule.default()
    wasmLoaded.value = true
    
    // Run initial tests
    testAdd()
    testMultiply()
    testGreet()
  } catch (error: any) {
    wasmError.value = `Error loading WASM: ${error?.message || error}`
    console.error('WASM Error:', error)
  }
})

const testAdd = () => {
  if (wasmInstance.value) {
    try {
      addResult.value = wasmInstance.value.ccall('add', 'number', ['number', 'number'], [num1.value, num2.value])
    } catch (error) {
      console.error('Error calling add:', error)
    }
  }
}

const testMultiply = () => {
  if (wasmInstance.value) {
    try {
      multiplyResult.value = wasmInstance.value.ccall('multiply', 'number', ['number', 'number'], [num1.value, num2.value])
    } catch (error) {
      console.error('Error calling multiply:', error)
    }
  }
}

const testGreet = () => {
  if (wasmInstance.value) {
    try {
      greetResult.value = wasmInstance.value.greet(greetName.value)
    } catch (error) {
      console.error('Error calling greet:', error)
    }
  }
}
</script>

<template>
  <h1>{{ msg }}</h1>

  <div class="card">
    <button type="button" @click="testStore.increment()">count is {{ testStore.count }}</button>
    <button type="button" @click="testStore.decrement()" style="margin-left: 10px;">Decrement</button>
    <p>
      Edit
      <code>components/HelloWorld.vue</code> to test HMR
    </p>
    <p style="margin-top: 10px; font-size: 0.9em; color: #666;">
      VueUse Mouse Position: X: {{ x }}, Y: {{ y }}
    </p>
    
    <!-- WASM Test Section -->
    <div style="margin-top: 20px; padding: 15px; background: #f0f0f0; border-radius: 8px; border: 1px solid #ddd;">
      <h3 style="margin-top: 0; margin-bottom: 15px;">WASM Module Test</h3>
      
      <div v-if="!wasmLoaded && !wasmError" style="color: #666;">
        <p>Loading WASM module...</p>
      </div>
      
      <div v-if="wasmError" style="color: red; padding: 10px; background: #ffe0e0; border-radius: 4px;">
        <strong>Error:</strong> {{ wasmError }}
      </div>
      
      <div v-if="wasmLoaded" style="display: flex; flex-direction: column; gap: 15px;">
        <!-- Add Function Test -->
        <div style="padding: 10px; background: white; border-radius: 4px;">
          <strong>Test add() function:</strong>
          <div style="margin-top: 8px; display: flex; align-items: center; gap: 8px;">
            <input v-model.number="num1" type="number" style="width: 80px; padding: 4px;" />
            <span> + </span>
            <input v-model.number="num2" type="number" style="width: 80px; padding: 4px;" />
            <button @click="testAdd" style="margin-left: 8px; padding: 4px 12px;">Calculate</button>
            <span v-if="addResult !== null" style="margin-left: 8px; font-weight: bold; color: #42b883;">
              = {{ addResult }}
            </span>
          </div>
        </div>
        
        <!-- Multiply Function Test -->
        <div style="padding: 10px; background: white; border-radius: 4px;">
          <strong>Test multiply() function:</strong>
          <div style="margin-top: 8px; display: flex; align-items: center; gap: 8px;">
            <input v-model.number="num1" type="number" style="width: 80px; padding: 4px;" />
            <span> × </span>
            <input v-model.number="num2" type="number" style="width: 80px; padding: 4px;" />
            <button @click="testMultiply" style="margin-left: 8px; padding: 4px 12px;">Calculate</button>
            <span v-if="multiplyResult !== null" style="margin-left: 8px; font-weight: bold; color: #42b883;">
              = {{ multiplyResult }}
            </span>
          </div>
        </div>
        
        <!-- Greet Function Test -->
        <div style="padding: 10px; background: white; border-radius: 4px;">
          <strong>Test greet() function:</strong>
          <div style="margin-top: 8px; display: flex; align-items: center; gap: 8px;">
            <input v-model="greetName" type="text" placeholder="Enter name" style="width: 150px; padding: 4px;" />
            <button @click="testGreet" style="margin-left: 8px; padding: 4px 12px;">Greet</button>
            <span v-if="greetResult" style="margin-left: 8px; font-weight: bold; color: #42b883;">
              {{ greetResult }}
            </span>
          </div>
        </div>
        
        <div style="margin-top: 10px; padding: 8px; background: #e8f5e9; border-radius: 4px; font-size: 0.9em; color: #2e7d32;">
          ✓ WASM module loaded successfully!
        </div>
      </div>
    </div>
  </div>

  <p>
    Check out
    <a href="https://vuejs.org/guide/quick-start.html#local" target="_blank"
      >create-vue</a
    >, the official Vue + Vite starter
  </p>
  <p>
    Learn more about IDE Support for Vue in the
    <a
      href="https://vuejs.org/guide/scaling-up/tooling.html#ide-support"
      target="_blank"
      >Vue Docs Scaling up Guide</a
    >.
  </p>
  <p class="read-the-docs">Click on the Vite and Vue logos to learn more</p>
</template>

<style scoped>
.read-the-docs {
  color: #888;
}
</style>
