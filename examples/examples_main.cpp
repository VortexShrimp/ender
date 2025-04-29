#include <windows.h>
#include <thread>

// Include the examples.
#include "simple_window\simple_window.hpp"

// Example Windows entry point.
INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    examples::run_simple_window();

    return 1;
}
