#include <windows.h>

// Include the examples.
#include "menu_app\menu_app.hpp"
#include "lua_app\lua_app.hpp"

// Example Windows entry point.
INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    // HANDLE menu_app_thread =
    //     CreateThread(nullptr, 0,
    //     reinterpret_cast<LPTHREAD_START_ROUTINE>(menu_app::run_menu_app),
    //                  nullptr, 0, nullptr);
    // if (menu_app_thread != INVALID_HANDLE_VALUE) {
    //     CloseHandle(menu_app_thread);
    // }

    /*return lua_app::run_lua_app();*/

    lua_app::run_lua_app();
    menu_app::run_menu_app();

    return 1;
}
