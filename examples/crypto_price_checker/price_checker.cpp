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
    if (app->create(create_handler, {.title = L"Price Checker",
                                     .class_name = L"cryto_miner",
                                     .width = 300,
                                     .height = 300,
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
        // Setup custom Lua API here.
        if (lua_create() == true) {
            m_lua_state["set_page_number"] = [this](int new_number) {
                set_page_number(new_number);
            };

            m_lua_state["create_coin_table"] = [this]() { return create_coin_table(); };
            m_lua_state["update_coin_table"] = [this]() { update_coin_table(m_coin_id); };

            // Custom imgui API.
            m_lua_state["imgui_coin_id_input"] = [this](const char* label) {
                ImGui::InputInt(label, &m_coin_id);
            };

            m_lua_state.script("crypto_on_create()");
        }

        return true;
    } catch (std::exception& ex) {
        ender::debug_print_formatted("Exception: {}\n", ex.what());
        MessageBox(nullptr, ender::console::multibyte_to_unicode(ex.what()).c_str(),
                   L"Lua Exception", MB_ICONERROR);
        return true;
    }
}

void crypto_price_checker::app_window::on_destroy_window() {
    try {
        m_lua_state.script("crypto_on_destroy()");
    } catch (std::exception& ex) {
        ender::debug_print_formatted("Exception: {}\n", ex.what());
        MessageBox(nullptr, ender::console::multibyte_to_unicode(ex.what()).c_str(),
                   L"Lua Exception", MB_ICONERROR);
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
        MessageBox(nullptr, ender::console::multibyte_to_unicode(ex.what()).c_str(),
                   L"Lua Exception", MB_ICONERROR);
    }

    return true;
}

void crypto_price_checker::app_window::on_render_frame() {
    try {
        m_lua_state.script(std::format("crypto_on_render_imgui({})", m_page_number));
    } catch (std::exception& ex) {
        ender::debug_print_formatted("Exception: {}\n", ex.what());
        MessageBox(nullptr, ender::console::multibyte_to_unicode(ex.what()).c_str(),
                   L"Lua Exception", MB_ICONERROR);
    }
}

void crypto_price_checker::app_window::set_page_number(int new_number) {
    m_page_number = new_number;
}

void crypto_price_checker::app_window::create_coin_table() {
    sol::table current_coin_table = m_lua_state.create_table("current_coin");

    // Create the fields in the table with defualt values.
    current_coin_table["name"] = "";
    current_coin_table["rank"] = 0;
    current_coin_table["price"] = "";
    current_coin_table["change_1h"] = "";
    current_coin_table["change_24h"] = "";
    current_coin_table["change_7d"] = "";
    current_coin_table["price_bitcoin"] = "";
}

void crypto_price_checker::app_window::update_coin_table(int coin_index) {
    // https://www.coinlore.com/cryptocurrency-data-api#global
    auto callback = [this](std::string http_response) {
        const nlohmann::json coin_data = nlohmann::json::parse(http_response);
        if (coin_data.empty() == true) {
            return;
        }

        const std::string coin_name = coin_data[0]["name"];
        m_lua_state["current_coin"]["name"] = coin_name;

        const int coin_rank = coin_data[0]["rank"];
        m_lua_state["current_coin"]["rank"] = coin_rank;

        const std::string coin_price_usd = coin_data[0]["price_usd"];
        m_lua_state["current_coin"]["price"] = coin_price_usd;

        const std::string coin_change_1h = coin_data[0]["percent_change_1h"];
        m_lua_state["current_coin"]["change_1h"] = coin_change_1h;

        const std::string coin_change_24h = coin_data[0]["percent_change_24h"];
        m_lua_state["current_coin"]["change_24h"] = coin_change_24h;

        const std::string coin_change_7d = coin_data[0]["percent_change_7d"];
        m_lua_state["current_coin"]["change_7d"] = coin_change_7d;

        const std::string coin_price_bitcoin = coin_data[0]["price_btc"];
        m_lua_state["current_coin"]["price_bitcoin"] = coin_price_bitcoin;
    };

    // Do the get request in a seperate thread to not block the main UI.
    std::thread(ender::get_request_callback, "api.coinlore.net",
                std::format("/api/ticker/?id={}", coin_index), callback)
        .detach();
}
