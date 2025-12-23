import { EmscriptenWasmModule } from "../wasm/EmscriptenWasmModule"
import type { Transform } from '@/types/editor'

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

    getTransform(): Transform | null {
        if (!this.ptr) {
            console.warn('Entity.getTransform: Entity pointer is not available')
            return null
        }
        
        const getTransform = EmscriptenWasmModule.fn_gsr_entity_get_transform
        const malloc = EmscriptenWasmModule.fn_malloc
        const free = EmscriptenWasmModule.fn_free
        const memory = EmscriptenWasmModule.wasmMemory
        
        if (!getTransform || !malloc || !free || !memory) {
            console.warn('Entity.getTransform: required functions or memory not available')
            return null
        }
        
        try {
            // Allocate buffer for 10 floats (3 position + 4 rotation + 3 scale)
            const bufferSize = 10 * 4 // 40 bytes
            const bufferPtr = malloc(bufferSize)
            
            if (!bufferPtr || bufferPtr === 0) {
                console.warn('Entity.getTransform: failed to allocate buffer')
                return null
            }
            
            try {
                // Call WASM function to fill the buffer
                getTransform(this.ptr, bufferPtr)
                
                // Read the float values from WASM memory
                const view = new Float32Array(memory.buffer, bufferPtr, 10)
                
                return {
                    position: [view[0], view[1], view[2]] as [number, number, number],
                    rotation: [view[3], view[4], view[5], view[6]] as [number, number, number, number],
                    scale: [view[7], view[8], view[9]] as [number, number, number]
                }
            } finally {
                // Always free the buffer
                free(bufferPtr)
            }
        } catch (err) {
            console.warn('Entity.getTransform: call failed', err)
            return null
        }
    }
}