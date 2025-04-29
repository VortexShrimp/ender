#include <windows.h>
#include <thread>

// Include the examples.
#include "crypto_price_checker\price_checker.hpp"
#include "simple_window\simple_window.hpp"

// Example Windows entry point.
INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    // Requires updating...
    // crypto_price_checker::run_example();

    examples::simple_window::run_example();

    return 1;
}
