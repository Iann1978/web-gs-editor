@echo off
REM Build script for Emscripten WASM modules
REM Emscripten SDK path: D:\dev\emsdk

set EMSDK_PATH=D:\dev\emsdk
set SRC_DIR=src
set OUTPUT_DIR=pkg

REM Activate Emscripten environment
call %EMSDK_PATH%\emsdk_env.bat

REM Create output directory if it doesn't exist
if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%

REM Compile test.cpp to WASM with WebGPU support
echo Compiling test.cpp to WASM...
emcc %SRC_DIR%\test.cpp ^
    -o %OUTPUT_DIR%\test.js ^
    -s MODULARIZE=1 ^
    -s EXPORT_ES6=1 ^
    -s USE_ES6_IMPORT_META=1 ^
    -s EXPORTED_RUNTIME_METHODS=["ccall","cwrap"] ^
    -s ALLOW_MEMORY_GROWTH=1 ^
    -s INITIAL_MEMORY=16777216 ^
    -lembind ^
    -O2 ^
    -std=c++17

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Output files in %OUTPUT_DIR%\
) else (
    echo Build failed!
    exit /b 1
)

