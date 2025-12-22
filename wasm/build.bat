@echo off
REM Build script for Emscripten WASM modules (app.js/app.wasm + test.js/test.wasm)

set EMSDK_PATH=D:\dev\emsdk
set BUILD_DIR=build-web
set PKG_DIR=pkg

REM Activate Emscripten environment
call %EMSDK_PATH%\emsdk_env.bat
if %ERRORLEVEL% NEQ 0 (
    echo Failed to activate Emscripten environment.
    exit /b 1
)

REM Configure CMake with Emscripten toolchain
echo Configuring CMake for Emscripten...
call emcmake cmake -B %BUILD_DIR%
if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)
echo CMake configuration completed successfully.

REM Build all wasm targets (app and test)
echo.
echo Building WebGPU app and test modules...
call cmake --build %BUILD_DIR% -j4
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)
echo Build completed successfully.

REM Create pkg directory if it doesn't exist
if not exist %PKG_DIR% mkdir %PKG_DIR%

REM Copy output files from build-web to pkg
echo Copying output files to pkg directory...
copy /Y %BUILD_DIR%\app.js %PKG_DIR%\app.js >nul
copy /Y %BUILD_DIR%\app.wasm %PKG_DIR%\app.wasm >nul
copy /Y %BUILD_DIR%\test.js %PKG_DIR%\test.js >nul
copy /Y %BUILD_DIR%\test.wasm %PKG_DIR%\test.wasm >nul

if %ERRORLEVEL% NEQ 0 (
    echo Warning: Some files may not have been copied successfully.
) else (
    echo Build successful! Output files are in %CD%\%PKG_DIR%\
)

