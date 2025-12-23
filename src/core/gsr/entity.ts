import { EmscriptenWasmModule } from "../wasm/EmscriptenWasmModule"


export class Entity {
    readonly ptr: number | undefined

    constructor(ptr?: number) {
      this.ptr = ptr
    }

    getName(): string {
        if (!this.ptr) return ''
        const getPtr = EmscriptenWasmModule.getExportedFunction<(entityPtr: number) => number>('gsr_entity_get_name')
        const getLen = EmscriptenWasmModule.getExportedFunction<(entityPtr: number) => number>('gsr_entity_get_name_len')
        const memory = EmscriptenWasmModule.wasmMemory
        if (!getPtr || !getLen || !memory) return ''
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