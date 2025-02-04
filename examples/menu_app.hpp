#pragma once

// We just need a window from ender.
#include "../src/ender/platform/window.hpp"

namespace menu_app {
    class menu_app : public ender::game_window {
    public:
        menu_app() : m_current_page(pages::login), game_window() {
        }

        bool on_create() noexcept;
        void on_destroy() noexcept;
        bool on_handle_events() noexcept;
        void on_render_frame_imgui() noexcept;

        enum class pages { login, home };

    private:
        pages m_current_page;
    };

    bool on_create_handler(ender::game_window* ctx);
    void on_destroy_handler(ender::game_window* ctx);
    bool on_handle_events_handler(ender::game_window* ctx);
    void on_render_frame_handler(ender::game_window* ctx);

    bool on_message_close_handler(ender::game_window* ctx);

}  // namespace menu_app
