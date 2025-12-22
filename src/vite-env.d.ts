/// <reference types="vite/client" />

// Type declarations for WASM modules
interface WasmModule {
  ccall(
    functionName: string,
    returnType: string,
    argTypes: string[],
    args: any[]
  ): any
  greet(name: string): string
  [key: string]: any
}

declare module '@wasm/test.js' {
  const wasmModule: () => Promise<WasmModule>
  export default wasmModule
}

