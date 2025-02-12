#pragma once
#include "../../ender/platform/lua_window.hpp"
#include "../../ender/utils/internet.hpp"

namespace crypto_price_checker {
    bool create_handler(ender::window* ctx);
    void destroy_handler(ender::window* ctx);
    bool process_events_handler(ender::window* ctx);
    void render_frame_handler(ender::window* ctx);

    class app_window : public ender::lua_window {
    public:
        app_window() : m_page_number(0), m_coin_id(90), lua_window() {
        }

        bool on_create_window();
        void on_destroy_window();
        bool on_process_events();
        void on_render_frame();

        //
        // Custom window code.
        //

    private:
        void set_page_number(int new_number);

        void create_coin_table();
        void update_coin_table(int index);

        int m_page_number;
        int m_coin_id;
    };

    int run_example();
}  // namespace crypto_price_checker
