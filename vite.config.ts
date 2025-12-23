import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import wasm from 'vite-plugin-wasm'
import topLevelAwait from 'vite-plugin-top-level-await'
import path from 'path'

// https://vite.dev/config/
export default defineConfig({
  base: '/web-gs-editor/',
  plugins: [vue(), wasm(), topLevelAwait()],
  resolve: {
    alias: {
      '@': path.resolve(__dirname, 'src'),
      '@wasm': path.resolve(__dirname, 'wasm/pkg'),
    },
  },
  worker: {
    format: 'es',
    plugins: () => [wasm(), topLevelAwait()],
  },
  optimizeDeps: {
    exclude: [],
  },
})
