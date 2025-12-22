@echo off
REM Build script for Emscripten WASM modules (app.js/app.wasm + test.js/test.wasm)

set EMSDK_PATH=D:\dev\emsdk
set BUILD_DIR=build-web

REM Activate Emscripten environment
call %EMSDK_PATH%\emsdk_env.bat
if %ERRORLEVEL% NEQ 0 (
    echo Failed to activate Emscripten environment.
    exit /b 1
)

REM Configure CMake with Emscripten toolchain
echo Configuring CMake for Emscripten...
emcmake cmake -B %BUILD_DIR% -S .
if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed.
    exit /b 1
)

REM Build all wasm targets (app and test) -> outputs to wasm/pkg
echo Building WebGPU app and test modules...
cmake --build %BUILD_DIR% -j4
if %ERRORLEVEL% NEQ 0 (
    echo Build failed.
    exit /b 1
)

echo Build successful! Output files are in %CD%\pkg\

