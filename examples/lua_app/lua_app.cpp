#include "lua_app.hpp"

#include "../src/utils/console.hpp"

bool lua_app::window::on_create() {
    if (create_lua_state() == false) {
        return false;
    }

    m_lua_state.script("on_global_window_create()");

    return true;
}

void lua_app::window::on_destroy() {
    m_lua_state.script("on_global_window_destroy()");
}

void lua_app::window::on_render_frame() {
    m_lua_state.script("on_global_window_render_frame()");
}

bool lua_app::on_create_handler(ender::window* ctx) {
    auto app = static_cast<window*>(ctx);
    return app->on_create();
}

void lua_app::on_destroy_handler(ender::window* ctx) {
    auto app = static_cast<window*>(ctx);
    app->on_destroy();
}

void lua_app::on_render_frame_handler(ender::window* ctx) {
    auto app = static_cast<window*>(ctx);
    app->on_render_frame();
}

int lua_app::run_lua_app() {
    // Call anywhere to allocate a console in DEBUG builds.
    ender::debug_print_raw("Welcome to ender!\n");

    auto app = std::make_unique<window>();
    if (app->create(on_create_handler, {.title = L"lua app",
                                        .width = 1280,
                                        .height = 720,
                                        .on_message_create = nullptr,
                                        .on_message_destroy = nullptr,
                                        .on_message_close = nullptr,
                                        .instance = nullptr,
                                        .cmd_show = SW_SHOWDEFAULT}) == true) {
        while (app->process_events(nullptr) == true) {
            app->render_frame(on_render_frame_handler);
        }

        app->destroy(on_destroy_handler);
        return 0;
    }

    return 1;
}
