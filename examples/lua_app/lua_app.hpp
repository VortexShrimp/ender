#pragma once
#include "../../src/platform/window.hpp"
#include "../../src/utils/console.hpp"

namespace lua_app {
    class window : public ender::lua_window {
    public:
        bool on_create();
        void on_destroy();
        void on_render_frame();

    private:
        ender::os_console m_console;
    };

    bool on_create_handler(ender::window* ctx);
    void on_destroy_handler(ender::window* ctx);
    void on_render_frame_handler(ender::window* ctx);

    int run_lua_app();
}  // namespace lua_app