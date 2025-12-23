

import { Scene } from './scene'
import { EmscriptenWasmModule } from '../wasm/EmscriptenWasmModule'

export class gsr {
  private static _scene: Scene | null = null

  // Returns a singleton Scene instance; will attach WASM scene pointer when available.
  public static getScene(): Scene {
    if (!this._scene) {
      const ptr = this.getScenePtrFromWasm()
      this._scene = new Scene(ptr ?? undefined)
    }
    return this._scene
  }

  // Attempts to retrieve the native Scene* pointer exposed by WASM as get_scene().
  private static getScenePtrFromWasm(): number | null {
    // Emscripten exports C symbols with a leading underscore; try both.
    const fn =
      EmscriptenWasmModule.getExportedFunction<() => number>('_get_scene') ||
      EmscriptenWasmModule.getExportedFunction<() => number>('get_scene')
    if (!fn) {
      console.warn('gsr.getScene: get_scene function not found')
      return null
    }
    try {
      const ptr = fn()
      console.log('gsr.getScene: get_scene pointer:', ptr)
      return typeof ptr === 'number' ? ptr : null
    } catch (err) {
      console.warn('gsr.getScene: get_scene call failed', err)
      return null
    }
  }
}