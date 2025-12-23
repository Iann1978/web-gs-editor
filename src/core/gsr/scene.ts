import { EmscriptenWasmModule } from '../wasm/EmscriptenWasmModule'
import { Entity } from './entity'

// Placeholder Scene wrapper; extends with WASM-backed handle when available.
export class Scene {
  readonly ptr: number | undefined

  constructor(ptr?: number) {
    this.ptr = ptr
  }

  // Returns entity count from native Scene if available; otherwise 0.
  getEntityCount(): number {
    if (!this.ptr) {
      console.warn('Scene.getEntityCount: Scene pointer is not available')
      return 0
    }
    const fn = 
      EmscriptenWasmModule.getExportedFunction<(scenePtr: number) => number>('_gsr_scene_get_entity_count') ||
      EmscriptenWasmModule.getExportedFunction<(scenePtr: number) => number>('gsr_scene_get_entity_count')
    if (!fn) {
      console.warn('Scene.getEntityCount: gsr_scene_get_entity_count function not found')
      return 0
    } 
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
    if (!this.ptr) {
      console.warn('Scene.getEntity: Scene pointer is not available')
      return null
    }
    const fn = 
      EmscriptenWasmModule.getExportedFunction<(scenePtr: number, idx: number) => number>('_gsr_scene_get_entity') ||
      EmscriptenWasmModule.getExportedFunction<(scenePtr: number, idx: number) => number>('gsr_scene_get_entity')
    if (!fn) {
      console.warn('Scene.getEntity: gsr_scene_get_entity function not found')
      return null
    }
    try {
      const entityPtr = fn(this.ptr, index)
      if (typeof entityPtr !== 'number' || entityPtr === 0) return null
      return new Entity(entityPtr)
    } catch (err) {
      console.warn('Scene.getEntity: call failed', err)
      return null
    }
  }
}
