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

            m_lua_state["get_and_update_current_coin"] = [this]() {
                get_and_update_current_coin(m_coin_id);
            };

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
    // https://www.coinlore.com/cryptocurrency-data-api#global
    const std::string global_json = get_request("api.coinlore.net", "/api/global");
    const nlohmann::json global_data = nlohmann::json::parse(global_json);

    sol::table global_data_table = m_lua_state.create_table("global_data");

    int coins_count = global_data[0]["coins_count"];
    global_data_table["coins"] = coins_count;
    const int active_markets = global_data[0]["active_markets"];
    global_data_table["markets"] = active_markets;

    const std::string bitcoin_dominance = global_data[0]["btc_d"];
    global_data_table["btc_dominance"] = bitcoin_dominance;
    const std::string etherium_dominance = global_data[0]["eth_d"];
    global_data_table["eth_dominance"] = etherium_dominance;

    const std::string volume_change = global_data[0]["volume_change"];
    global_data_table["volume_change"] = volume_change;
    const std::string average_change_percent = global_data[0]["avg_change_percent"];
    global_data_table["average_change_percent"] = average_change_percent;

    // Create it here to avoid copies.
    m_lua_state.create_table("current_coin");
}

void crypto_price_checker::app_window::get_and_update_current_coin(int index) {
    // https://www.coinlore.com/cryptocurrency-data-api#global
    // https://api.coinlore.net/api/ticker/?id=90
    const std::string url_objects = std::format("/api/ticker/?id={}", index);
    const std::string coin_json = get_request("api.coinlore.net", url_objects);
    const nlohmann::json coin_data = nlohmann::json::parse(coin_json);

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
}
