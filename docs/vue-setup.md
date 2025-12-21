# Vue 3 Familiarization Setup

## Overview

Set up Vue 3 + TypeScript + Vite project to get familiar with Vue 3 before building the editor.

## Prerequisites

Node.js 18.x+, npm 9.x+

## Project Setup

```bash
npm create vite@latest . -- --template vue-ts
npm install
npm run dev
```

Visit `http://localhost:5173` to see the app.

**Key Files:** `src/main.ts` (entry), `src/App.vue` (root), `vite.config.ts` (config)

## Vue 3 Essentials

**Composition API:** Use `<script setup>` with `ref()`, `computed()`, `onMounted()` from Vue

**Reactive Data:** `ref()` for primitives (`.value` in script), `reactive()` for objects, `computed()` for derived state

**Props/Emits:** `defineProps<Props>()`, `defineEmits<{ (e: 'event', value: type): void }>()`

**Template:** `{{ variable }}`, `v-if/v-else`, `v-for="item in items" :key="id"`, `:attr="value"`, `@click="handler"`, `v-model="var"`

## Quick Exercise

Counter component: `const count = ref(0)`, `@click="count++"` in template.

## Next Steps

Proceed to [Basic Viewer Development Plan](development-plan-basic-viewer.md) when ready.
