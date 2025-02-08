#include <windows.h>
#include <thread>

// Include the examples.
#include "menu_app\menu_app.hpp"
#include "lua_app\lua_app.hpp"

// Example Windows entry point.
INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    // TODO: Put these windows in their own threads.

    // std::thread menu_app(menu_app::run_menu_app);
    // menu_app.detach();

    lua_app::run_lua_app();

    return 1;
}
