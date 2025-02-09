#include "price_checker.hpp"

#include <nlohmann\json.hpp>

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

            m_lua_state["crypto_get_and_parse_api"] = [this]() {
                return get_and_parse_global_response();
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

void crypto_price_checker::app_window::get_and_parse_global_response() {
    {
        std::string json = get_request("api.coinlore.net", "/api/global");
        nlohmann::json json_data = nlohmann::json::parse(json);

        const int coins_count = json_data[0]["coins_count"];
        m_lua_state["global_coins_count"] = coins_count;
        const int active_markets = json_data[0]["active_markets"];
        m_lua_state["global_active_markets"] = active_markets;

        std::string bitcoin_dominance = json_data[0]["btc_d"];
        m_lua_state["global_bitcoin_dominance"] = bitcoin_dominance;
        std::string etherium_dominance = json_data[0]["eth_d"];
        m_lua_state["global_etherium_dominance"] = etherium_dominance;

        std::string volume_change = json_data[0]["volume_change"];
        m_lua_state["global_volume_change"] = volume_change;
        std::string average_change_percent = json_data[0]["avg_change_percent"];
        m_lua_state["global_change_percent"] = average_change_percent;
    }

    {}
}
