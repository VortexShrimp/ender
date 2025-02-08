#pragma once
#include "../platform/lua_window.hpp"
#include "../utils/console.hpp"

namespace lua_app {
    class window : public ender::lua_window {
    public:
        bool on_create();
        void on_destroy();
        void on_process_events();
        void on_render_frame();
    };

    bool on_create_handler(ender::window* ctx);
    void on_destroy_handler(ender::window* ctx);
    void on_process_events_handler(ender::window* ctx);
    void on_render_frame_handler(ender::window* ctx);

    int run_lua_app();
}  // namespace lua_app