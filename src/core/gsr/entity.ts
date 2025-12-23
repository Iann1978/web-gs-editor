import { EmscriptenWasmModule } from "../wasm/EmscriptenWasmModule"


export class Entity {
    readonly ptr: number | undefined

    constructor(ptr?: number) {
      this.ptr = ptr
    }

    getName(): string {
        if (!this.ptr) {
            console.warn('Entity.getName: Entity pointer is not available')
            return ''
        }
        const getPtr = EmscriptenWasmModule.getExportedFunction<(entityPtr: number) => number>('_gsr_entity_get_name') ||
        EmscriptenWasmModule.getExportedFunction<(entityPtr: number) => number>('gsr_entity_get_name')
        if (!getPtr) {
            console.warn('Entity.getName: gsr_entity_get_name function not found')
            return ''
        }
        const getLen = EmscriptenWasmModule.getExportedFunction<(entityPtr: number) => number>('_gsr_entity_get_name_len') ||
        EmscriptenWasmModule.getExportedFunction<(entityPtr: number) => number>('gsr_entity_get_name_len')
        if (!getLen) {
            console.warn('Entity.getName: gsr_entity_get_name_len function not found')
            return ''
        }
        const memory = EmscriptenWasmModule.wasmMemory
        if (!memory) {
            console.warn('Entity.getName: wasmMemory is not available')
            return ''
        }
        try {
            const ptr = getPtr(this.ptr)
            const len = getLen(this.ptr)
            if (typeof ptr !== 'number' || ptr === 0 || typeof len !== 'number' || len <= 0) return ''
            const view = new Uint8Array(memory.buffer, ptr, len)
            return new TextDecoder('utf-8').decode(view)
        } catch (err) {
            console.warn('Entity.getName: call failed', err)
            return ''
        }
    }
}