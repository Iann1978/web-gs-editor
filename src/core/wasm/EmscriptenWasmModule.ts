export interface EmscriptenModuleConfig {
  canvas?: HTMLCanvasElement | null
  wasmPath?: string
  jsPath?: string
  onStatusUpdate?: (text: string) => void
  onLoadingChange?: (loading: boolean) => void
  onError?: (error: string) => void
  onInitialized?: (module: any) => void
}

export class EmscriptenWasmModule {
  private static _module: any = undefined
  private static _scriptElement: HTMLScriptElement | null = null
  // Stored for potential future use (e.g., reinitialization)
  private static _config: EmscriptenModuleConfig | undefined = undefined

  public static fn_gsr_get_scene: (() => number) | undefined = undefined
  public static fn_gsr_scene_get_entity_count: ((scenePtr: number) => number) | undefined = undefined
  public static fn_gsr_scene_get_entity: ((scenePtr: number, idx: number) => number) | undefined = undefined
  public static fn_gsr_entity_get_name: ((entityPtr: number) => number) | undefined = undefined
  public static fn_gsr_entity_get_name_len: ((entityPtr: number) => number) | undefined = undefined

  static get config(): EmscriptenModuleConfig | undefined {
    return this._config
  }

  static get module(): any {
    return this._module
  }

  static get wasmMemory(): WebAssembly.Memory | undefined {
    return this._module?.wasmMemory
  }

  static async init(config: EmscriptenModuleConfig): Promise<void> {
    this._config = config

    // Check if WebGPU is supported (if needed)
    if (config.canvas && !('gpu' in navigator)) {
      const error = 'WebGPU is not supported in this browser'
      config.onError?.(error)
      throw new Error(error)
    }

    // Configure the Emscripten Module object before loading app.js
    const Module: any = {
      print(...args: any[]) {
        console.log(...args)
      },
      canvas: config.canvas || undefined,
      // Prevent automatic main() execution - we'll call it manually
      noInitialRun: true,
      setStatus(text: string) {
        if (text) {
          config.onStatusUpdate?.(text)
          console.log('Status:', text)
        }
      },
      totalDependencies: 0,
      monitorRunDependencies(left: number) {
        Module.totalDependencies = Math.max(Module.totalDependencies, left)
        if (left === 0) {
          config.onLoadingChange?.(false)
        }
      },
      locateFile(path: string) {
        // Ensure app.wasm is found at the correct path
        if (path.endsWith('.wasm')) {
          if (config.wasmPath) {
            return config.wasmPath
          }
          // Default path
          return new URL('../../wasm/pkg/app.wasm', import.meta.url).href
        }
        return path
      },
      onRuntimeInitialized() {
        // Called when WASM is ready
        EmscriptenWasmModule._module = Module
        
        EmscriptenWasmModule.fn_gsr_get_scene = Module.cwrap('gsr_get_scene', 'number', [])
        if (!EmscriptenWasmModule.fn_gsr_get_scene) {
          console.warn('EmscriptenWasmModule: gsr_get_scene function not found')
        }
        EmscriptenWasmModule.fn_gsr_scene_get_entity_count = Module.cwrap('gsr_scene_get_entity_count', 'number', ['number'])
        if (!EmscriptenWasmModule.fn_gsr_scene_get_entity_count) {
          console.warn('EmscriptenWasmModule: gsr_scene_get_entity_count function not found')
        }
        EmscriptenWasmModule.fn_gsr_scene_get_entity = Module.cwrap('gsr_scene_get_entity', 'number', ['number', 'number'])
        if (!EmscriptenWasmModule.fn_gsr_scene_get_entity) {
          console.warn('EmscriptenWasmModule: gsr_scene_get_entity function not found')
        }
        EmscriptenWasmModule.fn_gsr_entity_get_name = Module.cwrap('gsr_entity_get_name', 'number', ['number'])
        if (!EmscriptenWasmModule.fn_gsr_entity_get_name) {
          console.warn('EmscriptenWasmModule: gsr_entity_get_name function not found')
        }
        EmscriptenWasmModule.fn_gsr_entity_get_name_len = Module.cwrap('gsr_entity_get_name_len', 'number', ['number'])
        if (!EmscriptenWasmModule.fn_gsr_entity_get_name_len) {
          console.warn('EmscriptenWasmModule: gsr_entity_get_name_len function not found')
        }

        config.onLoadingChange?.(false)
        config.onInitialized?.(Module)
        console.log('WASM module initialized')
      }
    }

    // Make Module available globally (app.js expects it)
    ;(window as any).Module = Module

    // Handle WebGPU context loss
    if (config.canvas) {
      config.canvas.addEventListener('webglcontextlost', (e) => {
        console.error('WebGL context lost')
        const error = 'WebGPU context lost. Please reload the page.'
        config.onError?.(error)
        e.preventDefault()
      }, false)
    }

    // Handle window errors
    const originalErrorHandler = window.onerror
    window.onerror = (event) => {
      console.error('Error occurred:', event)
      const error = 'An error occurred. Check the console for details.'
      config.onError?.(error)
      config.onLoadingChange?.(false)
      if (originalErrorHandler) {
        return originalErrorHandler.call(window, event)
      }
      return false
    }

    // Dynamically load app.js
    this._scriptElement = document.createElement('script')
    this._scriptElement.type = 'text/javascript'
    this._scriptElement.async = true

    // Try to resolve the path
    try {
      const scriptUrl = config.jsPath 
        ? config.jsPath 
        : new URL('../../wasm/pkg/app.js', import.meta.url).href
      this._scriptElement.src = scriptUrl
    } catch (e) {
      // Fallback: assume files are in public folder or served from root
      this._scriptElement.src = config.jsPath || '/wasm/pkg/app.js'
    }

    // Wait for script to load
    await new Promise<void>((resolve, reject) => {
      this._scriptElement!.onload = () => {
        resolve()
      }
      this._scriptElement!.onerror = () => {
        const error = 'Failed to load app.js. Make sure wasm/pkg/app.js is accessible.'
        config.onError?.(error)
        reject(new Error(error))
      }
      document.head.appendChild(this._scriptElement!)
    })

    // Store reference to module
    this._module = Module
  }

  static cleanup(): void {
    // Cleanup script element
    if (this._scriptElement && this._scriptElement.parentNode) {
      this._scriptElement.parentNode.removeChild(this._scriptElement)
      this._scriptElement = null
    }

    // Clean up global Module if it exists
    if ((window as any).Module === this._module) {
      delete (window as any).Module
    }

    // Reset error handler
    window.onerror = null

    this._module = undefined
    this._config = undefined
  }

  // Helper method to access exported functions (similar to WasmModule pattern)
  static getExportedFunction<T extends (...args: any[]) => any>(name: string): T | undefined {
    if (!this._module) {
      console.warn(`EmscriptenWasmModule: Module not initialized. Cannot get function: ${name}`)
      return undefined
    }
    return this._module[name] as T | undefined
  }
}

