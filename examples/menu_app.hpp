#pragma once
#include <array>

// We just need a window from ender.
#include "../src/platform/window.hpp"
#include "../src/platform/console.hpp"

namespace menu_app {
    class menu_app : public ender::platform_window {
    public:
        menu_app()
            : m_console(),
              m_should_remember_me(true),
              m_current_page(pages::login),
              m_username_buffer(),
              platform_window() {
        }

        bool on_create() noexcept;
        void on_destroy() noexcept;
        bool on_handle_events() noexcept;
        void on_render_frame_imgui() noexcept;

        enum class pages { login, home };

    private:
        // We want a console for example.
        ender::platform_console m_console;

        // Custom application fields.
        bool m_should_remember_me;
        pages m_current_page;
        std::array<char, 256> m_username_buffer;
    };

    bool on_create_handler(ender::platform_window* ctx);
    void on_destroy_handler(ender::platform_window* ctx);
    bool on_handle_events_handler(ender::platform_window* ctx);
    void on_render_frame_handler(ender::platform_window* ctx);

    bool on_message_close_handler(ender::platform_window* ctx);

}  // namespace menu_app
