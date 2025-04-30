#include <windows.h>

// Include the examples.
#include "simple_app/simple_app.hpp"

// Example Windows entry point.
INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    examples::run_simple_app();

    return 1;
}
