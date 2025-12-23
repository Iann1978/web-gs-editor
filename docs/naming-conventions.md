# Naming Conventions

## Singleton Pattern Naming

This document describes the naming convention used for singleton classes in the codebase.

### Overview

Singleton classes in this codebase follow a consistent naming pattern for their static members:

- `ins`: Static pointer member that holds the singleton instance (stands for "instance")
- `Ref()`: Static method that returns a reference to the singleton instance

### Pattern Details

#### `ins` - Instance Pointer

The `ins` member is a static pointer that stores the singleton instance:

```cpp
static WebGPUContext* ins;
```

- Initialized to `nullptr`
- Set to `this` in the constructor if it's the first instance
- Used internally by the class to track the singleton instance

#### `Ref()` - Reference Accessor

The `Ref()` method provides access to the singleton instance:

```cpp
static WebGPUContext& Ref() {
    return *ins;
}
```

- Returns a reference to the singleton instance
- Preferred way to access the singleton from outside the class
- Should only be called after an instance has been created

### Usage Example

```cpp
// Create an instance (typically done during initialization)
WebGPUContext* context = new WebGPUContext();
context->Initialize();

// Access the singleton instance
WebGPUContext& ctx = WebGPUContext::Ref();
ctx.CreateSurface(window);
ctx.ConfigureSurface();
```

### Safety Requirements

**Important**: `Ref()` should only be called after a singleton instance has been created. Calling `Ref()` when `ins` is `nullptr` will result in undefined behavior.

### Consistency

This naming pattern (`ins` and `Ref()`) will be used consistently across all singleton classes in the codebase to maintain code readability and consistency.

### Reference Implementation

See `WebGPUContext` class in `wasm/src/gsr/WebGPUContext.h` and `wasm/src/gsr/WebGPUContext.cpp` for a complete example of this pattern.

