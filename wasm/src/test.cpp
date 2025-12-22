#include <emscripten.h>
#include <emscripten/bind.h>
#include <string>

// Simple test function to verify WASM compilation works
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    int add(int a, int b) {
        return a + b;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int multiply(int a, int b) {
        return a * b;
    }
}

// Using Emscripten bindings for more complex types (optional)
using namespace emscripten;

EMSCRIPTEN_KEEPALIVE
std::string greet(const std::string& name) {
    return "Hello, " + name + " from WASM!";
}

EMSCRIPTEN_BINDINGS(test_module) {
    function("greet", &greet);
}

