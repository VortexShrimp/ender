#include <windows.h>
#include <thread>

// Include the examples.
#include "cryto_price_checker\price_checker.hpp"
#include "menu_app\menu_app.hpp"
#include "lua_app\lua_app.hpp"

// Example Windows entry point.
INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    crypto_price_checker::run_example();

    return 1;
}
