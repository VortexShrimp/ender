#include "price_checker.hpp"

// For debug printing.
#include "../utils/console.hpp"
#include "../utils/internet.hpp"

bool crypto_price_checker::create_handler(ender::window* ctx) {
    auto app = static_cast<app_window*>(ctx);
    return app->on_create_window();
}

void crypto_price_checker::destroy_handler(ender::window* ctx) {
    auto app = static_cast<app_window*>(ctx);
    app->on_destroy_window();
}

bool crypto_price_checker::process_events_handler(ender::window* ctx) {
    auto app = static_cast<app_window*>(ctx);
    return app->on_process_events();
}

void crypto_price_checker::render_frame_handler(ender::window* ctx) {
    auto app = static_cast<app_window*>(ctx);
    app->on_render_frame();
}

int crypto_price_checker::run_example() {
    auto app = std::make_unique<app_window>();
    if (app->create(create_handler, {.title = L"price checker | ender",
                                     .class_name = L"cryto_miner",
                                     .width = 300,
                                     .height = 400,
                                     .on_message_create = nullptr,
                                     .on_message_destroy = nullptr,
                                     .on_message_close = nullptr}) == true) {
        while (app->process_events(process_events_handler) == true) {
            app->render_frame(render_frame_handler);
        }

        app->destroy(destroy_handler);
        return 0;
    }

    return 1;
}

bool crypto_price_checker::app_window::on_create_window() {
    try {
        if (lua_create() == true) {
            //
            // Setup custom Lua API here.
            //

            m_lua_state["set_page_number"] = [this](int new_number) {
                set_page_number(new_number);
            };

            m_lua_state["get_request"] = [this](std::string_view url, std::string_view objects) {
                return get_request(url, objects);
            };

            m_lua_state.script("crypto_on_create()");
        }

        return true;
    } catch (std::exception& e) {
        ender::debug_print_formatted("Exception: {}\n", e.what());
        return true;
    }
}

void crypto_price_checker::app_window::on_destroy_window() {
    try {
        m_lua_state.script("crypto_on_destroy()");
    } catch (std::exception& ex) {
        ender::debug_print_formatted("Exception: {}\n", ex.what());
    }
}

bool crypto_price_checker::app_window::on_process_events() {
    if (m_is_running == false) {
        return false;
    }

    try {
        m_lua_state.script("crypto_on_process_events()");
    } catch (std::exception& ex) {
        ender::debug_print_formatted("Exception: {}\n", ex.what());
    }

    return true;
}

void crypto_price_checker::app_window::on_render_frame() {
    try {
        m_lua_state.script(std::format("crypto_on_render_imgui({})", m_page_number));
    } catch (std::exception& ex) {
        ender::debug_print_formatted("Exception: {}\n", ex.what());
    }
}

void crypto_price_checker::app_window::set_page_number(int new_number) {
    m_page_number = new_number;
}

std::string crypto_price_checker::app_window::get_request(std::string_view url,
                                                          std::string_view objects) {
    std::string response = "";

    ender::internet_client client = {};
    if (client.create() == true) {
        client.get(url, objects, response);
    }

    return response;
}
