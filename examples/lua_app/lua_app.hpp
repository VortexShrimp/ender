#pragma once
#include "../../src/platform/window.hpp"

namespace lua_app {
    class window : public ender::lua_window {
    public:
        bool on_create();
        void on_render_frame();
    };

    bool on_create_handler(ender::window* ctx);
    void on_render_frame_handler(ender::window* ctx);

    int run_lua_app();
}  // namespace lua_app