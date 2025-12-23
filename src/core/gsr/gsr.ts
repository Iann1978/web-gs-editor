

import { Scene } from './scene'
import { EmscriptenWasmModule } from '../wasm/EmscriptenWasmModule'

export class gsr {
  // Returns a singleton Scene instance; will attach WASM scene pointer when available.
  public static getScene(): Scene | null {
    const ptr = EmscriptenWasmModule.fn_gsr_get_scene!();
    if (!ptr) {
      console.warn('gsr.getScene: gsr_get_scene pointer is not available')
      return null
    }
    return new Scene(ptr)
  }
}