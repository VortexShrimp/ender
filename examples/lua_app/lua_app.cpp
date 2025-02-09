#include "lua_app.hpp"

#include "../utils/console.hpp"

bool lua_app::window::on_create() {
    try {
        // Setup Lua.
        if (lua_create() == true) {
            m_lua_state.script("on_global_create()");
        }

        ender::debug_print_formatted("[crypto] Window created successfully\n");

        return true;
    } catch (std::exception& e) {
        ender::debug_print_formatted("Exception: {}\n", e.what());
        return true;
    }
}

void lua_app::window::on_destroy() {
    try {
        m_lua_state.script("on_global_destroy()");
    } catch (std::exception& e) {
        ender::debug_print_formatted("Exception: {}\n", e.what());
    }
}

void lua_app::window::on_process_events() {
    try {
        m_lua_state.script("on_global_process_events()");
        m_lua_state.collect_garbage();
    } catch (std::exception& e) {
        ender::debug_print_formatted("Exception: {}\n", e.what());
    }
}

void lua_app::window::on_render_frame() {
    try {
        m_lua_state.script("on_global_render_frame()");
    } catch (std::exception& e) {
        ender::debug_print_formatted("Exception: {}\n", e.what());
    }
}

bool lua_app::on_create_handler(ender::window* ctx) {
    auto app = static_cast<window*>(ctx);
    return app->on_create();
}

void lua_app::on_destroy_handler(ender::window* ctx) {
    auto app = static_cast<window*>(ctx);
    app->on_destroy();
}

void lua_app::on_process_events_handler(ender::window* ctx) {
    auto app = static_cast<window*>(ctx);
    app->on_process_events();
}

void lua_app::on_render_frame_handler(ender::window* ctx) {
    auto app = static_cast<window*>(ctx);
    app->on_render_frame();
}

int lua_app::run_lua_app() {
    auto app = std::make_unique<window>();
    if (app->create(on_create_handler, {.title = L"lua app",
                                        .class_name = L"lua_app",
                                        .width = 1280,
                                        .height = 720,
                                        .on_message_create = nullptr,
                                        .on_message_destroy = nullptr,
                                        .on_message_close = nullptr}) == true) {
        while (app->process_events(nullptr) == true) {
            app->render_frame(on_render_frame_handler);
        }

        app->destroy(on_destroy_handler);
        return 0;
    }

    return 1;
}
