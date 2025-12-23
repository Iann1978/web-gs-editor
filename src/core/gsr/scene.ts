import { EmscriptenWasmModule } from '../wasm/EmscriptenWasmModule'
import { Entity } from './entity'

// Placeholder Scene wrapper; extends with WASM-backed handle when available.
export class Scene {
  readonly ptr: number

  constructor(ptr: number) {
    this.ptr = ptr
  }

  // Returns entity count from native Scene if available; otherwise 0.
  getEntityCount(): number {
    const fn = EmscriptenWasmModule.fn_gsr_scene_get_entity_count!;
    try {
      const count = fn(this.ptr)
      return typeof count === 'number' ? count : 0
    } catch (err) {
      console.warn('Scene.getEntityCount: call failed', err)
      return 0
    }
  }

  // Returns an Entity wrapper for the given index if available; otherwise null.
  getEntity(index: number): Entity | null {
    const fn = EmscriptenWasmModule.fn_gsr_scene_get_entity!;
    try {
      const entityPtr = fn(this.ptr, index)
      if (typeof entityPtr !== 'number' || entityPtr === 0) {
        console.warn('Scene.getEntity: entity pointer is not available')
        return null
      }
      return new Entity(entityPtr)
    } catch (err) {
      console.warn('Scene.getEntity: call failed', err)
      return null
    }
  }
}
