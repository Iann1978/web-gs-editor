import { EmscriptenWasmModule } from "../wasm/EmscriptenWasmModule"


interface Transform {
    position: [number, number, number]
    rotation: [number, number, number, number] // quaternion
    scale: [number, number, number]
}

export class Entity {
    readonly ptr: number

    constructor(ptr: number) {
      this.ptr = ptr
    }

    getName(): string {
        const getPtr = EmscriptenWasmModule.fn_gsr_entity_get_name!;
        const getLen = EmscriptenWasmModule.fn_gsr_entity_get_name_len!;
        const memory = EmscriptenWasmModule.wasmMemory!
        try {
            const ptr = getPtr(this.ptr)
            const len = getLen(this.ptr)
            if (typeof ptr !== 'number' || ptr === 0 || typeof len !== 'number' || len <= 0) {
                console.warn('Entity.getName: pointer or length is not available')
                return ''
            }
            const view = new Uint8Array(memory.buffer, ptr, len)
            return new TextDecoder('utf-8').decode(view)
        } catch (err) {
            console.warn('Entity.getName: call failed', err)
            return ''
        }
    }
}